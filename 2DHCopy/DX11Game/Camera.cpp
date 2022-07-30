//=============================================================================
//
// �J���� �N���X [Camera.cpp]
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
// �O���[�o���ϐ�
//*****************************************************************************
namespace {
	const float CAM_POS_P_X = 0.0f;					// �J�����̎��_�����ʒu(X���W)
	const float CAM_POS_P_Y = 40.0f;					// �J�����̎��_�����ʒu(Y���W)
	const float CAM_POS_P_Z = -300.0f;				// �J�����̎��_�����ʒu(Z���W)
	const float CAM_POS_R_X = 0.0f;					// �J�����̒����_�����ʒu(X���W)
	const float CAM_POS_R_Y = 0.0f;					// �J�����̒����_�����ʒu(Y���W)
	const float CAM_POS_R_Z = 0.0f;					// �J�����̒����_�����ʒu(Z���W)

	const float VIEW_ANGLE = 45.0f;					// �r���[���ʂ̎���p

	const float VIEW_ASPECT = (float)SCREEN_WIDTH / SCREEN_HEIGHT;	// �r���[���ʂ̃A�X�y�N�g��

	const float VIEW_NEAR_Z = 10.0f;				// �r���[���ʂ�NearZ�l
	const float VIEW_FAR_Z = 1000.0f;				// �r���[���ʂ�FarZ�l

	const float VALUE_MOVE_CAMERA = 2.0f;			// �J�����̈ړ���
	const float VALUE_ROTATE_CAMERA = 1.8f;			// �J�����̉�]��
	const float RATE_ROTATE_CAMERA = 0.20f;			// �J�����̒����_�ւ̕␳�W��

	const float INTERVAL_CAMERA_R = 12.5f;			// ���f���̎����̐�܂ł̋���
	const float RATE_CHASE_CAMERA_P = 0.35f;		// �J�����̎��_�ւ̕␳�W��
	const float RATE_CHASE_CAMERA_R = 0.20f;		// �J�����̒����_�ւ̕␳�W��

	const float CHASE_HEIGHT_P = 100.0f;			// �ǐՎ��̎��_�̍���
	const float CHASE_HEIGHT_R = 10.0f;				// �ǐՎ��̒����_�̍���

	CCamera g_camera;								// �J���� �C���X�^���X
}



CCamera* CCamera::m_pCamera = &g_camera;			// ���݂̃J����

// �R���X�g���N�^
CCamera::CCamera()
	: m_pPosX(nullptr),m_pPosY(nullptr)
	, m_vScrSize(SCREEN_WIDTH, SCREEN_HEIGHT)
{
	Init();
}

// ������
void CCamera::Init()
{
	m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);	// ���_
	m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);// �����_
	m_vUp = XMFLOAT3(0.0f, 1.0f, 0.0f);							// ����x�N�g��
	m_vSrcPos = m_vPos;
	m_vDestPos = m_vPos;
	m_vDestTarget = m_vTarget;
	m_vVelocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vDestAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vDestTargetAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_fAspectRatio = VIEW_ASPECT;		// �c����
	m_fFovY = VIEW_ANGLE;				// ����p(�P��:Degree)
	m_fNearZ = VIEW_NEAR_Z;				// �O���N���b�v����
	m_fFarZ = VIEW_FAR_Z;				// ����N���b�v����

	float dx, dz;
	dx = m_vPos.x - m_vTarget.x;
	dz = m_vPos.z - m_vTarget.z;
	m_fLengthInterval = sqrtf(dx * dx + dz * dz);

	CalcWorldMatrix();

#pragma region ---�������ň�xXMFloat4x4�ɔ��f�����Ă����Ȃ��ƃX�e�[�W�̃R���X�g���N�^�ł̌v�Z�����������Ȃ�
	//	�r���[�}�g���b�N�X�X�V
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));

	//	�ˉe�s��X�V
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
#pragma endregion
	m_Limit = XMFLOAT2(0.0f, 100000.0f);


	m_bZoom = false;
}

