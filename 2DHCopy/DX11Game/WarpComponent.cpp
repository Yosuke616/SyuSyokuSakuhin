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
			//今回は仮に真っ黒で覆う
			CreateBlak();

			//全てのオブジェクトの更新をストップする
			for (auto&& obj :ObjectManager::GetInstance()->GetUpdateList()) {
				if (obj->GetName() == WARP_NAME) {
					continue;
				}
				obj->StopUpdate();
			}
			//UI描画もストップする
			for (auto&& obj:ObjectManager::GetInstance()->GetDrawList()) {
				if (obj->GetDrawOrder() == DRAW_UI|| obj->GetDrawOrder() == DRAW_MODEL) {
					obj->StopDraw();
				}
			}

			ChangeCamera();

			//一度設定を変えたらフラグを戻して入らないようにする
			//m_bWarpFlg;
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
* @fn		CWarp::CreateBlack
* @brief	黒背景を作成する後で消す
*/
void CWarp::CreateBlak() {
	Object* black = new Object("black_outoooo", UPDATE_DEBUG, DRAW_DEBUG);
	//コンポーネントの追加
	auto trans = black->AddComponent<CTransform>();
	auto draw = black->AddComponent<CDraw2D>();
	//オブジェクトの設定
	draw->SetColor(0.0f, 0.0f, 0.0f);
	draw->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//リストに追加
	ObjectManager::GetInstance()->AddObject(black);
}

/**
* @fn		CWarp::ChangeCamera
* @brief	カメラの注視点を切って、カメラとプレイヤーの座標を変更する
*/
void CWarp::ChangeCamera() {
	//プレイヤーのオブジェクトのポインタを取得する
	Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);

	obj->GetComponent<CTransform>()->Pos.x = 4580;

	//カメラの注視点をオフにする
	//
	CCamera::Get()->SetAxisX(nullptr);

	//ワープしている先かどうかのフラグを持っておいた方がいいかも
	//ステージにごとにワープ先にいるかどうかのフラグを持たせて
	//カメラのソースでリミットを変化させる

}
