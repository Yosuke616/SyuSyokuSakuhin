//インクルード部
#include "TexScrollComponent.h"
#include "Draw2DComponent.h"
#include "Camera.h"

#define RATE_CHASE_SCROLL (0.1f)

//静的メンバ変数
bool CTexScroll::m_bScroll = true;

/**
* @fn		CTexScroll::CTexScroll
* @brief	もはやコンストラクタとは言えない
*/
CTexScroll::CTexScroll() 
	:m_pDraw2D(nullptr),
	 m_pPosX(nullptr),m_pPosY(nullptr){
	m_Scroll = XMFLOAT2(0.0f,0.0f);
	m_UVscroll = XMFLOAT2(0.0f,0.0f);
	m_ScrollValue = XMFLOAT2(0.0f,0.0f);
}

/**
* @fn		CTexScroll::~CTexScroll
* @brief	デストラクタが死んだ　この人でなし！
*/
CTexScroll::~CTexScroll() {

}

/**
* @fn		CTexScroll::Start()
* @brief	初期化処理
*/
void CTexScroll::Start() {

	m_pDraw2D = Parent->GetComponent<CDraw2D>();
}

/**
* @fn		CTexScroll::Update
* @brief	更新処理
*/
void CTexScroll::Update() {
	//スクロールしない場合
	if (!m_bScroll) {
		//現在のUV移動量を設置
		m_pDraw2D->SetScroll(m_UVscroll);
		return;
	}

	//X方向のスクロール基準が設定されていたら
	if (m_pPosX) {
		//現在のスクロール量と目的のスクロール量の差分を求める
		float defferenceX = (*m_pPosX * m_ScrollValue.x) - m_Scroll.x;

		//今回のスクロール = 今までのスクロール量 + 差分 * 割合
		m_Scroll.x = m_Scroll.x + defferenceX * RATE_CHASE_SCROLL;

		//小数点だけで取り出す
		m_UVscroll.x = m_Scroll.x - (int)m_Scroll.x;
	}

	//Y方向のスクロール基準が設定されていたら
	if(m_pPosY){
		m_UVscroll.y = *m_pPosY;
	}

	//テクスチャのUV移動量をセット
	m_pDraw2D->SetScroll(m_UVscroll);

}

/**
* @fn		CTexScroll::Draw
* @brief	描画関数は描画しないらしい
*/
void CTexScroll::Draw() {

}

/**
* @fn		CTexScroll::SetAxizX
* @brief	????????
* @param	????????
*/
void CTexScroll::SetAxizX(float *px) {
	m_pPosX = px;
}

/**
* @fn		CTexScroll::SetAxizY
* @brief	????????
* @param	????????
*/
void CTexScroll::SetAxizY(float* py) {
	m_pPosY = py;
}

/**
* @fn		CTexScroll::SetScrollValue
* @brief	????????
* @param    ????????
*/
void CTexScroll::SetScrollValueX(float fx) {
	m_ScrollValue.x = fx;
}

/**
* @fn		CTexScroll::SetScrollValueY
* @brief	????????
* @param	????????
*/
void CTexScroll::SetScrollValueY(float fy) {
	m_ScrollValue.y = fy;
}

/**
* @fn		CTexScroll::Scroll
* @brief	?????????
* @param	?????????
*/
void CTexScroll::Scroll(bool bScroll) {
	m_bScroll = bScroll;
}