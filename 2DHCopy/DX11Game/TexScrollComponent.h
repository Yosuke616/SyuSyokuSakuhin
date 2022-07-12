/**
* @file		TexScrollComponent.h
* @brief	テクスチャのスクロール
* @author	志水陽祐
* @date		2022/6/30
*/

//インクルードガード
#ifndef _TEX_SCROLL_H_
#define _TEX_SCROLL_H_

//インクルード
#include "Component.h"

//前方宣言
class CDraw2D;

//クラス定義
class CTexScroll : public Component {
private:
	//メンバ変数
	/** @brief 描画の設定*/
	CDraw2D*	m_pDraw2D;
	/** @brief スクロールの基準とするX座標*/
	float*		m_pPosX;
	/** @brief スクロールの基準とするY座標*/
	float*		m_pPosY;

	/** @brief スクロールの値*/
	XMFLOAT2 m_Scroll;
	/** @brief スクロールの値(0~1)*/
	XMFLOAT2 m_UVscroll;
	/** @brief スクロールの量*/
	XMFLOAT2 m_ScrollValue;
	/** @brief スクロールするかフラグ*/
	static bool m_bScroll;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CTexScroll();
	/** @brief デストラクタ*/
	~CTexScroll() override;
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新処理*/
	void Update()override;
	/** @brief 描画処理*/
	void Draw()override;

	//セッター
	void SetAxizX(float* px);
	void SetAxizY(float* py);
	void SetScrollValueX(float fx);
	void SetScrollValueY(float fy);
	static void Scroll(bool bScroll);
};

#endif