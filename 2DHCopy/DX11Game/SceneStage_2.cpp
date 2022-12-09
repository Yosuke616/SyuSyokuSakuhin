/**インクルード部**/
#include "SceneStage_2.h"
#include "imgui.h"

#include "ObjInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "Draw3dComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "TexScrollComponent.h"
#include "OutOfRange.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Sound.h"

/**静的メンバ変数**/
SceneStage_2* SceneStage_2::m_pInstance = nullptr;

/**
* @fn		Scenestage_2::GetInstance
* @brief	インスタンスの取得と生成
* @return	(SceneStage_2*)	ステージ2の情報が入ったポインタ
*/
SceneStage_2* SceneStage_2::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneStage_2();
	}
	return m_pInstance;
}

/**
* @fn		SceneStage_2::Destroy
* @brief	インスタンスの破棄
*/
void SceneStage_2::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		SceneStage_2::SceneStage_2
* @brief	別にあなたの為にコンストラクタしてる訳じゃないんだから状態
*/
SceneStage_2::SceneStage_2() {
	
}

/**
* @fn		SceneStage_2::~SceneStage_2
* @brief	ツンデストラクタ
*/
SceneStage_2::~SceneStage_2() {

}

/**
* @fn		SceneStage_2::Init
* @brief	初期化処理
*/
void SceneStage_2::Init() {
	//管理クラスの取得
	m_pObjectManager = ObjectManager::GetInstance();

	//テクスチャ読込
	TextureManager* pTexManager = TextureManager::GetInstance();
	/*プレイヤーのテクスチャ*/
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);

	//モデル読込
	ModelManager* pModelManager = ModelManager::GetInstance();
	//ブロックとロゼッタ
	pModelManager->AddModel(PATH_WALK_ENEMY, WALK_ENEMY_MODEL_NUM);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//プレイヤー状態の取得
	m_pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	for (int i = 0; i < 2; i++) {
		//背景の描画
		Object* objBG = new Object("Back", UPDATE_BG, DRAW_BG);
		//コンポーネントの追加
		auto transBG = objBG->AddComponent<CTransform>();
		auto drawBG = objBG->AddComponent<CDraw2D>();
		auto scrollBG = objBG->AddComponent<CTexScroll>();
		//オブジェクトの追加
		transBG->SetPosition(-720 * 0.5f + i * 720, 0);
		drawBG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
		drawBG->SetSize(720, 720);
		scrollBG->SetAxizX(&(m_pPlayer->GetComponent<CTransform>()->Pos.x));
		scrollBG->SetScrollValueX(0.0016f);
		//リストに追加
		m_pObjectManager->AddObject(objBG);
	}
	//BGM再生
	CSound::Play(TITLE_BGM);

}

/**
* @fn		SceneStage_2::Uninit
* @brief	終了処理
*/
void SceneStage_2::Uninit() {
	CSound::Stop(TITLE_BGM);
}

/**
* @fn		SceneStage_2
* @brief	更新処理
*/
void SceneStage_2::Update() {

}

/**
* @fn		SceneStage_2::Draw
* @brief	描画処理
*/
void SceneStage_2::Draw() {

}