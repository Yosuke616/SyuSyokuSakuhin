/**インクルード部**/
#include "WarpComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "InputManager.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "imgui.h"

/**
* @fn		CWarp::CWarp
* @brief	コンストラクタエンジン
*/
CWarp::CWarp() 
	:m_pTransform(nullptr),m_pCollider(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CWarp::CWarp
* @brief	デストラクタみたいでテンション上がるなぁ
*/
CWarp::~CWarp() {

}

/**
* @fn		CWarp::Start	
* @brief	開始処理
*/
void CWarp::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();


}

/**
* @fn		CWarp::Update
* @brief	更新処理
*/
void CWarp::Update() {

}

/**
* @fn		CWarp::Draw
* @brief	描画処理
*/
void CWarp::Draw() {

}

/**
* @fn		CWarp::OnCliisionEnter
* @brief	当たり判定の処理
* @param	(Object*)	当たった相手やで
*/
void CWarp::OnCollisionEnter(Object* pObject) {
	if (CollPlayer(pObject)) {
		//シーンごとにどうワープさせるかを考える
		switch (SceneGame::GetInstance()->GetStage())
		{
		case STAGE_1:
			if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
				int i = 1 + 10;
			}
			break;
		case STAGE_1_RE:break;
		default:break;
		}
	}
}

/**
* @fn		CWarp::CollPlayer
* @brief	プレイヤーとの当たり判定の処理
* @param	(Object*)	プレイヤーと当たった時かどうか
* @return	(bool)		当たったかどうか
*/
bool CWarp::CollPlayer(Object* pObject) {
	if (pObject->GetName() == PLAYER_NAME) {
		//プレイヤーの情報を取得
		auto Player = pObject->GetComponent<CTransform>();
		auto PlayerColl = pObject->GetComponent<CCollider>();
		auto PlayerPos = PlayerColl->GetCenterPos();
		auto PlayerSize = PlayerColl->GetColliderSize();
		auto PlayerOffSet = PlayerColl->GetOffSet();

		//プレイヤーの半分の大きさを取得
		XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2, PlayerSize.y / 2);

		XMFLOAT2 WarpHalfSize = XMFLOAT2(m_pCollider->GetColliderSize().x / 2.0f, m_pCollider->GetColliderSize().y / 2.0f);

		if (m_pCollider->GetCenterPos().x - WarpHalfSize.x + m_pTransform->Vel.x < PlayerPos.x + PlayerHalfSize.x + Player->Vel.x &&
			PlayerPos.x - PlayerHalfSize.x + Player->Vel.x < m_pCollider->GetCenterPos().x + WarpHalfSize.x + m_pTransform->Vel.x) {

			if (m_pCollider->GetCenterPos().y - WarpHalfSize.y + m_pTransform->Vel.y < PlayerPos.y + PlayerHalfSize.y + Player->Vel.y &&
				PlayerPos.y - PlayerHalfSize.y + Player->Vel.y < m_pCollider->GetCenterPos().y + WarpHalfSize.y + m_pTransform->Vel.y) {
				return true;
			}
		}
	}
	return false;
}