// �X�V
void CCamera::Update()
{
	InputManager* pInput = InputManager::Instance();

	//--- game
	if (SceneGame::GetInstance()->m_bEditMode == false)
	{
#ifdef _DEBUG
		if (pInput->GetKeyPress(DIK_S)) {
			// �����
			m_vDestAngle.z -= VALUE_ROTATE_CAMERA;
			if (m_vDestAngle.z < -180.0f) {
				m_vDestAngle.z += 360.0f;
			}
		}
		if (pInput->GetKeyPress(DIK_X)) {
			// ������
			m_vDestAngle.z += VALUE_ROTATE_CAMERA;
			if (m_vDestAngle.z >= 180.0f) {
				m_vDestAngle.z -= 360.0f;
			}
		}
		// �����ʒu
		if (pInput->GetKeyPress(DIK_HOME)) {
			// ���Z�b�g
			m_vPos = XMFLOAT3(CAM_POS_P_X, CAM_POS_P_Y, CAM_POS_P_Z);	// ���_
			m_vTarget = XMFLOAT3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);// �����_
			m_vSrcPos = m_vPos;
			m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
			m_vDestAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
		}
#endif // _DEBUG

#pragma region --- ����
		// �ړI�̊p�x�܂ł̍���Y
		float fDiffRotY = m_vDestAngle.y - m_vAngle.y;
		if (fDiffRotY >= 180.0f)	fDiffRotY -= 360.0f;
		if (fDiffRotY < -180.0f)	fDiffRotY += 360.0f;
		// �ړI�̊p�x�܂Ŋ�����������Y
		m_vAngle.y += fDiffRotY * RATE_ROTATE_CAMERA;
		if (m_vAngle.y >= 180.0f)	m_vAngle.y -= 360.0f;
		if (m_vAngle.y < -180.0f)	m_vAngle.y += 360.0f;
		// �ړI�̊p�x�܂ł̍���Z
		float fDiffRotZ = m_vDestAngle.z - m_vAngle.z;
		if (fDiffRotZ >= 180.0f)	fDiffRotZ -= 360.0f;
		if (fDiffRotZ < -180.0f)	fDiffRotZ += 360.0f;
		// �ړI�̊p�x�܂Ŋ�����������Z
		m_vAngle.z += fDiffRotZ * RATE_ROTATE_CAMERA;
		if (m_vAngle.z >= 180.0f)	m_vAngle.z -= 360.0f;
		if (m_vAngle.z < -180.0f)	m_vAngle.z += 360.0f;

		// ���_���W�ړ�����Z�o
		m_vSrcPos.x = -SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vSrcPos.y = -SinDeg(m_vAngle.z) * m_fLengthInterval;
		m_vSrcPos.z = -CosDeg(m_vAngle.z) * m_fLengthInterval;
		m_vDestPos.x = m_vSrcPos.x;
		m_vDestPos.y = m_vSrcPos.y;
		m_vDestPos.z = m_vSrcPos.z;
		if (m_pPosX)	m_vDestPos.x += *m_pPosX;
		
		//�����_�ɕύX���������ꍇ�ʒu��������
		if (m_pPosY) {
			m_vDestPos.y += *m_pPosY;
		}

#pragma endregion

		// ��Ƃ���I�u�W�F�N�g���������ꍇ
		//�S�[���t���O��
		if (!m_bZoom && m_pPosX)
		{
			m_vDestTarget.x = *m_pPosX;

			// ���_�����X�Ɉړ���ɋ߂Â���
			m_vPos.x = m_vPos.x * 0.9f + m_vDestPos.x * 0.1f;

			// �����_�����X�Ɉړ���ɋ߂Â���
			m_vTarget.x = m_vTarget.x * 0.9f + m_vDestTarget.x * 0.1f;

		}

		// ��Ƃ���I�u�W�F�N�g���������ꍇ
		if (m_pPosY) {
			m_vDestTarget.y = *m_pPosY;

			// ���_�����X�Ɉړ���ɋ߂Â���
			m_vPos.y = m_vPos.y * 0.9f + m_vDestPos.y * 0.1f;

			// �����_�����X�Ɉړ���ɋ߂Â���
			m_vTarget.y = m_vTarget.y * 0.9f + m_vDestTarget.y * 0.1f;



		}

		// 2022/01/16 �J�����@�Y�[��
		if (m_bZoom)
		{
			// �v���C���[���̎擾
			auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>();

			// �ړ��ړI���W
			m_vDestPos.x = Player->Pos.x;
			m_vDestPos.y = Player->Pos.y + 70.0f;
			m_vDestPos.z = GOAL_POS_Z;

			// �����_�ړI���W
			m_vDestTarget = Player->Pos;
			m_vDestTarget.y += 50.0f;	// �v���C���[��菭����ɐݒ�

			//2022/01/16
			m_vPos.x = m_vPos.x * 0.95f + m_vDestPos.x * 0.05f;
			m_vPos.y = m_vPos.y * 0.95f + m_vDestPos.y * 0.05f;
			m_vPos.z = m_vPos.z * 0.95f + m_vDestPos.z * 0.05f;
			m_vTarget.x = m_vTarget.x * 0.95f + m_vDestTarget.x * 0.05f;
			m_vTarget.y = m_vTarget.y * 0.95f + m_vDestTarget.y * 0.05f;
		}


		// x���̊�Ƃ���I�u�W�F�N�g���ݒ肳��Ă��Ȃ� && �v���C���[���X�e�[�W�[�ɂ��Ȃ��ꍇ
		if (!m_bZoom && GetAxisX() == nullptr && (m_vPos.x >= 0.0f || m_Limit.y <= m_vPos.x))
		{
			//	�v���C���[�ɒ����_�����킹��
			auto player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
			if (player)	SetAxisX(&player->GetComponent<CTransform>()->Pos.x);

		}

		// y���A�X�e�[�W���Ƃ̏���≺���𒴂��Ȃ�����v���C���[�ɒǏ]��������
		// 2022/7/26���݂͏����͑��݂��Ȃ�/**�߂���mistake**
		if (1) {
			//�v���C���[�ɒ����_�����킹��
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
			m_vPos.x += 2.0f;		//	�J�������W
			m_vTarget.x += 2.0f;	//	�����_
		}
		else if (pInput->GetKeyPress(DIK_LEFT))
		{
			m_vPos.x -= 2.0f;
			m_vTarget.x -= 2.0f;
		}
	}

	//	�J�����̈ړ����E
	if (!m_bZoom && m_vPos.x < m_Limit.x)
	{
		SetAxisX(nullptr);		//	�����_�̐ݒ������
		m_vPos.x = m_Limit.x;
		m_vTarget.x = m_Limit.x;

		// �X�N���[�����~�߂�
		if (SceneManager::Instance()->GetScene() != SCENE_ENDING)
		{
			// �X�N���[�����~�߂�
			CTexScroll::Scroll(false);
		}
	}
	else if (!m_bZoom && m_vPos.x > m_Limit.y)
	{
		SetAxisX(nullptr);		//	�����_�̐ݒ������
		m_vPos.x = m_Limit.y;
		m_vTarget.x = m_Limit.y;

		// �X�N���[�����~�߂�
		if (SceneManager::Instance()->GetScene() != SCENE_ENDING)
		{
			CTexScroll::Scroll(false);
		}
	}
	else
	{
		// �X�N���[�����ĊJ
		CTexScroll::Scroll(true);
	}

	// �}�g���b�N�X�ɍX�V
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

