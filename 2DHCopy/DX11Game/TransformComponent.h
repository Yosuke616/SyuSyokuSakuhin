/**
* @fn		TransformComponent.h
* @brief	座標のあれこれのためのコンポーネント
* @author	志水陽祐
* @date		2022/6/23
*/

//インクルードガード
#ifndef _TRANSFORM_COMPONENT_H_
#define _TRANSFORM_COMPONENT_H_

//インクルード部
#include "mainApp.h"
#include "Component.h"

//クラス定義
class CTransform :public Component {
private:
	//メンバ変数

	//メンバ関数
protected:
	//メンバ変数

	//メンバ関数
public:
	//メンバ変数
	/** @brief 座標*/
	XMFLOAT3 Pos;
	/** @brief 速度*/
	XMFLOAT3 Vel;
	/** @brief 加速度*/
	XMFLOAT3 Acc;
	/** @brief 大きさ*/
	XMFLOAT3 Scale;
	/** @brief 回転*/
	XMFLOAT3 Rotate;

	//メンバ関数
	/** @brief コンストラクタ*/
	CTransform();
	/** @brief デストラクタ*/
	~CTransform();
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 座標の設定*/
	void SetPosition(float fx,float fy,float fz = 0.0f);
	/** @brief 速度の設定*/
	void SetVelocity(float fx,float fy,float fz = 0.0f);
	/** @brief 大きさの設定*/
	void SetScale(float fx,float fy,float fz = 0.0f);
	/** @brief 傾き(回転)の設定*/
	void SetRotate(float fx ,float fy,float fz = 0.0f);
};

#endif