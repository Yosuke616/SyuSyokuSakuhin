/**インクルード部**/
#include "SceneStage_1_Re.h"
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
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Sound.h"
#include  "OutOfRange.h"
#include "MosaicRollComponent.h"

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
SceneStage_1_Re::SceneStage_1_Re()
	:m_nBossHP(5){

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
		transBG->SetPosition(-720 * 0.5f + i * 720, 0); 
		drawBG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
		drawBG->SetSize(720, 720);
		scrollBG->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));
		scrollBG->SetScrollValueX(0.0016f);
		//リストに追加
		m_pObjectManager->AddObject(objBG);
	}

	//BGM再生
	CSound::Play(TITLE_BGM);

	//裏表を決める
	SceneGame::GetInstance()->SetLaL(true);

	//ボスのHPの設定
	m_nBossHP = 5;
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
	//リストの中身が更新されているかを判別する
	for (auto&& obj : m_EventList) {
		for (auto&& comp : obj->GetComponentList()) {
			//更新されているかを判別する
			if (comp->m_bUpdateFlag) {
				//画面外にどうか調べる奴はスキップ
				if (comp == obj->GetComponent<COutOfRange>()) {
					continue;
				}
				if (obj->GetName() == STAGE_RE_1_CHANGE_COLL) {
					//当たり判定用の関数を呼ぶ
					if (CollPlayer(obj)) {
						ChangeObject();
					}
				}
				else if (obj->GetName() == CAMERA_MOVE_NAME) {
					if (CollPlayer(obj)) {
						CCamera::Get()->SetCameraMove(true);
					}
				}
			}
		}
	}

	//ボスが消えたかどうかを判別する
	if (m_nBossHP <= 0) {
		//カメラの注視点を元に戻す
		CCamera::Get()->SetCameraMove(false);
	}
}

/**
* @fn		SceneStage_1_Re::Draw
* @brief	描画処理
*/
void SceneStage_1_Re::Draw() {

}

/**
* @fn		SceneStage_1_Re::ChangeObject
* @brief	オブジェクトを任意のタイミングで変更する
*/
void SceneStage_1_Re::ChangeObject() {
	//モザイクを流す
	MosaicRoll::Begin();

	//全てのオブジェクトの更新を止める
	for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
		obj->StopUpdate();
	}
	//更新リストからUI更新以外の物を全て色を変える(仮処置)
	for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
		if (obj->GetName() == "Back") {
			obj->GetComponent<CDraw2D>()->SetTexture(TextureManager::GetInstance()->GetTexture(STAGE_1_RE_BG_TEX_NUM));
		}
		else if(obj->GetName() == BLOCK_NAME){
			obj->GetComponent<CDraw3D>()->SetModel(ModelManager::GetInstance()->GetModel(GRASS_BLOCK_RE_NUM));
		}
		else if (obj->GetName() == BLOCK_RARD_NAME) {
			obj->GetComponent<CDraw3D>()->SetModel(ModelManager::GetInstance()->GetModel(RARD_BLOCK_RE_NUM));
		}
	}


	//更新再開
	//オブジェクトを全て再開する
	for (auto&& obj : m_pObjectManager->GetUpdateList()) {
		obj->Use();
	}

	//変化オブジェクトの削除
	for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
		if (obj->GetName() == STAGE_RE_1_CHANGE_COLL) {
			obj->NoUse();
		}
	}

	//裏表を変える
	SceneGame::GetInstance()->SetLaL(false);

	//モザイク終了
	MosaicRoll::End();
}

/**
* @fn		SceneStage_1_Re::SetBaseInfo
* @brief	特殊当たり判定をリストに保存するための変数
* @param	(std::list<Object*>)	ゲームのリスト
*/
void SceneStage_1_Re::SetBaseInfo(std::list<Object*> objList) {
	for (auto&& obj : objList) {
		if (obj->GetName() == STAGE_RE_1_CHANGE_COLL || obj->GetName() == CAMERA_MOVE_NAME) {
			m_EventList.push_back(obj);
		}
	}
}

/**
* @fn		SceneStage_1_Re::DeleteList
* @brief	イベントリストに中身があった場合削除する
*/
void SceneStage_1_Re::DeleteList() {
	if (m_EventList.size()) {
		m_EventList.clear();
	}
}

/**
* @fn		SceneStage_1_Re::CollPlayer
* @brief	プレイヤーとの当たり判定を取る関数
* @param	(Object*)	アクティブになっているオブジェクトのポインタ	
* @return	(bool)	trueで当たりました falseであたってないよ
*/
bool SceneStage_1_Re::CollPlayer(Object* obj){
	//プレイヤーの情報
	auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CCollider>();
	auto TransPlayer = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffSet = Player->GetOffSet(); Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CCollider>();

	//半分の大きさの計算
	XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2.0f, PlayerSize.y / 2.0f);

	auto Coll = obj->GetComponent<CCollider>();
	auto Trans = obj->GetComponent<CTransform>();

	//半分の大きさの計算
	XMFLOAT2 CollHalfSize = XMFLOAT2(Coll->GetColliderSize().x / 2.0f, Coll->GetColliderSize().y / 2.0f);

	if (Coll->GetCenterPos().x - CollHalfSize.x + Trans->Vel.x < Player->GetCenterPos().x + PlayerHalfSize.x + TransPlayer->Vel.x &&
		Player->GetCenterPos().x - PlayerHalfSize.x + TransPlayer->Vel.x < Coll->GetCenterPos().x + CollHalfSize.x + Trans->Vel.x) {

		if (Coll->GetCenterPos().y - CollHalfSize.y + Trans->Vel.y < Player->GetCenterPos().y + PlayerHalfSize.y + TransPlayer->Vel.y &&
			Player->GetCenterPos().y - PlayerHalfSize.y + TransPlayer->Vel.y < Coll->GetCenterPos().y + CollHalfSize.y + Trans->Vel.y) {
			return true;
		}
	}
	return false;
}

/**
* @fn		SceneStage_1_Re::GetOhuda
* @brief	お札を取得したかのフラグ
* @return	(bool)	お札を取得しているか
*/
bool SceneStage_1_Re::GetOhuda() {
	return m_bOhuda;
}

/**
* @fn		SceneStage_1_Re::SetOhuda
* @brief	お札を取得できているかをセットする関数
* @param	(bool)	お札を取得できているかどうかの関数
*/
void SceneStage_1_Re::SetOhuda(bool bOhuda) {
	m_bOhuda = bOhuda;
}

/**
* @fn		SceneStage_1_Re::SetBossHP
* @brief	ボスのHPを減らしていく関数
*/
void SceneStage_1_Re::SetBossHP() {
	m_nBossHP--;
}

/**
* @fn		SceneStage_1_Re::GetBossHP
* @brief	ボスのHPを取得するための関数
* @return	(int)	ボスのHP
*/
int SceneStage_1_Re::GetBossHP() {
	return m_nBossHP;
}