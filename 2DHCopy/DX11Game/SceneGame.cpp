//インクルード部
#include "sceneGame.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "FileManager.h"
#include "StageManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "Camera.h"
#include "imgui.h"


#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TexScrollComponent.h"
#include "Draw3dComponent.h"
#include "PlayerComponent.h"
#include "SeeCollComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "GravityComponent.h"

//ステージ関係のインクルード
#include "SceneStage_1.h"

//静的メンバ変数
SceneGame* SceneGame::m_pInstance = nullptr;

/**
* @fn		SceneGame::SceneGame
* @brief	コンストラクタ
* @detail	追加したいステージ等はここで追加していく
*/
SceneGame::SceneGame()
	:m_bPauseMode(false), m_bEditMode(false) 
	,m_pStageManager(nullptr){

	//ここでステージをリストに追加していく
	m_GameSceneList.push_back(new SceneStage_1());
	m_GameSceneList.push_back(new SceneStage_1());

	//最初のステージの設定
	m_eStageState = STAGE_1;
	m_CurrentScene = m_GameSceneList[m_eStageState];
}

/**
* @fn		SceneGame::~SceneGame
* @brief	デストラクタでステージをぶっこわーす
*/
SceneGame::~SceneGame() {
	//シーンリストを削除する
	for (auto scene : m_GameSceneList) {
		delete scene;
	}

	//リストを空にする
	m_GameSceneList.clear();

}

/** 
* @fn		SceneGame::Init
* @brief	初期化をする
*/
void SceneGame::Init() {
	//カメラを初期化する
	CCamera::Get()->Init();

	//編集モードとポーズフラグをオフにする
	m_bEditMode = false;
	m_bPauseMode = false;

	//ゲームオブジェクトの初期化

	//管理クラスインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();
	m_pStageManager = StageManager::GetInstance();
	pModelManager->AddModel(PATH_ROSALINA_X, ROSALINA_MODEL_X);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//テクスチャの読込
	TextureManager* pTexManager = TextureManager::GetInstance();

	//仮
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);

	//オブジェクトの作成
	//仮
	Object* Box = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	auto CollBox = Box->AddComponent<CCollider>();
	Box->AddComponent<CPlayer>();
	Box->AddComponent<CGravity>();
	Box->AddComponent<CSeeColl>();
	//設定仮
	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	TransBox->SetPosition(35.0f, 150.0f);
	CollBox->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
	//CollBox->SetOffset(-2.5f, 0.0f);

	DrawBox->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
	DrawBox->SetAnimSplit(3,3);
	DrawBox->SetSwapFrame(3);
	DrawBox->SetLoop(true);

	m_pObjectManager->AddObject(Box);	

	//ステージを作る
	m_pStageManager->CreateStage(m_eStageState);
}

/**
* @fn		SceneGame::Uninti
* @brief	終了処理
*/
void SceneGame::Uninit() {
	//オブジェクトの終了
	m_pObjectManager->Uninit();
}

/**
* @fn		SceneGame::Update
* @brief	更新処理
*/
void SceneGame::Update() {
	//カメラの更新処理
	CCamera::Get()->Update();

	//ステージ更新
	m_pStageManager->Update();

	//ゲームモード
	if (m_bEditMode == false) {
		if (m_bPauseMode == false) {
			
		}

		//ゲームオブジェクト更新
		m_pObjectManager->Update();

	}
	else if (m_bEditMode == true) {
		m_pObjectManager->UpdateEdit();
	}

	//ここでプレイヤーの速度を見る
	//下向きの速度がかかっていた場合落下判定とする
	if (m_pObjectManager->GetGameObject(PLAYER_NAME)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		if (Player->GetComponent<CTransform>()->Vel.y <= -MAX_VELOCITY) {
			Player->GetComponent<CPlayer>()->SetPlayerState(FALL_PLAYER);
		}
	}

#ifdef _DEBUG
	//何かしらの処理をしたらステータスを変える
	if (InputManager::Instance()->GetKeyTrigger(DIK_P)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		Player->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);
	}

	//	編集モードの切り替え
	if (InputManager::Instance()->GetKeyTrigger(DIK_F12))
	{
		m_bEditMode = m_bEditMode ^ 1;	// 切り替え

		//CSound::Play(SE_PAUSE);	// ←これ追加

		//	切り替えと同時にカメラの注視点の変更
		if (m_bEditMode == true)	CCamera::Get()->SetAxisX(nullptr);	//edit	プレイヤーから注視店を外す
		else						CCamera::Get()->SetAxisX(&ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x);	//game	プレイヤーを注視店に設定
	}
#endif

}

/** 
* @fn		SceneGame::Draw
* @brief	描画する関数
* @detail	教室が寒すぎる、上着を忘れるこれ即ち死を示す
*/
void SceneGame::Draw() {
	//ゲームオブジェクトの描画
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);

#ifdef _DEBUG
	//CCamera::Get()->Draw();
#endif

}

/**
* @fn		SceneGame::SetStage
* @brief	ステージの切り替え
* @param	(Stage)	次のステージ番号
*/
void SceneGame::SetStage(Stage NextStage) {
	//例外処理
	if (NextStage >= MAX_STAGE) {
		MessageBox(GetMainWnd(), _T("ステージ切り替え失敗"), NULL, MB_OK);
		return;
	}

	//現在のステージの終了
	if (m_CurrentScene) {
		m_CurrentScene->Uninit();
	}

	//ステージの切替
	m_CurrentScene = m_GameSceneList[NextStage];
	m_eStageState = NextStage;

}

/** 
* @fn		SceneGame::GetStage	
* @brief	ステージの情報を取得する
* @return	(Stage)	現在のステージ番号を返す
*/
Stage SceneGame::GetStage() {
	return m_eStageState;
}

/**
* @fn		SceneGame::GetInstance
* @brief	インスタンス取得
* @return	(SceneGame*)	インスタンスを返す
*/
SceneGame* SceneGame::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneGame();
	}
	return m_pInstance;
}

/**
* @fn		SceneGame::Destory
* @brief	インスタンス破棄
*/
void SceneGame::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}