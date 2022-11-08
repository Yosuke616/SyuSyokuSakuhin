/**インクルード部**/
#include "WarpComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "TextureManager.h"

#include "Object.h"
#include "ObjInfo.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "PlayerComponent.h"
#include "sceneGame.h"
#include "Camera.h"
#include "imgui.h"
#include "fade.h"

#include "SceneStage_1.h"

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
	m_bWarpFlg = false;
	m_StartPos = m_pTransform->Pos;
}

/**
* @fn		CWarp::Update
* @brief	更新処理
*/
void CWarp::Update() {
	//ワープフラグがオンになったら処理を開始する
	if (m_bWarpFlg) {
		//ステージごとにどうワープさせるかは決める

		//やりたいこと
		//画面を真っ暗にさせる(フェードアウト) 
		//オブジェクトの更新を止める
		//描画の更新を止める
		//UIの表示を消す
		//プレイヤーとカメラの位置を飛ばす(4580,-573.139771+770)
		//注視点をなくす(固定カメラにする)
		//背景を変更する
		//更新と描画の再開
		//UIの表示を再開
		//フェードインさせる

		switch (SceneGame::GetInstance()->GetStage()) {
		case STAGE_1:
			//ここはフェードアウトに変えたい
			if (GetFade() == FADE_IN) {

				ChangeCamera();
				//オブジェクトを再開させる
				for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
					obj->Use();
				}
				//最後にプレイヤーの状態を変更させる
				ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);

				//一度設定を変えたらフラグを戻して入らないようにする
				m_bWarpFlg = false;

				break;
			}

			if (GetFade() != FADE_OUT) {
				StartFadeOut(0);
				
				//プレイヤーの状態をワープしている状態にする
				Object* player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
				player->GetComponent<CPlayer>()->SetPlayerState(WARP_PLAYER);

				//動いているオブジェクトの更新をストップする
				for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
					if (obj->GetName() == WARP_NAME ||obj->GetName() == BLOCK_NAME || obj->GetName() == BLOCK_RARD_NAME || obj->GetName() == PLAYER_NAME) {
						continue;
					}
					obj->StopUpdate();
				}
				//UI描画もストップする
				for (auto&& obj : ObjectManager::GetInstance()->GetDrawList()) {
					if (obj->GetDrawOrder() == DRAW_UI) {
						obj->StopDraw();
					}
				}
				break;
			}
			break;
		case STAGE_1_RE:break;
		}
	}
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
	
}

/**
* @fn		CWarp::SetWarp
* @brief	ワープしたかどうかをセットする関数
* @param	(bool)	trueでワープを開始する
*/
void CWarp::SetWarp(bool bWarp) {
	m_bWarpFlg = bWarp;
}

/**
* @fn		CWarp::ChangeCamera
* @brief	カメラの注視点を切って、カメラとプレイヤーの座標を変更する
*/
void CWarp::ChangeCamera() {
	//プレイヤーのオブジェクトのポインタを取得する
	Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);

	//ステージ(ワープ先かどうかも判定を取る)ごとに場所を変える
	switch (SceneGame::GetInstance()->GetStage())
	{
	case STAGE_1:
		if (SceneStage_1::GetInstance()->GetWarpPoint() == 0) { 
			//0番に飛ばすぜ
			auto point = ObjectManager::GetInstance()->GetGameObject(WARP_NUMBER_0)->GetComponent<CTransform>();

			//カメラの座標をセットする
			CCamera::Get()->SetPos(point->Pos.x, obj->GetComponent<CTransform>()->Pos.y, -300.0f);
			obj->GetComponent<CTransform>()->Pos.x = point->Pos.x+20;
			obj->GetComponent<CTransform>()->Pos.y = point->Pos.y+50;
			//このオブジェクトを移動させる
			m_pTransform->Pos.x = 4400;
			m_pTransform->Pos.y = 75;
			//カメラの移動限界を変更する
			SceneStage_1::GetInstance()->SetWarpPoint(1);
		}
		else if(SceneStage_1::GetInstance()->GetWarpPoint() == 1){
			//0番に飛ばすぜ
			auto point = ObjectManager::GetInstance()->GetGameObject(WARP_NUMBER_1)->GetComponent<CTransform>();

			//カメラの座標をセットする
			CCamera::Get()->SetPos(point->Pos.x, obj->GetComponent<CTransform>()->Pos.y, -300.0f);
			obj->GetComponent<CTransform>()->Pos.x = point->Pos.x;
			obj->GetComponent<CTransform>()->Pos.y = point->Pos.y+50;
			//このオブジェクトを移動させる
			m_pTransform->Pos.x = m_StartPos.x;
			m_pTransform->Pos.y = m_StartPos.y;
			//カメラの移動限界を変更する
			SceneStage_1::GetInstance()->SetWarpPoint(0);
		}
		break;
	case STAGE_1_RE:break;

	default:break;
	}
}
