//=============================================================================
//
// �v���C���[ �N���X���� [Player.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Player.h"
#include "Input.h"
#include "DebugProc.h"

// �O���[�o���ϐ�
namespace {
	const float VALUE_MOVE_PLAYER = 1.0f;
	const float VALUE_ROTATE_PLAYER = 1.0f;
}

// �R���X�g���N�^
CPlayer::CPlayer(CScene* pScene) : CModel(pScene)
{
}

// �f�X�g���N�^
CPlayer::~CPlayer()
{
}

// ������
HRESULT CPlayer::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_PLAYER);
	return hr;
}

// �I������
void CPlayer::Fin()
{
	CModel::Fin();
}

// �X�V
void CPlayer::Update()
{
	// �J�����̌����擾
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (CInput::GetKeyPress(VK_LEFT)) {
		if (CInput::GetKeyPress(VK_UP)) {
			// ���O�ړ�
			m_vPos.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
		} else if (CInput::GetKeyPress(VK_DOWN)) {
			// ����ړ�
			m_vPos.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
		} else {
			// ���ړ�
			m_vPos.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
		}
	} else if (CInput::GetKeyPress(VK_RIGHT)) {
		if (CInput::GetKeyPress(VK_UP)) {
			// �E�O�ړ�
			m_vPos.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
		} else if (CInput::GetKeyPress(VK_DOWN)) {
			// �E��ړ�
			m_vPos.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
		} else {
			// �E�ړ�
			m_vPos.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
		}
	} else if (CInput::GetKeyPress(VK_UP)) {
		// �O�ړ�
		m_vPos.x -= SinDeg(rotCamera.y + 180.0f) * VALUE_MOVE_PLAYER;
		m_vPos.z -= CosDeg(rotCamera.y + 180.0f) * VALUE_MOVE_PLAYER;
	} else if (CInput::GetKeyPress(VK_DOWN)) {
		// ��ړ�
		m_vPos.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
		m_vPos.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
	}

	if (CInput::GetKeyPress(VK_LSHIFT)) {
		// ����]
		m_vAngle.y -= VALUE_ROTATE_PLAYER;
		if (m_vAngle.y < -180.0f) {
			m_vAngle.y += 360.0f;
		}
	}
	if (CInput::GetKeyPress(VK_RSHIFT)) {
		// �E��]
		m_vAngle.y += VALUE_ROTATE_PLAYER;
		if (m_vAngle.y >= 180.0f) {
			m_vAngle.y -= 360.0f;
		}
	}

	if (CInput::GetKeyPress(VK_RETURN)) {
		// ���Z�b�g
		m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	CDebugProc::Print("[��� �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[��� ѷ : (%f)]\n", m_vAngle.y);
	CDebugProc::Print("\n");

	CDebugProc::Print("*** ��� ��� ***\n");
	CDebugProc::Print("ϴ   ��޳ : \x1e\n");//��
	CDebugProc::Print("���  ��޳ : \x1f\n");//��
	CDebugProc::Print("���� ��޳ : \x1d\n");//��
	CDebugProc::Print("з�  ��޳ : \x1c\n");//��
	CDebugProc::Print("���� �ݶ� : LSHIFT\n");
	CDebugProc::Print("з�  �ݶ� : RSHIFT\n");
	CDebugProc::Print("\n");

	CModel::Update();
}
