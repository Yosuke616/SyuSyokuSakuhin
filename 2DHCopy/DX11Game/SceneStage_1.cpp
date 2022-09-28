/**インクルード部**/
#include "SceneStage_1.h"
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
SceneStage_1* SceneStage_1::m_pInstance = nullptr;

/**
* @fn		SceneStage_1::GetInstance
* @brief	インスタンス取得
* @return	(SceneStage_1*)	ステージ1の情報が入ったポインタ
*/
SceneStage_1* SceneStage_1::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneStage_1();
	}
	return m_pInstance;
}

/**
* @fn		SceneStage_1::Destroy
* @brief	インスタンスの破棄
*/
void SceneStage_1::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		SceneStage_1::SceneStage_1
* @brief	コンストラクタは嘆いている
*/
SceneStage_1::SceneStage_1() {

}

/**	
* @fn		SceneStage_1::~SceneStage_1
* @brief	デストラクタ、お前の父は私だ
*/
SceneStage_1::~SceneStage_1(){

}

/**
* @fn		SceneStage_1
* @brief	初期化処理
*/
void SceneStage_1::Init() {
	//管理クラスのインスタンス取得
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

	for (int i = 0;i < 2;i++) {
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

	//イベントフラグをオフにする
	m_bEventFlg = false;

	//BGM再生
	CSound::Play(TITLE_BGM);

}

/**
* @fn		SceneStage_1
* @brief	終了処理
*/
void SceneStage_1::Uninit() {
	CSound::Stop(TITLE_BGM);
}

/**
* @fn		SceneStage_1::Update
* @brief	更新処理
*/
void SceneStage_1::Update() {
	//リストのオブジェクトとの当たり判定の計算
	for (auto&& obj : m_EventList) {
		//オブジェクトを更新するかどうかを見る
		for (auto&& comp : obj->GetComponentList()) {
			if (comp->m_bUpdateFlag) {
				//画面外かどうか調べるやつはスキップ
				if (comp == obj->GetComponent<COutOfRange>()) {
					continue;
				}
				//当たり判定用の関数を呼ぶ
				if (CollPlayer(obj)) {
					//やりたいこと
					//プレイヤーをミス状態にする
					//ステージの読込先を変える
					m_bEventFlg = true;
					SceneGame::GetInstance()->SetStage(STAGE_1_RE);
					auto player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CPlayer>();
					player->SetPlayerState(MISS_PLAYER);
				}
				break;
			}
		}
	}
}

/**
* @fn		SceneStage_1::Draw
* @brief	描画処理
*/
void SceneStage_1::Draw() {

}

/**
* @fn		SceneStage_1::SetBaseInfo
* @brief	イベントの用当たり判定のポインタを保存して
			リストに保存する。
* @detail	ステージマネージャーの初期化が終わった段階で呼ぶ
* @param	(list<Object*>)	リストを貰って必要なものだけまた別のリストに入れる	
*/
void SceneStage_1::SetBaseInfo(std::list<Object*> list) {
	for (auto&& obj : list) {
		if (obj->GetName() == STAGE_1_MISS_EVENT) {
			m_EventList.push_back(obj);
		}
	}
}

/**
* @fn		SceneStage_1::DeleteList
* @brief	イベントリストに中身があった場合削除する
*/
void SceneStage_1::DeleteList() {
	if (m_EventList.size()) {
		m_EventList.clear();
	}
}

/**
* @fn		SceneStage_1::CollPlayer
* @brief	プレイヤーとの当たり判定用の関数
* @param	(Object*)	アクティブになっているオブジェクトのポインタ
* @return	(bool)		trueで当たりました　falseで当たってないよ
*/
bool SceneStage_1::CollPlayer(Object* obj) {
	//プレイヤーとリスト(イベント当たり判定)にあるもの
	auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CCollider>();
	auto TransPlayer = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffSet = Player->GetOffSet();
	//半分の大きさの計算
	XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2.0f, PlayerSize.y / 2.0f);
	
	auto Coll = obj->GetComponent<CCollider>();
	auto Trans = obj->GetComponent<CTransform>();
	//半分の大きさの計算
	XMFLOAT2 CollHalfSize = XMFLOAT2(Coll->GetColliderSize().x/2.0f,Coll->GetColliderSize().y/2.0f);

	if (Coll->GetCenterPos().x - CollHalfSize.x + Trans->Vel.x < Player->GetCenterPos().x + PlayerHalfSize.x + TransPlayer->Vel.x &&
		Player->GetCenterPos().x - PlayerHalfSize.x + TransPlayer->Vel.x < Coll->GetCenterPos().x + CollHalfSize.x + Trans->Vel.x) {

		if (Coll->GetCenterPos().y - CollHalfSize.y + Trans->Vel.y < Player->GetCenterPos().y + PlayerHalfSize.y + TransPlayer->Vel.y &&
			Player->GetCenterPos().y - PlayerHalfSize.y + TransPlayer->Vel.y < Coll->GetCenterPos().y + CollHalfSize.y + Trans->Vel.y) {
			return true;
		}
	}

	return false;
}
