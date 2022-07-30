//=============================================================================
//
// カメラ クラス [Camera.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Camera.h"

#include "InputManager.h"
#include "imgui.h"
#include "TransformComponent.h"
#include "sceneGame.h"
#include "TexScrollComponent.h"
#include "SceneManager.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace {
	const float CAM_POS_P_X = 0.0f;					// カメラの視点初期位置(X座標)
	const float CAM_POS_P_Y = 40.0f;					// カメラの視点初期位置(Y座標)
	const float CAM_POS_P_Z = -300.0f;				// カメラの視点初期位置(Z座標)
	const float CAM_POS_R_X = 0.0f;					// カメラの注視点初期位置(X座標)
	const float CAM_POS_R_Y = 0.0f;					// カメラの注視点初期位置(Y座標)
	const float CAM_POS_R_Z = 0.0f;					// カメラの注視点初期位置(Z座標)

	const float VIEW_ANGLE = 45.0f;					// ビュー平面の視野角

	const float VIEW_ASPECT = (float)SCREEN_WIDTH / SCREEN_HEIGHT;	// ビュー平面のアスペクト比

	const float VIEW_NEAR_Z = 10.0f;				// ビュー平面のNearZ値
	const float VIEW_FAR_Z = 1000.0f;				// ビュー平面のFarZ値

	const float VALUE_MOVE_CAMERA = 2.0f;			// カメラの移動量
	const float VALUE_ROTATE_CAMERA = 1.8f;			// カメラの回転量
	const float RATE_ROTATE_CAMERA = 0.20f;			// カメラの注視点への補正係数

	const float INTERVAL_CAMERA_R = 12.5f;			// モデルの視線の先までの距離
	const float RATE_CHASE_CAMERA_P = 0.35f;		// カメラの視点への補正係数
	const float RATE_CHASE_CAMERA_R = 0.20f;		// カメラの注視点への補正係数

	const float CHASE_HEIGHT_P = 100.0f;			// 追跡時の視点の高さ
	const float CHASE_HEIGHT_R = 10.0f;				// 追跡時の注視点の高さ

	CCamera g_camera;								// カメラ インスタンス
}



CCamera* CCamera::m_pCamera = &g_camera;			// 現在のカメラ

// コンストラクタ
CCamera::CCamera()
	: m_pPosX(nullptr),m_pPosY(nullptr)
	, m_vScrSize(SCREEN_WIDTH, SCREEN_HEIGHT)
{
	Init();
}

// 初期化
void CCamera::Init()
{
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);	// 視点
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);// 注視点
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);							// 上方ベクトル
	m_vSrcPos = m_vPos;
	m_vDestPos = m_vPos;
	m_vDestTarget = m_vTarget;
	m_vVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vDestAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vDestTargetAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_fAspectRatio = VIEW_ASPECT;		// 縦横比
	m_fFovY = VIEW_ANGLE;				// 視野角(単位:Degree)
	m_fNearZ = VIEW_NEAR_Z;				// 前方クリップ距離
	m_fFarZ = VIEW_FAR_Z;				// 後方クリップ距離

	float dx, dz;
	dx = m_vPos.x - m_vTarget.x;
	dz = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(dx * dx + dz * dz);

	CalcWorldMatrix();

#pragma region ---初期化で一度XMFloat4x4に反映させておかないとステージのコンストラクタでの計算がおかしくなる
	//	ビューマトリックス更新
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));

	//	射影行列更新
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
#pragma endregion
	m_Limit = XMFLOAT2(0.0f, 100000.0f);


	m_bZoom = false;
}

