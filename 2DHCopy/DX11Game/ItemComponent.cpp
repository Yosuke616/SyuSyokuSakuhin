/**インクルード部**/
#include "ItemComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "imgui.h"

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
	m_bUpDown = false;
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
		//回転させる(Z軸回転)
		m_pTransform->Rotate.z += 1.0f;

		//上下に移動させる
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
	case ITEM_OHUDA:break;
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