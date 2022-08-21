/**インクルード部**/
#include "SceneSelect.h"

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "ModelManager.h"
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

/**構造体定義**/
StageSelect::Row StageSelect::m_StageGrid;

/**
* @fn		StageSelect::StageSelect
* @brief	コンストラクタは二度見る
*/
StageSelect::StageSelect() {
	//StageSelect::Load();
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
	//ロードします
	//進行状況の読込
	//StageSelect::Load();

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

	//モデルの読込
	pModelManager->AddModel(PATH_ROSALINA_X, ROSALINA_MODEL_X);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//必要なオブジェクトの追加
	//動きそうに無いオブジェクトをここに生成する	
#pragma region ---背景
	//オブジェクトの生成
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//コンポーネントの追加
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CAnimMesh>();
	//オブジェクトの設定
	objBG->GetComponent<CTransform>()->SetPosition(0.0f,0.0f,0.0f);
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
	Draw_BG->SetSize(450, 260);
	Draw_BG->SetColor(0.0f,0.0f,1.0f);
	Draw_BG->SetAnimSplit(1,1);
	Draw_BG->SetLoop(true);
	//リストの追加
	m_pObjectManager->AddObject(objBG);

#pragma endregion
	//メニューの作成
	m_pMenuManager->CreateSelectMenu();
}

/**
* @fn		StageSelect::Uninit
* @brief	終了処理
*/
void StageSelect::Uninit() {
	//オブジェクトの終了
	m_pObjectManager->Uninit();
	//メニューの終了
	m_pMenuManager->DeleteMenu();
}

/**
* @fn		StageSelect::Update
* @brief	更新処理
*/
void StageSelect::Update(){
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
* @fn		StageSelect::GetSelectAble
* @brief	ステージが選択可能かどうかを見る
* @param	(int)	番号で識別する
* @return	(bool)	選択できるかどうかの結果を返す
*/
//bool StageSelect::GetSelectAble(int stage_num) {
//	return 0;
//}