// �r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
void CCamera::UpdateMatrix()
{
	//	�r���[�}�g���b�N�X�X�V
	XMStoreFloat4x4(&m_mtxView, XMMatrixLookAtLH(
		XMLoadFloat3(&m_vPos), XMLoadFloat3(&m_vTarget), XMLoadFloat3(&m_vUp)));

	//	�ˉe�s��X�V
	XMStoreFloat4x4(&m_mtxProj, XMMatrixPerspectiveFovLH(
		XMConvertToRadians(m_fFovY), m_fAspectRatio, m_fNearZ, m_fFarZ));
}

// ���[���h �}�g���b�N�X�ݒ�
void CCamera::SetWorldMatrix(XMFLOAT4X4& mtxWorld)
{
	m_mtxWorld = mtxWorld;
	m_vPos = XMFLOAT3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	m_vTarget = XMFLOAT3(mtxWorld._41 + mtxWorld._31, mtxWorld._42 + mtxWorld._32, mtxWorld._43 + mtxWorld._33);
	m_vUp = XMFLOAT3(mtxWorld._21, mtxWorld._22, mtxWorld._23);
}

// ���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
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


// �J�������W�̐ݒ�
void CCamera::SetPos(XMFLOAT3& vPos)
{
	m_vPos = vPos;
}
void CCamera::SetPos(float x, float y, float z)
{
	m_vPos.x = x, m_vPos.y = y, m_vPos.z = z;
}

