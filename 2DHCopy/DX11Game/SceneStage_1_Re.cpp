/**インクルード部**/
#include "SceneStage_1_Re.h"
#include "imgui.h"

#include "Object.h"
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
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Sound.h"

/**静的メンバ変数**/
SceneStage_1_Re* SceneStage_1_Re::m_pInstance = nullptr;

/**
* @fn		SceneStage_1_Re::GetInstance
* @brief	インスタンスを生成する
* @return	(SceneStage_1_Re*) ステージ1(リローデッド)の情報が入っているポインタ
*/
SceneStage_1_Re* SceneStage_1_Re::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneStage_1_Re();
	}
	return m_pInstance;
}

/**
* @fn		SceneStage_1_Re::Destroy
* @brief	インスタンスの破棄
*/
void SceneStage_1_Re::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		SceneStage_1_Re::SceneStage_1_Re
* @brief	コンストラクタにより脳が破壊された
*/
SceneStage_1_Re::SceneStage_1_Re() {

}

/**
* @fn		SceneStage_1_Re::~SceneStage_1_Re
* @brief	デストラクタにより脳が再生した
*/
SceneStage_1_Re::~SceneStage_1_Re() {

}

/**
* @fn		SceneStage_1_Re::Init
* @brief	初期化処理
*/
void SceneStage_1_Re::Init() {
	//管理クラスのインスタンス取得
	m_pObjectManager = ObjectManager::GetInstance();

	//テクスチャ読込
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_STAGE_1_RE_BG, STAGE_1_RE_BG_TEX_NUM);

	//モデル読込
	ModelManager* pModelManager = ModelManager::GetInstance();
	//ブロックのモデル

	//プレイヤーの状態の取得
	Object* pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	//背景の描画
	for (int i = 0;i < 2;i++) {
		//オブジェクトの生成
		Object* objBG = new Object("Back",UPDATE_BG,DRAW_BG);
		//コンポーネントの追加
		auto transBG = objBG->AddComponent<CTransform>();
		auto drawBG = objBG->AddComponent<CDraw2D>();
		auto scrollBG = objBG->AddComponent<CTexScroll>();
		//オブジェクトの追加
		transBG->SetPosition(-720 * 0.5f + i * 720, 0); drawBG->SetTexture(pTexManager->GetTexture(STAGE_1_RE_BG_TEX_NUM));
		drawBG->SetSize(720, 720);
		scrollBG->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));
		scrollBG->SetScrollValueX(0.0016f);
		//リストに追加
		m_pObjectManager->AddObject(objBG);
	}

	//BGM再生
	CSound::Play(TITLE_BGM);

}

/**
* @fn		SceneStage_1_Re::Uninit
* @brief	終了処理
*/
void SceneStage_1_Re::Uninit() {
	CSound::Stop(TITLE_BGM);
}

/**
* @fn		SceneStage_1_Re::Update
* @brief	更新処理
*/
void SceneStage_1_Re::Update() {

}

/**
* @fn		SceneStage_1_Re::Draw
* @brief	描画処理
*/
void SceneStage_1_Re::Draw() {

}
