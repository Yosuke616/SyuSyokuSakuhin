//=============================================================================
//
// カメラ クラス [Camera.h]
// Author : SHIMIZU YOSUKE
//
//=============================================================================
#pragma once
#include "mainApp.h"

class CTransform;

/**定数定義**/
#define GOAL_POS_Z	(-200.0f)
//ステージごとの限界の値
#define STAGE_REMIT_Y_MIN		(58.0f)		//ステージ共通？の高さの最小値
#define STAGE_REMIT_Y_MAX		(1000.0f)	//ステージ共通？の高さの最大値
#define STAGE_1_REMIT_MIN		(0.0f)		//ステージ1の左端
#define STAGE_1_REMIT_MAX		(3650.0f)	//ステージ1の右端
#define STAGE_1_WARP_MIN		(4500.0f)		//ワープ先のミニマム
#define STAGE_1_WARP_MAX		(4550.0f)


#define STAGE_1_RE_REMIT_MIN	(0.0f)		//ステージ1(裏)の左端
#define STAGE_1_RE_REMIT_MAX	(4000.0f)	//ステージ1(裏)の右端

class CCamera
{
private:
	//CTransform* m_pTransform;
	float* m_pPosX;
	/** @brief Y軸用の座標ポインタ 2022/7/26*/
	float* m_pPosY;
	bool m_bZoom;

	XMFLOAT3 m_vPos;				// 視点
	XMFLOAT3 m_vTarget;				// 注視点
	XMFLOAT3 m_vUp;					// 上方ベクトル
	XMFLOAT2 m_Limit;
	XMFLOAT2 m_LimitY;				// Y軸方向の上限

	XMFLOAT3 m_vSrcPos;				// 元の視点

	XMFLOAT3 m_vDestPos;			// 視点の目的位置
	XMFLOAT3 m_vDestTarget;			// 注視点の目的位置
	XMFLOAT3 m_vVelocity;			// 移動量

	float m_fAspectRatio;			// 縦横比
	float m_fFovY;					// 視野角(Degree)
	float m_fNearZ;					// 前方クリップ距離
	float m_fFarZ;					// 後方クリップ距離

	XMFLOAT4X4 m_mtxWorld;			// ワールド マトリックス
	XMFLOAT4X4 m_mtxView;			// ビュー マトリックス
	XMFLOAT4X4 m_mtxProj;			// プロジェクション マトリックス

	//2022/01/08
	XMFLOAT2 m_vScrSize;			// 画面サイズ

	XMFLOAT3 m_vAngle;				// カメラの角度
	XMFLOAT3 m_vDestAngle;			// カメラの目的の向き
	XMFLOAT3 m_vDestTargetAngle;	// 注視点の目的の向き
	float m_fLengthInterval;		// カメラの視点と注視点の距離

	static CCamera* m_pCamera;		// 現在のカメラ

public:
	CCamera();						//	コンストラクタ
	void Init();					//	初期化
	void Update();					//	更新
	void Draw();

	XMFLOAT4X4& CalcWorldMatrix();					//	視点/注視点/上方ベクトルからワールド マトリックス算出
	void UpdateMatrix();							//	ビュー/プロジェクション マトリックス更新
	//setter
	void SetPos(XMFLOAT3& vPos);					//	カメラ座標の設定
	void SetPos(float x, float y, float z);			//	カメラ座標の設定
	void SetTarget(XMFLOAT3& vTarget);				//	注視点の設定
	void SetTarget(float x, float y, float z);		//	注視点の設定
	void SetUpVector(XMFLOAT3& vUp);				//	アップベクトルの設定
	void SetUpVector(float x, float y, float z);	//	アップベクトルの設定
	void SetWorldMatrix(XMFLOAT4X4& mtxWorld);		//	ワールドマトリックスの設定
	void SetFovY(float fFovY);						//	？？
	void SetAspectRatio(float fAspect);				//	アスペクト比の設定
	void SetRangeZ(float fNearZ, float fFarZ);		//	ニアクリップとファークリップの設定
	void SetLimit(XMFLOAT2 limit);
	void SetDestPos(float CameraposZ);
	//getter
	XMFLOAT3& GetPos();								//	カメラ座標の取得
	XMFLOAT3& GetAngle();							//	カメラの傾き取得
	XMFLOAT4X4& GetWorldMatrix();					//	ワールド		 マトリックスの取得
	XMFLOAT4X4& GetViewMatrix();					//	ビュー		 マトリックスの取得
	XMFLOAT4X4& GetProjMatrix();					//	プロジェクション マトリックスの取得
	float* GetAxisX();
	// Effekseer
	const XMFLOAT3& GetLook() const;
	const XMFLOAT3& GetUp() const;
	float GetAspect() const;
	float GetNearClip() const;
	float GetFarClip() const;
	float GetFOV() const;
	CCamera& operator=(const CCamera& camera);

	static CCamera* Get();							//	インスタンス取得
	static void Set(CCamera* pCamera);				//	現在のカメラの設定

	//void SetTransform(CTransform* transform);	//	x,y,z固定
	void SetAxisX(float* px);
	/** @brief Y軸の注視点の設定をする 2022/7/26*/
	void SetAxisY(float* py);
	/** @brief カメラを動かなくする 2022/9/9*/

	/** @brief カメラの動きを再開する*/

};
