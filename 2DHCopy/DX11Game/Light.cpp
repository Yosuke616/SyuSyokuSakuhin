//インクルード部
#include "Light.h"

//名前空間
using namespace DirectX;

// グローバル変数
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 1.0f);			//	ライトの向き(画面の奥下方向)
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);		//	反射光(光が当たってる状態)	
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);	//	環境光(光が当たってない状態)	
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);		//	自己発光	

	//	インスタンス作成
	CLight g_light;
};

/**
* @fn		CLight::CLight
* @brief	コンストラクタが耳元でささやいている
*/
CLight::CLight()
{
	//	初期化
	Init();
}

/**
* @fn		CLight::Init
* @brief	初期化します
*/
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;										//	ライトの向き(画面の奥下方向)
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));	//	単位ベクトルにしてストアに保存

	m_diffuse = LIGHT0_DIFFUSE;		//	反射光(光が当たってる状態)	
	m_ambient = LIGHT0_AMBIENT;		//	環境光(光が当たってない状態)
	m_specular = LIGHT0_SPECULAR;	//	自己発光	
	m_bEnable = true;
}

/**
* @fn		CLight::GetDiffuse
* @brief	反射光の取得
* @return	(XMFLOAT4&)	反射光の情報が入っているアドレスを返す
*/
XMFLOAT4& CLight::GetDiffuse()
{
	return m_diffuse;
}

/**
* @fn		CLight::GetAmbient
* @brief	環境光の取得
* @return	(XMFLOAT4&)	環境光の情報が入っているアドレスを返す
*/
XMFLOAT4& CLight::GetAmbient()
{
	return m_ambient;
}

/**
* @fn		CLight::GetSpecular
* @brief	自己発光の取得
* @return	(XMFLOAT4&)	自己発光の情報が入っているアドレスを返す
*/
XMFLOAT4& CLight::GetSpecular()
{
	return m_specular;
}


/**
* @fn		CLight::GetDir
* @brief	光源方向を取得する
* @return	(XMFLOAT3&)	ライトがオンになっていた場合その方向を返す
*/
XMFLOAT3& CLight::GetDir()
{
	//	ライトON
	if (m_bEnable) return m_direction;

	//	ライトOFF
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

/**
* @fn		CLight::Get
* @brief	光源インスタンスの取得をする
* @param	(CLight*)	インスタンス情報のポインタを返す
*/
CLight* CLight::Get()
{
	return &g_light;
}

/**
* @fn		CLight::SetEnable
* @brief	ライトをONにする
* @oaram	(bool)	ライトを付けるかどうかよ
*/
void CLight::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

/**
* @fn		CLight::SetDisable
* @brief	ライトをオフにする
* @param	(bool)	ライトを消すかどうか　つまり0
*/
void CLight::SetDisable(bool bDisable)
{
	m_bEnable = !bDisable;
}
