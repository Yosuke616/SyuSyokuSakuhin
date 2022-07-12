//インクルード部
#include "sceneGame.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TexScrollComponent.h"
#include "Draw3dComponent.h"

//静的メンバ変数
SceneGame* SceneGame::m_pInstance = nullptr;

/**
* @fn		SceneGame::SceneGame
* @brief	コンストラクタ
* @detail	追加したいステージ等はここで追加していく
*/
SceneGame::SceneGame()
	:m_bPauseMode(false), m_bEditMode(false) {

	//ここでステージをリストに追加していく
	//ex).m_GameSceneList.push_back(new ??????());

	//最初のステージの設定

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
	ModelManager* pModelManager = ModelManager::GettInstance();
	pModelManager->AddModel(PATH_MODEL_KARIKARI, KARIKARIMODEL);

	//テクスチャの読込
	TextureManager* pTexManager = TextureManager::GetInstance();


	//モデル読込
	Object* KARI3D = new Object(BLOCK_KARI, UPDATE_PLAYER, DRAW_PLAYER);
	auto BLO = KARI3D->AddComponent<CTransform>();
	auto DRAW_BLO = KARI3D->AddComponent<CDraw3D>();
	DRAW_BLO->SetModel(pModelManager->GetModel(KARIKARIMODEL));

	BLO->SetScale(50.0f, 50.0f, 50.0f);
	BLO->SetVelocity(0.0f, 0.0f, 0.0f);
	BLO->SetPosition(0.0f, 0.0f, 400.0f);
	BLO->SetRotate(0.0f,180.0f,0.0f);
	m_pObjectManager->AddObject(KARI3D);
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

#ifdef _DEBUG
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