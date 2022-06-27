//インクルード部
#include "TransformComponent.h"

/**
* @fn		CTransform
* @brief	コンストラクタだったもの
*/
CTransform::CTransform() {
	//コンポーネントリストの更新順序の設定
	m_eUpdateOrder = COM_UPDATE_TRANSRATE;

	Pos			= XMFLOAT3(0.0f,0.0f,0.0f);
	Vel			= XMFLOAT3(0.0f,0.0f,0.0f);
	Acc			= XMFLOAT3(0.0f,0.0f,0.0f);
	Scale		= XMFLOAT3(0.0f,0.0f,0.0f);
	Rotate		= XMFLOAT3(0.0f,0.0f,0.0f);
}

/**
* @fn		~CTransform
* @brief	いなくなったデストラクタ
*/
CTransform::~CTransform() {

}

/**
* @fn		CTransform::Update
* @breif	更新処理
*/
void CTransform::Update() {
	//加速度を加算
	Vel.x += Acc.x;
	Vel.y += Acc.y;
	Vel.z += Acc.z;
	//座標に速度を加算
	Pos.x += Vel.x;
	Pos.y += Vel.y;
	Pos.z += Vel.z;
}

/**
* @fn		CTransform::SetPosition
* @brief	座標の設定
* @param	(float)		設定したいx座標
* @param	(float)		設定したいy座標
* @param	(float)		設定したいz座標
*/
void CTransform::SetPosition(float fx,float fy,float fz) {
	Pos.x = fx;
	Pos.y = fy;
	Pos.z = fz;
}

/**
* @fn		CTransform::SetVelocity
* @brief	速度の設定
* @param	(float)		設定したい速度(x)
* @param	(float)		設定したい速度(y)
* @param	(float)		設定したい速度(z)
*/
void CTransform::SetVelocity(float fx,float fy,float fz) {
	Vel.x = fx;
	Vel.y = fy;
	Vel.z = fz;
}

/**
* @fn		CTransform::SetScale
* @brief	大きさの設定
* @param	(float)		設定したい大きさ(x)
* @param	(float)		設定したい大きさ(y)
* @param	(float)		設定したい大きさ(z)
*/
void CTransform::SetScale(float fx,float fy,float fz) {
	Scale.x = fx;
	Scale.y = fy;
	Scale.z = fz;
}

/**
* @fn		CTransform::SetRotate
* @brief	角度(回転)の設定
* @param	(float)		設定したい傾き(回転)(x)
* @param	(float)		設定したい傾き(回転)(y)
* @param	(float)		設定したい傾き(回転)(z)
*/
void CTransform::SetRotate(float fx,float fy,float fz) {
	Rotate.x = fx;
	Rotate.y = fy;
	Rotate.z = fz;
}
