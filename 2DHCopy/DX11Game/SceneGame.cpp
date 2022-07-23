//インクルード部
#include "sceneGame.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
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
	pModelManager->AddModel(PATH_MODEL_BLOCKKARI, BLOCK_KARIKARI);

	//テクスチャの読込
	TextureManager* pTexManager = TextureManager::GetInstance();

	//for (int i = 0;i < 10;i++) {
		//モデル読込
		Object* KARI3D = new Object(BLOCK_KARI, UPDATE_FIELD, DRAW_FIELD);
		auto BLO = KARI3D->AddComponent<CTransform>();
		auto DRAW_BLO = KARI3D->AddComponent<CDraw3D>();
		auto COLL_BLO = KARI3D->AddComponent<CCollider>();
		KARI3D->AddComponent<CSeeColl>();
		DRAW_BLO->SetModel(pModelManager->GetModel(BLOCK_KARIKARI));

		BLO->SetScale(10.0f, 10.0f, 10.0f);
		BLO->SetVelocity(0.0f, 0.0f, 0.0f);
		BLO->SetPosition(35, 0.0f, 0.0f);
		BLO->SetRotate(0.0f, 180.0f, 0.0f);
		COLL_BLO->SetCollisionSize(70.0f, 70.0f, 70.0f);
		m_pObjectManager->AddObject(KARI3D);
	//}


	//仮
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);

	//オブジェクトの作成
	//仮
	Object* Box = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	auto CollBox = Box->AddComponent<CCollider>();
	Box->AddComponent<CPlayer>();
	//Box->AddComponent<CSeeColl>();
	//設定仮
	TransBox->SetPosition(35.0f, 50.0f);
	CollBox->SetCollisionSize(30.0f, 30.0f, 100.0f);
	CollBox->SetOffset(7.4f, 0.0f);

	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	DrawBox->SetSize(30.0f, 30.0f);
	//DrawBox->SetAnimSplit(3,3);
	//DrawBox->SetSwapFrame(10);
	DrawBox->SetLoop(true);

	m_pObjectManager->AddObject(Box);

	/*CollBox->SetCollisionSize(350.0f,320.0f,100.0f);
	CollBox->SetOffset(10.0f,75.0f);
	CollBox->SetCollisionSize(10.0f, 60.0f, 100.0f);
	CollBox->SetOffset(7.4f, 0.0f);*/

	//Object* Box2 = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//auto TransBox2 = Box2->AddComponent<CTransform>();
	//auto DrawBox2 = Box2->AddComponent<CDraw3D>();
	//auto CollBox2 = Box2->AddComponent<CCollider>();
	//auto PlayBox2 = Box2->AddComponent<CPlayer>();
	//Box2->AddComponent<CSeeColl>();
	//DrawBox2->SetModel(pModelManager->GetModel(BLOCK_KARIKARI));

	////設定仮
	//TransBox2->SetScale(10.0f,10.0f,10.0f);
	//TransBox2->SetPosition(50.0f, 50.0f);
	//TransBox2->SetVelocity(0.0f, 0.0f, 0.0f);
	//TransBox2->SetRotate(0.0f, 180.0f, 0.0f);
	//CollBox2->SetCollisionSize(50.0f, 50.0f, 50.0f);

	//m_pObjectManager->AddObject(Box2);
	
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