// 更新
void CCamera::Update()
{
	InputManager* pInput = InputManager::Instance();

	//--- game
	if (SceneGame::GetInstance()->m_bEditMode == false)
	{
#ifdef _DEBUG
		if (pInput->GetKeyPress(DIK_S)) {
			// 上旋回
			m_vDestAngle.z -= VALUE_ROTATE_CAMERA;
			if (m_vDestAngle.z < -180.0f) {
				m_vDestAngle.z += 360.0f;
			}
		}
		if (pInput->GetKeyPress(DIK_X)) {
			// 下旋回
			m_vDestAngle.z += VALUE_ROTATE_CAMERA;
			if (m_vDestAngle.z >= 180.0f) {
				m_vDestAngle.z -= 360.0f;
			}
		}
		// 初期位置
		if (pInput->GetKeyPress(DIK_HOME)) {
			// リセット
			m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);	// 視点
			m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);// 注視点
			m_vSrcPos = m_vPos;
			m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_vDestAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
#endif // _DEBUG

#pragma region --- 慣性
		// 目的の角度までの差分Y
		float fDiffRotY = m_vDestAngle.y - m_vAngle.y;
		if (fDiffRotY >= 180.0f)	fDiffRotY -= 360.0f;
		if (fDiffRotY < -180.0f)	fDiffRotY += 360.0f;
		// 目的の角度まで慣性をかけるY
		m_vAngle.y += fDiffRotY * RATE_ROTATE_CAMERA;
		if (m_vAngle.y >= 180.0f)	m_vAngle.y -= 360.0f;
		if (m_vAngle.y < -180.0f)	m_vAngle.y += 360.0f;
		// 目的の角度までの差分Z
		float fDiffRotZ = m_vDestAngle.z - m_vAngle.z;
		if (fDiffRotZ >= 180.0f)	fDiffRotZ -= 360.0f;
		if (fDiffRotZ < -180.0f)	fDiffRotZ += 360.0f;
		// 目的の角度まで慣性をかけるZ
		m_vAngle.z += fDiffRotZ * RATE_ROTATE_CAMERA;
		if (m_vAngle.z >= 180.0f)	m_vAngle.z -= 360.0f;
		if (m_vAngle.z < -180.0f)	m_vAngle.z += 360.0f;

		// 視点座標移動先を算出
		m_vSrcPos.x = -SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vSrcPos.y = -SinDeg(m_vAngle.z) * m_fLengthInterval;
		m_vSrcPos.z = -CosDeg(m_vAngle.z) * m_fLengthInterval;
		m_vDestPos.x = m_vSrcPos.x;
		m_vDestPos.y = m_vSrcPos.y;
		m_vDestPos.z = m_vSrcPos.z;
		if (m_pPosX)	m_vDestPos.x += *m_pPosX;
		
		//注視点に変更があった場合位置を加える
		if (m_pPosY) {
			m_vDestPos.y += *m_pPosY;
		}

#pragma endregion

		// 基準とするオブジェクトがあった場合
		//ゴールフラグや
		if (!m_bZoom && m_pPosX)
		{
			m_vDestTarget.x = *m_pPosX;

			// 視点を徐々に移動先に近づける
			m_vPos.x = m_vPos.x * 0.9f + m_vDestPos.x * 0.1f;

			// 注視点を徐々に移動先に近づける
			m_vTarget.x = m_vTarget.x * 0.9f + m_vDestTarget.x * 0.1f;

		}

		// 基準とするオブジェクトがあった場合
		if (m_pPosY) {
			m_vDestTarget.y = *m_pPosY;

			// 視点を徐々に移動先に近づける
			m_vPos.y = m_vPos.y * 0.9f + m_vDestPos.y * 0.1f;

			// 注視点を徐々に移動先に近づける
			m_vTarget.y = m_vTarget.y * 0.9f + m_vDestTarget.y * 0.1f;



		}

		// 2022/01/16 カメラ　ズーム
		if (m_bZoom)
		{
			// プレイヤー情報の取得
			auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>();

			// 移動目的座標
			m_vDestPos.x = Player->Pos.x;
			m_vDestPos.y = Player->Pos.y + 70.0f;
			m_vDestPos.z = GOAL_POS_Z;

			// 注視点目的座標
			m_vDestTarget = Player->Pos;
			m_vDestTarget.y += 50.0f;	// プレイヤーより少し上に設定

			//2022/01/16
			m_vPos.x = m_vPos.x * 0.95f + m_vDestPos.x * 0.05f;
			m_vPos.y = m_vPos.y * 0.95f + m_vDestPos.y * 0.05f;
			m_vPos.z = m_vPos.z * 0.95f + m_vDestPos.z * 0.05f;
			m_vTarget.x = m_vTarget.x * 0.95f + m_vDestTarget.x * 0.05f;
			m_vTarget.y = m_vTarget.y * 0.95f + m_vDestTarget.y * 0.05f;
		}


		// x軸の基準とするオブジェクトが設定されていない && プレイヤーがステージ端にいない場合
		if (!m_bZoom && GetAxisX() == nullptr && (m_vPos.x >= 0.0f || m_Limit.y <= m_vPos.x))
		{
			//	プレイヤーに注視点を合わせる
			auto player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			if (player)	SetAxisX(&player->GetComponent<CTransform>()->Pos.x);

		}

		// y軸、ステージごとの上限や下限を超えない限りプレイヤーに追従し続ける
		// 2022/7/26現在は条件は存在しない/**過ちのmistake**
		if (1) {
			//プレイヤーに注視点を合わせる
			auto player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			if (player) {
				SetAxisY(&player->GetComponent<CTransform>()->Pos.y);
			}
		}
	}


	//--- edit
	else if (SceneGame::GetInstance()->m_bEditMode)
	{
		if (pInput->GetKeyPress(DIK_RIGHT))
		{
			m_vPos.x += 2.0f;		//	カメラ座標
			m_vTarget.x += 2.0f;	//	注視点
		}
		else if (pInput->GetKeyPress(DIK_LEFT))
		{
			m_vPos.x -= 2.0f;
			m_vTarget.x -= 2.0f;
		}
	}

	//	カメラの移動限界
	if (!m_bZoom && m_vPos.x < m_Limit.x)
	{
		SetAxisX(nullptr);		//	注視点の設定を解除
		m_vPos.x = m_Limit.x;
		m_vTarget.x = m_Limit.x;

		// スクロールを止める
		if (SceneManager::Instance()->GetScene() != SCENE_ENDING)
		{
			// スクロールを止める
			CTexScroll::Scroll(false);
		}
	}
	else if (!m_bZoom && m_vPos.x > m_Limit.y)
	{
		SetAxisX(nullptr);		//	注視点の設定を解除
		m_vPos.x = m_Limit.y;
		m_vTarget.x = m_Limit.y;

		// スクロールを止める
		if (SceneManager::Instance()->GetScene() != SCENE_ENDING)
		{
			CTexScroll::Scroll(false);
		}
	}
	else
	{
		// スクロールを再開
		CTexScroll::Scroll(true);
	}

	// マトリックスに更新
	UpdateMatrix();
}


