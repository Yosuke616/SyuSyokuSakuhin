/**
* @fn		Draw2DComponent.h
* @brief	2D描画をするコンポーネント
* @author	志水陽祐
* @date		2022/6/22
*/

//インクルードガード
#ifndef _DRAW_2D_COMPONENT_H_
#define _DRAW_2D_COMPONENT_H_

//インクルード部
#include "mainApp.h"
#include "Component.h"

//前方宣言
class CTransform;

//クラス定義
class CDraw2D : public Component {
private:
	//メンバ変数

	//メンバ関数

protected:
	//メンバ変数
	/** @brief 座標などの為の変数*/
	CTransform* m_pTransform;
	/** @brief テクスチャの情報の為の変数*/
	ID3D11ShaderResourceView* m_pTexture;
	/** @brief 四角形ポリゴンの大きさ*/
	XMFLOAT2 m_fPolygonSize;
	/** @brief テクスチャの割り当て範囲*/
	XMFLOAT2 m_fTexFrameSize;
	/** @brief 頂点カラー*/
	XMFLOAT4 m_fVertevColor;
	/** @brief 角度*/
	float m_fAngle;
	/** @brief スクロール*/
	XMFLOAT2 m_Scroll;
	/** @brief ブレンドの種類*/
	EBlendState m_BlendState;
	/** @brief アニメーション番号*/
	int m_nAnimNum;
	/** @brief 横のアニメーションの分割数*/
	int m_nAnimSplitX;
	/** @brief 縦のアニメーションの分割数*/
	int m_nAnimSplitY;
	
	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CDraw2D();
	/** @brief デストラクタ*/
	~CDraw2D() override;
	/** @brief 初期化処理*/
	void Start() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;
	/** @brief ポリゴンのサイズ設定*/
	void SetSize(float,float);
	/** @brief UVサイズ設定*/
	void SetUVsize(float,float);
	/** @brief アニメーション分割数の設定*/
	void SetAnimSplit(int ,int);
	/** @brief アニメーション番号の設定*/
	void SetAnimNum(int);
	/** @brief テクスチャの設定*/
	void SetTexture(ID3D11ShaderResourceView*);
	/** @brief カラー設定*/
	void SetColor(float r,float g,float b);
	/** @brief 透明度の設定*/
	void SetAlpha(float alpha);
	/** @brief 角度の設定*/
	void SetAngle(float angle);
	/** @brief スクロール量の設定*/
	void SetScroll(XMFLOAT2 scroll);
	/** @brief ブレンドステートの設定*/
	void SetBlend(EBlendState state);
	/** @brief UVのUの設定*/
	void SetU(float u);
	/** @brief UVのVの設定*/
	void SetV(float v);

	/** @brief 横方向の分割数を得る*/
	int GetAnimSplitX();
};

#endif