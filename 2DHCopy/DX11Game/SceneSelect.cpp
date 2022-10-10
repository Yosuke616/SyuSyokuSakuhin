/**インクルード部**/
#include "SceneSelect.h"

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "ModelManager.h"
#include "SceneManager.h"
#include "imgui.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "Draw3dComponent.h"
#include "AnimMeshComponent.h"
#include "Sound.h"

#include "Load.h"

/**構造体定義**/
StageSelect::Row StageSelect::m_StageGrid;
int StageSelect::m_nCurrentStage = 0;
int StageSelect::m_nOldStage;
StageSelect::Column StageSelect::m_Ohuda;

/**グローバル変数**/
CAnimMesh *g_CurrentBG = nullptr;

/**
* @fn		StageSelect::StageSelect
* @brief	コンストラクタは二度見る
*/
StageSelect::StageSelect() {
	StageSelect::Load();

	m_nOldStage = STAGE_A;
}

/**
* @fn		StageSelect::~StageSelect
* @brief	あっ、茂みからデストラクタが飛び出した
*/
StageSelect::~StageSelect() {

}

/**
* @fn		StageSelect::Init()
* @brief	初期化処理
*/
void StageSelect::Init() {
	//ロード画面のスレッドを作る
	Load::Begin();

	//ロードします
	//進行状況の読込
	StageSelect::Load();

	//管理クラス取得
	m_pObjectManager = ObjectManager::GetInstance();
	m_pMenuManager = MenuManager::GetInsutance();
	TextureManager* pTexManager = TextureManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();

	//カメラの初期化
	CCamera::Get()->Init();

	//テクスチャの読込
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);
	pTexManager->AddTexture(PATH_TEX_PUSH, PUSH_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_TO_TITLE, TO_TITLE_TEX_NUM);

	//モデルの読込
	pModelManager->AddModel(PATH_WALK_ENEMY, WALK_ENEMY_MODEL_NUM);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	m_nCurrentStage = m_nOldStage;

	//必要なオブジェクトの追加
	//動きそうに無いオブジェクトをここに生成する	
#pragma region ---背景
	//オブジェクトの生成
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//コンポーネントの追加
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CAnimMesh>();
	//オブジェクトの設定
	objBG->GetComponent<CTransform>()->SetPosition(0.0f,55.0f,0.0f);
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
	Draw_BG->SetSize(450, 260);
	Draw_BG->SetColor(0.0f,0.0f,1.0f);
	Draw_BG->SetAnimSplit(1,1);
	Draw_BG->SetLoop(true);
	//リストの追加
	m_pObjectManager->AddObject(objBG);

#pragma endregion

	//グローバル変数に値を入れる
	g_CurrentBG = Draw_BG;

	//メニューの作成
	m_pMenuManager->CreateSelectMenu();

	//BGM再生
	CSound::Play(SELECT_SE);

	//ロード終了
	Load::End();
}

/**
* @fn		StageSelect::Uninit
* @brief	終了処理
*/
void StageSelect::Uninit() {
	//BGM停止
	CSound::Stop(SELECT_SE);
	//オブジェクトの終了
	m_pObjectManager->Uninit();
	//メニューの終了
	m_pMenuManager->Destroy();
}

/**
* @fn		StageSelect::Update
* @brief	更新処理
*/
void StageSelect::Update() {

	//ステージごとに背景の雰囲気を変える
	TextureManager *pTex = TextureManager::GetInstance();

	switch (m_nCurrentStage) {
	case STAGE_A:
		//選ばれているステージの更新
		m_nOldStage = STAGE_A;
		g_CurrentBG->SetColor(0.0f,0.0f,1.0f);
		break;
	case STAGE_B:
		//選ばれているステージの更新
		m_nOldStage = STAGE_B;
		g_CurrentBG->SetColor(0.0f, 1.0f, 0.0f);
		break;
	case STAGE_C:
		//選ばれているステージの更新
		m_nOldStage = STAGE_C;
		g_CurrentBG->SetColor(1.0f, 0.0f, 0.0f);
		break;
	case STAGE_D:
		//選ばれているステージの更新
		m_nOldStage = STAGE_D;
		g_CurrentBG->SetColor(1.0f, 0.0f, 1.0f);
		break;
	case STAGE_E:
		//選ばれているステージの更新
		m_nOldStage = STAGE_E;
		g_CurrentBG->SetColor(1.0f, 1.0f, 1.0f);
		break;
	default: break;
	}
	//オブジェクトリストの更新
	m_pObjectManager->Update();
	//メニューの更新
	m_pMenuManager->Update();
}

/** 
* @fn		StageSelect::Draw
* @brief	描画処理
*/
void StageSelect::Draw() {
	//オブジェクトリストの描画
	SetZBuffer(true);
	m_pObjectManager->Draw();
	m_pMenuManager->Draw();
	SetZBuffer(false);
}