void CCamera::Draw()
{
#ifdef _DEBUG
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Camera");
	Text("Pos    : %.0f %.0f %.0f", m_vPos.x, m_vPos.y, m_vPos.z);
	Text("Target : %.0f %.0f %.0f", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	Text("Angle  : %.0f %.0f %.0f", m_vAngle.x, m_vAngle.y, m_vAngle.z);
	End();
#endif // _DEBUG
}

// ビュー/プロジェクション マトリックス更新
void CCamera::UpdateMatrix()
{
	//	ビューマトリックス更新
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));

	//	射影行列更新
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ワールド マトリックス設定
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// 視点/注視点/上方ベクトルからワールド マトリックス算出
XMFLOAT4X4& CCamera::CalcWorldMatrix()
{
	XMVECTOR vecZ = XMVectorSet(m_vTarget.x - m_vPos.x, m_vTarget.y - m_vPos.y, m_vTarget.z - m_vPos.z, 0.0f);
	XMFLOAT3 vZ;
	XMStoreFloat3(&vZ, XMVector3Normalize(vecZ));
	XMVECTOR vecY = XMLoadFloat3(&m_vUp);
	XMVECTOR vecX = XMVector3Normalize(XMVector3Cross(vecY, vecZ));
	XMFLOAT3 vX;
	XMStoreFloat3(&vX, vecX);
	vecY = XMVector3Normalize(XMVector3Cross(vecZ, vecX));
	XMFLOAT3 vY;
	XMStoreFloat3(&vY, vecY);

	m_mtxWorld._11 = vX.x;
	m_mtxWorld._12 = vX.y;
	m_mtxWorld._13 = vX.z;
	m_mtxWorld._14 = 0.0f;
	m_mtxWorld._21 = vY.x;
	m_mtxWorld._22 = vY.y;
	m_mtxWorld._23 = vY.z;
	m_mtxWorld._24 = 0.0f;
	m_mtxWorld._31 = vZ.x;
	m_mtxWorld._32 = vZ.y;
	m_mtxWorld._33 = vZ.z;
	m_mtxWorld._34 = 0.0f;
	m_mtxWorld._41 = m_vPos.x;
	m_mtxWorld._42 = m_vPos.y;
	m_mtxWorld._43 = m_vPos.z;
	m_mtxWorld._44 = 1.0f;

	return m_mtxWorld;
}


