/**インクルード部**/
#include "ItemComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "imgui.h"

#include <stdlib.h>

/**定数定義**/
/** @brief 勾玉が上下する速さ*/
#define MAGATAMA_SPEED	(0.05f)
 
/**
* @fn		CItem::CItem
* @brief	コンストラクタオンライン
*/
CItem::CItem()
	:m_pTransform(nullptr),m_pDraw(nullptr),m_pCollider(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CItem::~CItem
* @brief	週一デストラクタ
*/
CItem::~CItem() {

}

/**
* @fn		CItem::Start
* @brief	開始処理
*/
void CItem::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw3D>();
	m_pCollider  = Parent->GetComponent<CCollider>();
	m_fUpDown = 0.0f;
	m_fRotTime = 90.0f;
	m_bUpDown = false;
	m_bPlayer = false;
	m_bSetUp  = false;
	m_bFirstBlad = false;
}

/**
* @fn		CItem::Update
* @brief	アイテムオブジェクトの更新
* @detail	選択されたアイテムで内容は変化する
*/
void CItem::Update() {
	//選択されたアイテムで内容を変える
	switch (m_eItem) {
	case ITEM_KOBAN:
		//回転させる(Y軸回転)
		m_pTransform->Rotate.y += 5.0f;
		break;
	case ITEM_MAGA:
		//プレイヤーに取得されたかどうかで処理を変える
		if (m_bPlayer) {
			//回転させる(Z軸回転)
			m_pTransform->Rotate.z += 5.0f;

			//当たり判定を削除する
			m_pCollider->m_bUpdateFlag = false;

			//プレイヤーの上についているかついていないか
			if (m_bSetUp) {
				Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
				XMFLOAT3 player_pos = obj->GetComponent<CTransform>()->Pos;

				m_fRotTime += 0.05f;
				if (m_fRotTime > 360) {
					m_fRotTime = 0;
				}

				m_pTransform->Pos.x = player_pos.x + (cos(m_fRotTime) * 75);
				m_pTransform->Pos.y = player_pos.y + (sin(m_fRotTime) * 75);
			}
			else {
				if (!m_bFirstBlad) {
					//右上に力をかける
					m_pTransform->Vel.x -= 2.0f;
					m_pTransform->Vel.y -= 2.0f;
					m_bFirstBlad = true;
				}

				//プレイヤーの頭上に持ってくる
				Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
				XMFLOAT3 player_pos = obj->GetComponent<CTransform>()->Pos;
				player_pos.y += 75.0f;

				//プレイヤーの勾玉とのベクトルを取得する
				XMFLOAT2 direction = XMFLOAT2(player_pos.x - m_pTransform->Pos.x, player_pos.y - m_pTransform->Pos.y);
				
				//XMFLOAT2をXMVECTOR型にを変換
				XMVECTOR xmvNorm = XMLoadFloat2(&direction);
				//ベクトルの正規化
				xmvNorm = XMVector2Normalize(xmvNorm);
				//正規化されたXMVECTOR型をXMFLOAT2型に入れる箱の用意
				XMFLOAT2 vNorm;
				//XMVECTORをXMFLOAT2に変換
				XMStoreFloat2(&vNorm,xmvNorm);

				//速度の制御----------------------------------
				m_pTransform->Vel.x += vNorm.x * 0.5f * 2;
				if (m_pTransform->Vel.x > 2.5f) {
					m_pTransform->Vel.x = 2.5f;
				}
				m_pTransform->Vel.y += vNorm.y * 0.5f * 2;
				if (m_pTransform->Vel.y > 2.5f) {
					m_pTransform->Vel.y = 2.5f;
				}
				//--------------------------------------------

				//x,yそれぞれの座標が被ったら速度を0にする
				float X = (player_pos.x - m_pTransform->Pos.x);
				float Y = (player_pos.y - m_pTransform->Pos.y);

				//絶対値の設定する
				X = abs(X);
				Y = abs(Y);

				if (X < 10.0f) {
					m_pTransform->Vel.x = 0.0f;
				}
				if (Y < 10.0f) {
					m_pTransform->Vel.y = 0.0f;
				}

				//プレイヤーの上の座標と同じになったら抜ける
				if (X < 10.0f && Y < 10.0f){
					m_bSetUp = true;
				}

			}
		}
		else {
			//上下に移動させる
			//回転させる(Z軸回転)
			m_pTransform->Rotate.z += 1.0f;

			if (m_bUpDown) {
				//上に移動させる
				m_pTransform->Vel.y = MAGATAMA_SPEED;
				m_fUpDown += MAGATAMA_SPEED;

				//ある程度上がったら下に下げる
				if (m_fUpDown > 2.0f) {
					m_bUpDown = false;
				}
			}
			else {
				//下に移動させる
				m_pTransform->Vel.y = -MAGATAMA_SPEED;
				m_fUpDown -= MAGATAMA_SPEED;

				//ある程度下がったら上に上げる
				if (m_fUpDown < -2.0f) {
					m_bUpDown = true;
				}
			}
		}

		break;
	case ITEM_OHUDA:
		//小判の回転と勾玉のフワフワを出す
		//回転させる(Y軸回転)
		m_pTransform->Rotate.y -= 5.0f;
		if (m_bUpDown) {
			//上に移動させる
			m_pTransform->Vel.y = MAGATAMA_SPEED;
			m_fUpDown += MAGATAMA_SPEED;

			//ある程度上がったら下に下げる
			if (m_fUpDown > 2.0f) {
				m_bUpDown = false;
			}
		}
		else {
			//下に移動させる
			m_pTransform->Vel.y = -MAGATAMA_SPEED;
			m_fUpDown -= MAGATAMA_SPEED;

			//ある程度下がったら上に上げる
			if (m_fUpDown < -2.0f) {
				m_bUpDown = true;
			}
		}
		break;
	}
}

/** 
* @fn		CItem::Draw
* @brief	アイテムオブジェクトの描画
*/
void CItem::Draw() {
	
}

/**
* @fn		CItem::OnCollisionEnter
* @brief	当たり判定の処理
* @param	(Object*)	当たった相手やで
*/
void CItem::OnCollisionEnter(Object* obj) {

}

/**
* @fn		CItem::SetItem
* @brief	アイテムの種類を決める
* @param	(ITEM_TYPE)	作るアイテム
*/
void CItem::SetItem(ITEM_TYPE type) {
	m_eItem = type;
}

/** 
* @fn		CItem::GetItem
* @brief	設定されているアイテムの種類を取得する
* @return	(ITEM_TYPE)	このオブジェクトのアイテムタイプ 
*/
ITEM_TYPE CItem::GetItem() {
	return m_eItem;
}

/**
* @fn		CItem::SetPlayer
* @brief	プレイヤーに取得されたかどうかを設定する
* @param	(bool)	プレイヤーに取得されたらtrue
*/
void CItem::SetPlayer(bool player) {
	m_bPlayer = player;
}