/**インクルード部**/
#include "OutOfRange.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "EnemyComponent.h"

/**
* @fn		COutOfRange::COutOfRange
* @brief	そんなコンストラクタで大丈夫か？
*/
COutOfRange::COutOfRange() 
	:m_pTransform(nullptr){
	//範囲内の設定
	m_LimitRange.x = ENEMY_OUT_RANGE_X;
	m_LimitRange.y = ENEMY_OUT_RANGE_Y;
}

/** 
* @fn		COutOfRange::~COutOfRange
* @brief	一番いいデストラクタをくれ
*/
COutOfRange::~COutOfRange() {

}

/**
* @fn		CoutOfRange::Start
* @brief	初期化
*/
void COutOfRange::Start() {
	//親オブジェクトの座標の取得
	m_pTransform = Parent->GetComponent<CTransform>();
}

/**
* @fn		COutOfRange::Update
* @brief	更新処理
* @detail	カメラを基準にし、一定以上離れたらオブジェクトの機能を停止する
*/
void COutOfRange::Update() {
	//画面の中心座標の取得
	XMFLOAT3 Center = CCamera::Get()->GetPos();

	//一定の距離離れていたら更新を止める
	if (Center.x + m_LimitRange.x < m_pTransform->Pos.x ||		// 画面外
		Center.x - m_LimitRange.x > m_pTransform->Pos.x	||		// 画面外
		Center.y + m_LimitRange.y < m_pTransform->Pos.y ||		// 画面外
		Center.y - m_LimitRange.y > m_pTransform->Pos.y)		// 画面外
	{ 
		////動くものがあった場合ここで座標をいじる
		//if (Parent->GetName() == "Enemy") {
		//	//ここで最終的にいた場所を保存する
		//	Parent->GetComponent<CEnemy>()->SetRangePos();
		//	//次に画面内に入ったときに座標を更新できるようにフラグをオンにする
		//	Parent->GetComponent<CEnemy>()->SetFirstTake(true);
		//}

		//機能の停止
		Parent->StopUpdate();
		Parent->StopDraw();


		//このコンポーネントの更新フラグをオンにする
		this->m_bUpdateFlag = true;
	}
	else {
		//更新を再開
		Parent->Use();

		////動くものは更新した後に座標を元に戻す
		//if (Parent->GetName() == "Enemy") {	
		//	//一度だけ座標をセットしたいのでフラグをOnOffする
		//	if (Parent->GetComponent<CEnemy>()->GetFirstTake()) {
		//		Parent->GetComponent<CTransform>()->Pos = Parent->GetComponent<CEnemy>()->GetRangePos();
		//		Parent->GetComponent<CEnemy>()->SetFirstTake(false);
		//	}
		//}
	}
}

/**
* @fn		COutOfRange::Draw
* @brief	描画処理
*/
void COutOfRange::Draw() {

}

/**
* @fn		COutOfRange::OnCollisionEnter
* @brief	衝突判定の処理
* @detail	内装はないそうです
* @param	(Object*)	衝突したオブジェクトの情報
*/
void COutOfRange::OnCollisionEnter(Object* pObject) {

}

/**
* @fn		COutOfRange::NoFunction
* @brief	このコンポーネント以外の処理を停止する
*/
void COutOfRange::NoFunction() {
	//機能の停止
	Parent->NoUse();

	//このコンポーネントだけ更新する
	this->m_bUpdateFlag = true;
}

/**
* @fn		COutOfRange::SetLimitRange 
* @brief	範囲の設定
* @detail	個別で範囲を設定したいときに必要になってくるのでしょう
* @param	(float)	横の範囲の設定
* @param	(float)	縦の範囲の設定
*/
void COutOfRange::SetLimitRange(float x, float y) {
	m_LimitRange.x = x;
	m_LimitRange.y = y;
}