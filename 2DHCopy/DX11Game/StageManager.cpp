/**インクルード部**/
#include "StageManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "sceneGame.h"
#include "Object.h"
#include "ObjInfo.h"
#include "imgui.h"
#include "Camera.h"
//ファイルを管理するマネージャーを作る
#include "FileManager.h"
//CSVを読み込む機能を読み込む
#include "ResourceCSV.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "SeeCollComponent.h"
#include "AnimMeshComponent.h"
#include "Draw3dComponent.h"
#include "Draw2DComponent.h"
#include "ModelManager.h"
#include "GravityComponent.h"
#include "OutOfRange.h"
#include "EnemyComponent.h"
#include "BillboardComponent.h"
#include "ItemComponent.h"

//ステージのインクルード
#include "SceneStage_1.h"
#include "SceneStage_1_Re.h"

/**列挙体宣言**/
/** @brief どのオブジェクトを配置するかを判別する*/
enum MAP_CHIP{
	N = -1,				//何もない
	ENEMY_1 = 1,			//敵

	//緑
	B_1 = 10,			//ブロック(上部) 
	GRASS_IN = 11,		//ブロック(内部(当たり判定無し))
	GRASS_IN_COLL = 12,	//ブロック(内部(当たり判定あり))

	GOAL = 50,			//ゴール
	KOBAN,				//小判(スコアアップアイテム)
	MAGA,				//勾玉(パワーアップアイテム)
	OHUDA,				//お札(キーアイテム)

	MISS_COLL = 99,					//ミスブロック
	WARP,							//ステージごとにワープの条件を変える
	STAGE_1_MISS_COLL = 199,		//ステージ1イベントミス

	STAGE_1_RE_CHANGE_COLL = 299,	//オブジェクトの見た目を変えるためのイベント

	SOCRE_UP = 2000,				//所謂コイン的なやつ

	ARROW = 9999,					//ビルボード(仮)

	MAX_MAP_CHIP
};

/**静的メンバ変数**/
StageManager* StageManager::m_pInstance = nullptr;

/**
* @fn		StageManager::GetInstance
* @brief	インスタンスの取得
*/
StageManager* StageManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new StageManager();
	}

	return m_pInstance;
}

/**
* @fn		StageManager::Destroy
* @brief	インスタンスの破棄
*/
void StageManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**	
* @fn		StageManager::StageManager
* @brief	コンストラクタを今・・・ここで決める！！！
*/
StageManager::StageManager() 
	:m_nBlock_X(0),m_nBlock_Y(0),m_nMapSta(N)
	,m_pShowObject(nullptr){
	//画面に移っているワールド座標の立幅と横幅を計算する
	m_WorldSize = CalcWorldSize();

	//ステージ描画の起点
	m_BasePos.x = -(m_WorldSize.x - MAPCHIP_WIDTH)  * 0.5f - MAPCHIP_WIDTH;
	m_BasePos.y =  (m_WorldSize.y - MAPCHIP_HEIGHT) * 0.5f;
}

/**
* @fn		StageManager::~StageManager
* @brief	ｸｯ　デストラクタに負けた・・・
*/
StageManager::~StageManager() {
	if (m_pShowObject) {
		delete m_pShowObject;
		m_pShowObject = nullptr;
	}
}