// �J�������W�̎擾
XMFLOAT3& CCamera::GetPos()
{
	return m_vPos;
}

// �����_���W�̐ݒ�
void CCamera::SetTarget(XMFLOAT3& vTarget)
{
	m_vTarget = vTarget;
}
void CCamera::SetTarget(float x, float y, float z)
{
	m_vTarget.x = x, m_vTarget.y = y, m_vTarget.z = z;
}

// �A�b�v�x�N�g���̐ݒ�
void CCamera::SetUpVector(XMFLOAT3& vUp)
{
	m_vUp = vUp;
}
void CCamera::SetUpVector(float x, float y, float z)
{
	m_vUp.x = x, m_vUp.y = y, m_vUp.z = z;
}

//	���[���h�}�g���b�N�X�̎擾
XMFLOAT4X4& CCamera::GetWorldMatrix()
{
	return m_mtxWorld;
}

//	�H�H
void CCamera::SetFovY(float fFovY)
{
	m_fFovY = fFovY;
}

//	�A�X�y�N�g��̐ݒ�
void CCamera::SetAspectRatio(float fAspect)
{
	m_fAspectRatio = fAspect;
}

//	�j�A�N���b�v�ƃt�@�[�N���b�v�̐ݒ�
void CCamera::SetRangeZ(float fNearZ, float fFarZ)
{
	m_fNearZ = fNearZ, m_fFarZ = fFarZ;
}

//	�r���[		 �}�g���b�N�X�̎擾
XMFLOAT4X4& CCamera::GetViewMatrix()
{
	return m_mtxView;
}

//	�v���W�F�N�V���� �}�g���b�N�X�̎擾
XMFLOAT4X4& CCamera::GetProjMatrix()
{
	return m_mtxProj;
}

//	�J�����̌X���擾
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

// ����p
float CCamera::GetFOV() const
{
	// 2022/01/18	�p�x���烉�W�A���ɕς���
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


//	�C���X�^���X�擾
CCamera* CCamera::Get()
{
	return m_pCamera;
}

//	�J�����̐ݒ�
void CCamera::Set(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

// fps,tps
//void CCamera::SetTransform(CTransform* transform)
//{
//	m_pTransform = transform;
//}

// �J������X���𑼂̃I�u�W�F�N�g�ˑ��ɂ���
void CCamera::SetAxisX(float* px)
{
	m_pPosX = px;
	if (px)	m_vPos.x = *px;
	else return;

	m_vDestPos.x = *px;
}

/**
* @fn		CCamera::SetAxisY
* @brief	�J������Y���𑼂̃I�u�W�F�N�g�ˑ�������
* @param	(float*)	���I�u�W�F�N�g��Y���W
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