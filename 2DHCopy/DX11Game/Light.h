/**
* @file		Light.h
* @brief	ライトを設定するためのクラス
* @author	志水陽祐
* @date		2022/7/5
*/

//インクルードガード
#ifndef _LIGHT_H_
#define _LIGHT_H_

//インクルード部
#include "mainApp.h"

//クラス定義
class CLight
{
	//=== メンバ変数 ===
private:
	/** @brief 反射光(光が当たってる状態)*/
	XMFLOAT4 m_diffuse;	
	/** @brief 環境光(光が当たってない状態)*/
	XMFLOAT4 m_ambient;		
	/** @brief 自己発光*/
	XMFLOAT4 m_specular;	
	/** @brief ライトの向き*/
	XMFLOAT3 m_direction;
	/** @brief ライティング有効/無効*/
	bool m_bEnable;		


	//=== メンバ関数 ===
public:
	/** @brief コンストラクタ*/
	CLight();								
	/** @brief 初期化*/
	void Init();					
	/** @brief 反射光の取得*/
	XMFLOAT4& GetDiffuse();					
	/** @brief 環境光の取得*/
	XMFLOAT4& GetAmbient();					
	/** @brief 自己発光の取得*/
	XMFLOAT4& GetSpecular();				
	/** @brief 光源方向取得*/
	XMFLOAT3& GetDir();						
	/** @brief ライトON*/
	void SetEnable(bool bEnable = true);	
	/** @brief ライトOFF*/
	void SetDisable(bool bDisable = true);	

	/** @brief ライトのインスタンス取得*/
	static CLight* Get();					
};

#endif