/**
* @fn		StageManager::Update
* @brief	更新処理
*/
void StageManager::Update() {
#ifdef _DEBUG
	//編集モード
	if (SceneGame::GetInstance()->m_bEditMode) {
		//マウスのスクリーン座標をワールド座標に変換する
		InputManager* Input = InputManager::Instance();
		XMFLOAT2 Mouse_Pos = Input->GetMousePos();
		XMMATRIX view_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
		XMMATRIX prj_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
		XMFLOAT3 worldPos;		// 算出結果を格納する
		CalcScreenToXY(&worldPos, Mouse_Pos.x, Mouse_Pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view_2, prj_2);

		//何か置きたい物があったら追加していく
		if (Input->GetKeyTrigger(DIK_1)) m_nMapSta = B_1;				//ブロック1である

		//要素数を出すための計算をする
		m_Element_X = (int)((worldPos.x + m_WorldSize.x * 0.5f) / (MAPCHIP_WIDTH));
		m_Element_Y = (int)((-worldPos.y + m_WorldSize.y * 0.5f) / (MAPCHIP_HEIGHT));

		//生成座標を求める
		float CreatePosX = m_BasePos.x + (m_Element_X)* MAPCHIP_WIDTH;		//生成起点から要素数×ブロックのサイズ分だけ移動した点
		float CreatePosY = m_BasePos.y - (m_Element_Y)* MAPCHIP_HEIGHT;

		// カーソルで表示するもの生成
		this->UpdateShowObject(CreatePosX, CreatePosY);

		//ボタンを押したらオブジェクトを追加する
		if (Input->GetMouseTrigger(MOUSE_L))
		{
			// 生成する場所のブロックIDを求める
			int nMapChipID = (m_Element_Y * m_nBlock_X) + m_Element_X;	//yの要素サイズ × xの要素サイズ + xの要素数

			//オブジェクトを作る(生成座標x, 生成座標y, ステージ配列)
			CreateBlock_Deb(CreatePosX, CreatePosY, nMapChipID, m_Element_X, m_Element_Y, m_nMapSta);

			//2021/12/30 narita
			// ファイル書き換え
			std::string file;
			switch (SceneGame::GetInstance()->GetStage())
			{
				//読み込むCSVを選択(?)
			case STAGE_1:break;
			default: break;
			}
			ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
			CSV->SetInt(m_Element_X, m_Element_Y, m_nMapSta);
		}

		// ステージをcsvに保存
		if (Input->GetKeyPress(DIK_LCONTROL) &&
			Input->GetKeyTrigger(DIK_S))
		{
			this->Save(SceneGame::GetInstance()->GetStage());
		}

		// カーソルオブジェクトの更新
		if (m_pShowObject)	m_pShowObject->Update();
		//-----------------------------------------------------------------------------------------------------

		else

			// 編集モードの終わり
			if (m_pShowObject)
			{
				delete m_pShowObject;
				m_pShowObject = nullptr;
			}
	}
#endif
}