// カメラ座標の設定
void CCamera::SetPos(XMFLOAT3& vPos)
{
	m_vPos = vPos;
}
void CCamera::SetPos(float x, float y, float z)
{
	m_vPos.x = x, m_vPos.y = y, m_vPos.z = z;
}

// カメラ座標の取得
XMFLOAT3& CCamera::GetPos()
{
	return m_vPos;
}

// 注視点座標の設定
void CCamera::SetTarget(XMFLOAT3& vTarget)
{
	m_vTarget = vTarget;
}
void CCamera::SetTarget(float x, float y, float z)
{
	m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z;
}

// アップベクトルの設定
void CCamera::SetUpVector(XMFLOAT3& vUp)
{
	m_vUp = vUp;
}
void CCamera::SetUpVector(float x, float y, float z)
{
	m_vUp.x = x, m_vUp.y = y, m_vUp.z = z;
}

//	ワールドマトリックスの取得
XMFLOAT4X4& CCamera::GetWorldMatrix()
{
	return m_mtxWorld;
}

//	？？
void CCamera::SetFovY(float fFovY)
{
	m_fFovY = fFovY;
}

//	アスペクト比の設定
void CCamera::SetAspectRatio(float fAspect)
{
	m_fAspectRatio = fAspect;
}

//	ニアクリップとファークリップの設定
void CCamera::SetRangeZ(float fNearZ, float fFarZ)
{
	m_fNearZ = fNearZ, m_fFarZ = fFarZ;
}

//	ビュー		 マトリックスの取得
XMFLOAT4X4& CCamera::GetViewMatrix()
{
	return m_mtxView;
}

//	プロジェクション マトリックスの取得
XMFLOAT4X4& CCamera::GetProjMatrix()
{
	return m_mtxProj;
}

//	カメラの傾き取得
XMFLOAT3& CCamera::GetAngle()
{
	return m_vAngle;
}

const XMFLOAT3& CCamera::GetLook() const
{
	return m_vTarget;
}

const XMFLOAT3& CCamera::GetUp() const
{
	return m_vUp;
}

float CCamera::GetAspect() const
{
	//return m_vScrSize.x / m_vScrSize.y;
	return m_fAspectRatio;
}

float CCamera::GetNearClip() const
{
	return m_fNearZ;
}

float CCamera::GetFarClip() const
{
	return m_fFarZ;
}

// 視野角
float CCamera::GetFOV() const
{
	// 2022/01/18	角度からラジアンに変えた
	return XMConvertToRadians(m_fFovY);
}

CCamera& CCamera::operator=(const CCamera& camera)
{
	m_vPos = camera.m_vPos;
	m_vTarget = camera.m_vTarget;
	m_vUp = camera.m_vUp;
	m_vScrSize = camera.m_vScrSize;
	m_fNearZ = camera.m_fNearZ;
	m_fFarZ = camera.m_fFarZ;
	m_fFovY = camera.m_fFovY;
	return *this;
}


//	インスタンス取得
CCamera* CCamera::Get()
{
	return m_pCamera;
}

//	カメラの設定
void CCamera::Set(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

// fps,tps
//void CCamera::SetTransform(CTransform* transform)
//{
//	m_pTransform = transform;
//}

// カメラのX軸を他のオブジェクト依存にする
void CCamera::SetAxisX(float* px)
{
	m_pPosX = px;
	if (px)	m_vPos.x = *px;
	else return;

	m_vDestPos.x = *px;
}

/**
* @fn		CCamera::SetAxisY
* @brief	カメラのY軸を他のオブジェクト依存させる
* @param	(float*)	他オブジェクトのY座標
*/
void CCamera::SetAxisY(float* py) {
	m_pPosY = py;
	if (py) {
		m_vPos.y = *py;
	}
	else {
		return;
	}

	m_vDestPos.y = *py;
}

void CCamera::SetLimit(XMFLOAT2 limit)
{
	m_Limit.x = limit.x;
	m_Limit.y = limit.y;
}

float* CCamera::GetAxisX()
{
	return m_pPosX;
}

void CCamera::SetDestPos(float CameraposZ)
{
	m_bZoom = true;
	m_vDestPos.z = CameraposZ;
	m_vDestTarget.z = CameraposZ;

	m_vDestPos.x = m_vDestTarget.x;
}