/**
* @fn		StageSelect::Load
* @brief	ステージクリア状況を読み込む
* @return	(bool)	本当に読み込めたのかな
*/
bool StageSelect::Load() {
	//クローバー取得数の読込
	if (FileManager::GetInstance()->Load(OHUDA_DATA_CSV) ==false){
		//エラーメッセージ
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}

	//クリア状況の読込
	if (FileManager::GetInstance()->Load(STAGE_CLEAR_CSV) == false) {
		//エラーメッセージ
		MessageBox(GetMainWnd(), _T("select_load"), NULL, MB_OK);
		return false;
	}

	//クリア状況を書き込む
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);
	m_StageGrid.resize(CSV->GetRowSize());				// 行の数を設定
	for (int y = 0; y < CSV->GetRowSize(); ++y) {
		m_StageGrid[y].resize(CSV->GetColumnSize(y));	// 列の数を設定

		for (int x = 0; x < CSV->GetColumnSize(y); x++){
			// 要素を格納する
			m_StageGrid[y][x] = CSV->GetInt(x, y);
		}
	}

	//クローバーの取得状況を移す
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	m_Ohuda.clear();
	for (int x = 0;x < OhudaCSV->GetColumnSize(0);x++) {
		//要素を格納する
		m_Ohuda.push_back(OhudaCSV->GetInt(x,0));
	}

	//無事に返す
	return true;
}

/**
* @fn		StageSelect::Save
* @brief	ステージのクリア状況を保存する
* @return	(bool)	ステージをクリアしたら書き込む
*/
bool StageSelect::Save() {
	// クリア状況の保存
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);
	if (CSV == nullptr) { 
		return false;
	}
	if (CSV->Save(STAGE_CLEAR_CSV) == false){
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	//クローバーの取得状況の保存
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	if (OhudaCSV ==nullptr) {
		return false;
	}
	if (OhudaCSV->Save(OHUDA_DATA_CSV) == false) {
		// エラーメッセージ
		MessageBox(GetMainWnd(), _T("save_data"), NULL, MB_OK);
		return false;
	}

	return true;
}

/**
* @fn		StageSelect::NewGame
* @brief	CSVファイルを初期化する
* @return	(bool)	初期化が成功したかどうか
*/
bool StageSelect::NewGame() {
	m_nOldStage = STAGE_A;

	// 書き換え
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);

	for (int nRow = 0; nRow < m_StageGrid.size(); nRow++){
		for (int nColumn = 0; nColumn < m_StageGrid[nRow].size(); nColumn++){
			CSV->SetInt(nColumn, nRow, 0);
		}
	}

	// 最初のステージ選択は可能
	CSV->SetInt(0, 0, 1);

	//クローバーの取得状況
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	for (int nColumn = 0;nColumn < m_Ohuda.size();nColumn++) {
		OhudaCSV->SetInt(nColumn,0,0);
	}

	StageSelect::Save();

	return true;
}

/**
* @fn		StageSelect::SaveClearinfo
* @brief	クリア状況の保存
* @param	(int)	何番目のステージをクリアしたか	
* @return	(bool)	正しく保存できたかどうか
*/
bool StageSelect::SaveClearInfo(int nStage,bool bOhuda) {
	//お札のcsvを取得する
	ResourceCSV* OhudaCSV = (ResourceCSV*)FileManager::GetInstance()->Get(OHUDA_DATA_CSV);
	
	//お札を書き換える
	OhudaCSV->SetInt(nStage,0,bOhuda);

	// クリア状況を書き換える
	ResourceCSV* CSV = (ResourceCSV*)FileManager::GetInstance()->Get(STAGE_CLEAR_CSV);
	// 次のステージのフラグへ
	int x = nStage + 1;

	CSV->SetInt(x, 0, 1);	// (列, 行, 格納する数字)

	// csvに保存
	StageSelect::Save();

	return true;
}

/**
* @fn		StageSelect::GetSelectAble
* @brief	ステージが選択可能かどうかを調べる
* @param	(int)	何番目のステージが選択可能か調べる
* @return	(bool)	選択可能かどうかを調べる
*/
bool StageSelect::GetSelectAble(int nStage) {

	int n = nStage + 1;

	if (m_StageGrid[0][n] == 0)
	{
		if (SceneManager::Instance()->GetScene() == SCENE_SELECT) {
		
		}
		return false;
	}
	else {
		return true; 
	}


}

/**
* @fn		StageSelect::SetCurrentStage
* @brief	現在選ばれているステージ番号の設定
* @param	(bool)	trueだったら+ falseなら-	
*/
void StageSelect::SetCurrentStage(bool num) {
	if (num) {
		if (m_nCurrentStage >= STAGE_E) {
		
		}
		else {
			m_nCurrentStage++;		
		}
	}
	else {
		if (m_nCurrentStage <= STAGE_A) {
		
		}
		else {
			m_nCurrentStage--;
		}
	}
}

/**
* @fn		StageSelect::SaveOhudaInfo
* @brief	お札の取得情報を取得する
* @param	(int)	どのステージかの情報を得る
* @return	(bool)	しっかりと読み込めたかどうか
*/
bool StageSelect::SaveOhudaInfo(int nStage) {
	m_Ohuda[nStage] = 1;

	return true;
}

/**
* @fn		StageSelect::GetOhuda
* @brief	お札の情報を取得する
* @return	(StageSelect::Column)	何列目かどうか
*/
StageSelect::Column StageSelect::GetOhuda() {
	return m_Ohuda;
}