/** 
* @fn		StageManager::Draw
* @brief	描画するよ
* @detail	置きたいオブジェクトとかデバッグ用の描画
*/
void StageManager::Draw() {
	//カーソルオブジェクトの描画
	if (m_pShowObject) {
		m_pShowObject->Draw();
	}

#ifdef _DEBUG
	using namespace ImGui;
	SetNextWindowSize(ImVec2(120, 120), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(120, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Stage");

	//マウスのスクリーン座標をワールド座標に変換する
	InputManager* Input = InputManager::Instance();
	XMFLOAT2 Mouse_Pos = Input->GetMousePos();
	XMMATRIX view_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetViewMatrix());
	XMMATRIX prj_2 = DirectX::XMLoadFloat4x4(&CCamera::Get()->GetProjMatrix());
	XMFLOAT3 worldPos;		// 算出結果を格納する
	CalcScreenToXY(&worldPos, Mouse_Pos.x, Mouse_Pos.y, SCREEN_WIDTH, SCREEN_HEIGHT, view_2, prj_2);
	//要素数を出すための計算をする
	int Num_x = (int)((worldPos.x + m_WorldSize.x * 0.5f) / (MAPCHIP_WIDTH));	// 要素ｘ
	int Num_y = (int)((-worldPos.y + m_WorldSize.y * 0.5f) / (MAPCHIP_HEIGHT));	// 要素ｙ
	// 生成する場所のブロックIDを求める
	int nMapChipID = (Num_y * m_nBlock_X) + Num_x;	//yの要素サイズ × xの要素サイズ + xの要素数
	Text("ID   : %2d", nMapChipID);

	End();

#endif // _DEBUG

}

/**
* @fn		StageManager::CreateStage
* @brief	ステージの地形の作成
* @param	(int)	ステージの番号(何番目のステージか)
*/
void StageManager::CreateStage(int stage_state) {
	//ステージの読み込む(番号によって読み込むcsvを変更)
	switch (stage_state) {
	case STAGE_1:this->Load(STAGE_1_CSV); break;
	case STAGE_1_RE:this->Load(STAGE_1_RE_CSV); break;
	default:break;
	}

	float fBaseX = m_BasePos.x;
	float fBaseY = m_BasePos.y;
	int nBlockID = 0;

	//マップの配列だけループ
	for (int y = 0;y < (int)m_StageGrid.size();y++) {
		for (int x = 0;x < (int)m_StageGrid[y].size();x++) {
			Object* pObject;
			//ブロック生成
			pObject = CreateBlock(fBaseX,fBaseY,m_StageGrid[y][x],nBlockID);

			//次のブロックの座標へ
			fBaseX += MAPCHIP_WIDTH;
			nBlockID++;
		}

		//X座標をリセットする
		fBaseX = m_BasePos.x;
		//次のブロック座標へ(Y座標)
		fBaseY -= MAPCHIP_HEIGHT;
	}

	//ステージの要素数を保管
	//Yの要素数
	m_nBlock_Y = (int)m_StageGrid.size();
	for (auto row:m_StageGrid) {
		//1番サイズが大きい列を反映
		if (m_nBlock_X < (int)row.size()) {
			//Xの要素数
			m_nBlock_X = (int)row.size();
		}
	}

	//ステージをサイズを格納
	//要素数×マップチップの横幅
	m_vStageSize.x = m_nBlock_X * MAPCHIP_WIDTH;
	m_vStageSize.y = m_nBlock_Y * MAPCHIP_HEIGHT;

	//ステージが作り終わったら選ばれているステージごとに、
	//設定しないといけないことを設定する
	switch (stage_state)
	{
	case STAGE_1:
		//リストの中身があったら消す作業を挟む
		SceneStage_1::GetInstance()->DeleteList();
		SceneStage_1::GetInstance()->SetBaseInfo(ObjectManager::GetInstance()->GetUpdateList());
		break;
	case STAGE_1_RE:
		SceneStage_1_Re::GetInstance()->DeleteList();
		SceneStage_1_Re::GetInstance()->SetBaseInfo(ObjectManager::GetInstance()->GetUpdateList());
		break;
	default:break;
	}

}

/**
* @fn		StageManager::CreateBlock
* @brief	ブロックの生成
* @param	(float)		X座標
* @param	(float)		Y座標
* @param	(int)		オブジェクトナンバー
* @param	(int)		ブロックと紐付いているキー
* @return	(Object*)	作られたオブジェクトのポインタ
*/
Object* StageManager::CreateBlock(float fPosX,float fPosY,int nState,int nBlockID) {
	//オブジェクトの番号がNだったら生成しない
	if (nState == N) {
		return nullptr;
	}

	//モデルマネージャーとテクスチャマネージャー
	ModelManager* pModelManager = ModelManager::GetInstance();
	TextureManager* pTextureManager = TextureManager::GetInstance();

#pragma region ---緑ブロック
	//ブロック
	if (nState == B_1) {
		Object* obj = new Object(BLOCK_NAME,UPDATE_FIELD,DRAW_FIELD);
		//コンポーネントの追加
		auto trans    = obj->AddComponent<CTransform>();
		auto draw	  = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		draw->SetModel(pModelManager->GetModel(MINT_GREEN_BLOCK_NUM));
		trans->SetPosition(fPosX,fPosY+770,0.0f);
		trans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();
		
		return obj;
	}
#pragma endregion
#pragma region ---土ブロック(当たり判定無し) 
	//土ブロック
	else if (nState == GRASS_IN) {
		Object* obj = new Object(BLOCK_RARD_NAME, UPDATE_FIELD, DRAW_FIELD);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto Range = obj->AddComponent<COutOfRange>();
		//オブジェクトの設定
		draw->SetModel(pModelManager->GetModel(RARD_BLOCK_NUM));
		trans->SetPosition(fPosX, fPosY + 770, 0.0f);
		trans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		//collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---土ブロック(当たり判定有り)
	//土ブロック
	else if (nState == GRASS_IN_COLL) {
		Object* obj = new Object(BLOCK_RARD_NAME, UPDATE_FIELD, DRAW_FIELD);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto Range = obj->AddComponent<COutOfRange>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		draw->SetModel(pModelManager->GetModel(RARD_BLOCK_NUM));
		trans->SetPosition(fPosX, fPosY + 770, 0.0f);
		trans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---歩く敵
	//歩く敵
	else if(nState == ENEMY_1){
		//オブジェクトの生成
		Object* obj = new Object(ENEMY_NAME,UPDATE_MODEL,DRAW_MODEL);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<CSeeColl>();
		obj->AddComponent<COutOfRange>();
		obj->AddComponent<CGravity>();
		//敵専用のコンポーネントを追加する予定
		obj->AddComponent<CEnemy>();
		//オブジェクトの設定
		draw->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
		trans->SetPosition(fPosX,fPosY+800,0.0f);//800
		trans->SetScale(E_WALK_SIZE_X, E_WALK_SIZE_Y, E_WALK_SIZE_Z);
		trans->SetRotate(0.0f,90.0f,0.0f);
		collider->SetCollisionSize(E_WALK_COLL_X, E_WALK_COLL_Y, E_WALK_COLL_Z);
		obj->GetComponent<CEnemy>()->SetEnemyType(ENEMY_WALK);
		//オブジェクトマネージャに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---ゴール
	else if (nState == GOAL) {
		//オブジェクトの生成
		Object* obj = new Object(GOAL_NAME,UPDATE_MODEL,DRAW_MODEL);
		//コンポーネントの追加
		auto trans	  = obj->AddComponent<CTransform>();
		auto draw	  = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		draw->SetModel(pModelManager->GetModel(WALK_ENEMY_MODEL_NUM));
		trans->SetPosition(fPosX, fPosY + 765, 0.0f);
		trans->SetScale(E_WALK_SIZE_X, E_WALK_SIZE_Y, E_WALK_SIZE_Z);
		collider->SetCollisionSize(E_WALK_COLL_X, E_WALK_COLL_Y, E_WALK_COLL_Z);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);
		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---ミス判定
	else if (nState == MISS_COLL) {
		//オブジェクトの生成(当たり判定のみ)
		Object* obj = new Object("MISS_COLL",UPDATE_DEBUG,DRAW_DEBUG);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDrawMesh>();
		auto collider = obj->AddComponent<CCollider>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		draw->SetTexture(pTextureManager->GetTexture(DEBUG_BLOCK_NUM));
		draw->SetSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y);
		//draw->SetLoop(true);
		trans->SetPosition(fPosX, fPosY + 940, 0.0f);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---ワープ地点を生成
	else if (nState == WARP) {
		//オブジェクトの生成
		Object* obj = new Object(WARP_NAME,UPDATE_UI,DRAW_UI);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定

		
	}
#pragma endregion
#pragma region ---ステージ1のミス判定
	else if (nState == STAGE_1_MISS_COLL) {
		//オブジェクトの生成
		Object* obj = new Object(STAGE_1_MISS_EVENT,UPDATE_DEBUG,DRAW_DEBUG);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDrawMesh>();
		auto collider = obj->AddComponent<CCollider>();
		auto range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		draw->SetTexture(pTextureManager->GetTexture(DEBUG_BLOCK_NUM));
		draw->SetSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y);
		trans->SetPosition(fPosX, fPosY + 770.0f, 0.0f);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);
		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---ステージ1(リローデッド)のイベント当たり判定
	else if (nState == STAGE_1_RE_CHANGE_COLL) {
		//オブジェクトの生成
		Object* obj = new Object(STAGE_RE_1_CHANGE_COLL,UPDATE_DEBUG,DRAW_DEBUG);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDrawMesh>();
		auto collider = obj->AddComponent<CCollider>();
		auto range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		draw->SetTexture(pTextureManager->GetTexture(DEBUG_BLOCK_NUM));
		draw->SetSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y);
		trans->SetPosition(fPosX, fPosY + 770.0f, 0.0f);
		collider->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);
		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---小判
	else if (nState == KOBAN) {
		//オブジェクトの生成
		Object* obj = new Object(ITEM_NAME,UPDATE_DEBUG,DRAW_DEBUG);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto type = obj->AddComponent<CItem>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		type->SetItem(ITEM_KOBAN);
		draw->SetModel(pModelManager->GetModel(KOBAN_MODEL_NUM));
		trans->SetPosition(fPosX,fPosY+770.0f,0.0f);
		trans->SetScale(KOBAN_SIZE_X, KOBAN_SIZE_Y, KOBAN_SIZE_Z);
		collider->SetCollisionSize(KOBAN_COLL_SIZE_X, KOBAN_COLL_SIZE_Y, KOBAN_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();

		return obj;
}
#pragma endregion
#pragma region ---勾玉
	else if (nState == MAGA) {
		//オブジェクトの生成
		Object* obj = new Object(ITEM_NAME,UPDATE_MODEL,DRAW_MODEL);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto type = obj->AddComponent<CItem>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		type->SetItem(ITEM_MAGA);
		draw->SetModel(pModelManager->GetModel(MAGA_MODEL_NUM));
		trans->SetPosition(fPosX+20.0f,fPosY + 770.0f,0.0f);
		trans->SetScale(MAGA_SIZE_X,MAGA_SIZE_Y, MAGA_SIZE_Z);
		collider->SetCollisionSize(MAGA_COLL_SIZE_X,MAGA_COLL_SIZE_Y,MAGA_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);

		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion 
#pragma region ---お札
	else if (nState == OHUDA) {
		//オブジェクトの生成
		Object* obj = new Object(ITEM_NAME,UPDATE_MODEL,DRAW_MODEL);
		//コンポーネントの追加
		auto trans = obj->AddComponent<CTransform>();
		auto draw = obj->AddComponent<CDraw3D>();
		auto collider = obj->AddComponent<CCollider>();
		auto type = obj->AddComponent<CItem>();
		auto Range = obj->AddComponent<COutOfRange>();
		obj->AddComponent<CSeeColl>();
		//オブジェクトの設定
		type->SetItem(ITEM_OHUDA);
		draw->SetModel(pModelManager->GetModel(OHUDA_MODEL_NUM));
		trans->SetPosition(fPosX + 20.0f, fPosY + 770.0f, 0.0f);
		trans->SetScale(OHUDA_SIZE_X, OHUDA_SIZE_Y, OHUDA_SIZE_Z);
		collider->SetCollisionSize(OHUDA_COLL_SIZE_X, OHUDA_COLL_SIZE_Y, OHUDA_COLL_SIZE_Z);
		Range->SetLimitRange(BLOCK_OUT_RANGE_X, BLOCK_OUT_RANGE_Y);
		//オブジェクトマネージャーに追加
		ObjectManager::GetInstance()->AddObject(obj);
		//ワールドマトリックスの更新
		draw->Update();

		return obj;
	}
#pragma endregion
#pragma region ---絶対消して役目でしょ
	else if (nState == ARROW) {
		//オブジェ生成
		Object* Arr = new Object("Arr",UPDATE_UI,DRAW_UI);
		//コンポネント
		auto trans = Arr->AddComponent<CTransform>();
		auto draw = Arr->AddComponent<CBillboard>();
		Arr->AddComponent<COutOfRange>();
		draw->SetTexture(TextureManager::GetInstance()->GetTexture(ARROW_NUM));
		draw->SetAnimSplit(1,1);
		draw->SetSize(E_WALK_SIZE_X, E_WALK_SIZE_Y);
		trans->SetPosition(fPosX, fPosY + 700, 0.0f);//800
		ObjectManager::GetInstance()->AddObject(Arr);

		draw->Update();

		return Arr;
		
	}
#pragma endregion
}

/**
* @fn		StageManager::CreateBlock_Deb
* @brief	マウス操作でのブロックの生成と削除
* @param	(float)	X座標
* @param	(float)	Y座標
* @param	(int)	消すブロックの番号
* @param	(int)	グリッドに登録するための番号X
* @param	(int)	グリッドに登録するための番号Y
* @param	(int)	作り出すブロックの種類
*/
void StageManager::CreateBlock_Deb(float fPosX,float fPosY,int nNumber,int x,int y,int nCreate_Status) {
	//idからオブジェクトブロック情報を取得
	Object *pBlock_Del = StageManager::GetBlock(nNumber);
	//オブジェクトが存在していたら削除する
	if (pBlock_Del) {
		//オブジェクトを削除
		pBlock_Del->Delete();
	}

	//ブロックを生成する
	this->CreateBlock(fPosX,fPosY,nCreate_Status,nNumber);
	//ステージ配列に登録
	m_StageGrid[x][y] = nCreate_Status;
}

/**
* @fn		StageManager::GetBlock
* @brief	idからオブジェクトの情報を取得する
* @param	(int)		オブジェクトと紐づいているオブジェクト
* @return	(Object*)	存在するかどうか
*/
Object* StageManager::GetBlock(int nNum) {

	//なにも存在しなかったら削除する
	//何かしらあったら返す
	//つまりそのポイントの座標も必要になってくる
	//座標はマウス座標をワールド座標にしたもの
	return nullptr;
}

/**
* @fn		Stagemanager::UpdateShowObject
* @brief	配置するオブジェクトの表示の更新
* @param	(float)	生成するX座標
* @param	(float)	生成するY座標
*/
void StageManager::UpdateShowObject(float fPosX,float fPosY) {
	if (!m_pShowObject) {
		//オブジェクトの生成
		m_pShowObject = new Object("debug",UPDATE_DEBUG,DRAW_DEBUG);
		//コンポーネントの追加
		auto trans = m_pShowObject->AddComponent<CTransform>();
		auto draw  = m_pShowObject->AddComponent<CDraw3D>();
		m_pShowObject->Start();
	}

	//座標の設定
	auto Transform = m_pShowObject->GetComponent<CTransform>();
	auto Draw = m_pShowObject->GetComponent<CDraw3D>();
	Transform->SetPosition(fPosX,fPosY);
	Transform->SetScale(1.0f,1.0f,1.0f);

	//モデルの設定
	if (m_nMapSta == B_1) {
		Transform->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		ModelManager* pModelManager = ModelManager::GetInstance();
		Draw->SetModel(pModelManager->GetModel(MINT_GREEN_BLOCK_NUM));
	}
}

/**
* @fn		StageManager::Save
* @brief	ステージ情報の書き込み
* @param	(int)	何番目のステージか
* @return	(bool)	問題なく書き込めたか
*/
bool StageManager::Save(int Stage) {
	//保存するCSVファイルを決める
	std::string file;
	switch (Stage) {
	case STAGE_1:file = STAGE_1_CSV; break;
	case STAGE_1_RE:file = STAGE_1_RE_CSV; break;
	default: break;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(file);
	if (CSV == nullptr) {
		return false;
	}

	//セーブする
	if (CSV->Save(file) == false) {
		//エラーメッセージ
		MessageBox(GetMainWnd(), _T("stage_save"), NULL, MB_OK);
		return false;
	}
	else {
		switch (Stage) {
		case STAGE_1:MessageBox(GetMainWnd(), L"Stage_1.csvに保存しました", _T("成功"), MB_OK); break;
		case STAGE_1_RE:MessageBox(GetMainWnd(), L"Stage_1_RE.csvに保存しました", _T("成功"), MB_OK); break;
		default: break;
		}
	}

	return true;
}

/**
* @fn		StageManager::Load
* @brief	ステージの地形情報の読み取り
* @param	(string)	ファイル名
* @return	(bool)	問題なく読み込めたか
*/
bool StageManager::Load(std::string fileName) {

	//ロード
	if (FileManager::GetInstance()->Load(fileName) == false) {
		//エラーメッセージ
		MessageBox(GetMainWnd(), _T("stage_load"), NULL, MB_OK);
		return false;
	}

	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(fileName);

	//ゲーム用の配列に格納する
	m_StageGrid.resize(CSV->GetRowSize());
	for (int y = 0;y <CSV->GetRowSize();++y) {
		m_StageGrid[y].resize(CSV->GetColumnSize(y));

		for (int x = 0;x < CSV->GetColumnSize(y);x++) {
			//要素を格納する
			m_StageGrid[y][x] = CSV->GetInt(x,y);
		}

	}

	return true;
}
