//インクルード部
#include "Draw2DComponent.h"
#include "TransformComponent.h"
#include "polygon.h"
#include "ObjectManager.h"

/**
* @fn		CDraw2D::CDraw2D
* @brief	コンストラクタが始まるということ
*/
CDraw2D::CDraw2D() 
	:m_pTexture(0),m_fAngle(0.0f),
	 m_nAnimNum(0),m_nAnimSplitX(1),m_nAnimSplitY(1),
	 m_BlendState(BS_ALPHABLEND){

	m_fPolygonSize = XMFLOAT2(0.0f,0.0f);
	m_fTexFrameSize = XMFLOAT2(1.0f,1.0f);
	m_fVertevColor = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	m_Scroll = XMFLOAT2(0.0f,0.0f);
	m_eUpdateOrder = (COM_UPDATE_DRAW);
}

/** 
* @fn		CDraw2D::~CDraw2d
* @brief	返事がない　ただのデストラクタのようだ　
*/
CDraw2D::~CDraw2D() {

}

/**
* @fn		CDraw2D::Start
* @brief	初期化の処理
*/
void CDraw2D::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
}

/**
* @fn		CDraw2D::Update
* @brief	更新処理だったもの
*/
void CDraw2D::Update() {

}

/**
* @fn		CDraw2D::SetTexture
* @brief	テクスチャをセットする
* @param	(ID3D11ShaderResourceView*)	テクスチャのポインタ
*/
void CDraw2D::SetTexture(ID3D11ShaderResourceView* pTex) {
	m_pTexture = pTex;
}

/**
* @fn		CDraw2D::SetAnimNum
* @brief	アニメーション番号をセットする
* @param	(int)	現在のアニメーションの番号
*/
void CDraw2D::SetAnimNum(int anim) {
	m_nAnimNum = anim;
}

/**
* @fn		CDraw2D::SetUVsize
* @brief	テクスチャのサイズを設定する
* @param	(float)		テクスチャの横幅
* @param	(float)		テクスチャの立幅
*/
void CDraw2D::SetUVsize(float UVSizeX,float UVSizeY) {
	m_fTexFrameSize.x = UVSizeX;
	m_fTexFrameSize.y = UVSizeY;
}

/**
* @fn		CDraw2D::SetU
* @brief	UVのUの設定
* @param	(float)		Uである
*/
void CDraw2D::SetU(float u) {
	m_fTexFrameSize.x = u;
}

/**
* @fn		CDraw2D::SetV
* @brief	UVのVの設定
* @param	(float)		実はVだった
*/
void CDraw2D::SetV(float v) {
	m_fTexFrameSize.y = v;
}

/**
* @fn		CDraw2D::SetSize
* @brief	ポリゴンの大きさを設定する
* @param	(float)	ポリゴンの横幅を決める
* @param	(float)	ポリゴンの立幅を決める
*/
void CDraw2D::SetSize(float SizeX,float SizeY) {
	m_fPolygonSize.x = SizeX;
	m_fPolygonSize.y = SizeY;
}

/**
* @fn		CDraw2D::SetColor
* @brief	色の設定
* @param	(float)	RGBの赤色でした
* @param	(float)	RGBの緑のひげ
* @param	(float) RGBの青空教室
*/
void CDraw2D::SetColor(float red,float green,float blue){
	m_fVertevColor.x = red;
	m_fVertevColor.y = green;
	m_fVertevColor.z = blue;
}

/**
* @fn		CDraw2D::SetAlpha
* @brief	透明度を設定する
* @param	(float)	透明度を下げて奥側を見よう
*/
void CDraw2D::SetAlpha(float alpha) {
	m_fVertevColor.w = alpha;
}

/**
* @fn		CDraw2D::SetAngle
* @brief	角度の設定
* @param	(float)	どの角度にするかの数値
*/
void CDraw2D::SetAngle(float angle) {
	m_fAngle = angle;
}

/**
* @fn		CDraw2D::SetAnimSplit
* @brief	アニメーションの分割数を設定
* @param	(int)	横の分割数の数値
* @param	(int)	縦の分割数の数値
*/
void CDraw2D::SetAnimSplit(int x,int y) {
	m_nAnimSplitX = x;
	m_nAnimSplitY = y;
	m_fTexFrameSize.x = 1.0f / x;
	m_fTexFrameSize.y = 1.0f / y;
}

/**
* @fn		CDraw2D::SetScroll
* @brief	スクロール量の設定
* @param	(XMFLOAT2)	どの位動かせばいいかの設定
*/
void CDraw2D::SetScroll(XMFLOAT2 scroll) {
	m_Scroll.x = scroll.x;
	m_Scroll.y = scroll.y;
}

/**
* @fn		CDraw2D::SetBlend
* @brief	ブレンドステートの設定
* @param	(EBlendState)	ブレンドステートの状態
*/
void CDraw2D::SetBlend(EBlendState state) {
	m_BlendState = state;
}

/**
* @fn		CDraw2D::GetAnimSplitX
* @brief	アニメーション分割数の横方向を取得する
* @return	(int) どの位分かれているかを返す
*/
int CDraw2D::GetAnimSplitX() {
	return m_nAnimSplitX;
}

/**
* @fn		CDraw2D::Draw
* @brief	描画関数	
* @detail	
*/
void CDraw2D::Draw() {
	//デバイスコンテキストの取得
	ID3D11DeviceContext* pDC = GetDeviceContext();
	//Zバッファを更新しない
	SetZBuffer(false);
	//アルファブレンド有効
	SetBlendState(m_BlendState);

	//テクスチャのセット
	SetPolygonTexture(m_pTexture);
	//ポリゴンの座標をセット
	SetPolygonPos(m_pTransform->Pos.x,m_pTransform->Pos.y);
	//ポリゴンの大きさをセット
	SetPolygonSize(m_fPolygonSize.x,m_fPolygonSize.y);
	//テクスチャ割り当て
	SetPolygonFrameSize(m_fTexFrameSize.x,m_fTexFrameSize.y);
	//アニメーション番号設定
	SetPolygonUV(
		(m_nAnimNum % m_nAnimSplitX) / (float)m_nAnimSplitX + m_Scroll.x,
		(m_nAnimNum / m_nAnimSplitX) / (float)m_nAnimSplitY + m_Scroll.y);
	//ポリゴンの色を設定
	SetPolygonColor(m_fVertevColor.x,m_fVertevColor.y,m_fVertevColor.z);
	//ポリゴンの透明度を設定
	SetPolygonAlpha(m_fVertevColor.w);
	//ポリゴンの傾きの設定
	SetPolygonAngle(m_fAngle);

	//描画してください何でもします状態
	DrawPolygon(pDC);

	//設定を元に戻す
	SetPolygonColor(1,1,1);
	SetPolygonAlpha(1.0f);
	SetPolygonAngle(0.0f);
	SetPolygonFrameSize(1.0f,1.0f);
	SetPolygonUV(0.0f,0.0f);
}
