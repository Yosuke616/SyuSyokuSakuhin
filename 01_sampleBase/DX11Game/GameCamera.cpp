//=============================================================================
//
// �Q�[�� �J���� �N���X [GameCamera.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "GameCamera.h"
#include "Input.h"
#include "DebugProc.h"
#include "Model.h"

// �O���[�o���ϐ�
namespace {
	const float VALUE_MOVE_CAMERA = 0.5f;			// �J�����ړ���
	const float VALUE_ROTATE_CAMERA = 0.5f;			// �J������]��
}

//������
void CGameCamera::Init() {
	CCamera::Init();
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// �X�V
void CGameCamera::Update()
{
	if (CInput::GetKeyPress(VK_A)) {
		if (CInput::GetKeyPress(VK_W)) {
			// ���O�ړ�
			m_vPos.x -= CosDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z += SinDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
		} else if (CInput::GetKeyPress(VK_S)) {
			// ����ړ�
			m_vPos.x -= CosDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z += SinDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
		} else {
			// ���ړ�
			m_vPos.x -= CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
			m_vPos.z += SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	} else if (CInput::GetKeyPress(VK_D)) {
		if (CInput::GetKeyPress(VK_W)) {
			// �E�O�ړ�
			m_vPos.x += CosDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z -= SinDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
		} else if (CInput::GetKeyPress(VK_S)) {
			// �E��ړ�
			m_vPos.x += CosDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z -= SinDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
		} else {
			// �E�ړ�
			m_vPos.x += CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
			m_vPos.z -= SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	} else if (CInput::GetKeyPress(VK_W)) {
		// �O�ړ�
		m_vPos.x += SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		m_vPos.z += CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	} else if (CInput::GetKeyPress(VK_S)) {
		// ��ړ�
		m_vPos.x -= SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		m_vPos.z -= CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	}

	if (CInput::GetKeyPress(VK_Z)) {
		// ���_����u���v
		m_vAngle.y += VALUE_ROTATE_CAMERA;
		if (m_vAngle.y >= 180.0f) {
			m_vAngle.y -= 360.0f;
		}

		m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_C)) {
		// ���_����u�E�v
		m_vAngle.y -= VALUE_ROTATE_CAMERA;
		if (m_vAngle.y < -180.0f) {
			m_vAngle.y += 360.0f;
		}

		m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_Y)) {
		// ���_�ړ��u��v
		m_vPos.y += VALUE_MOVE_CAMERA;
	}
	if (CInput::GetKeyPress(VK_N)) {
		// ���_�ړ��u���v
		m_vPos.y -= VALUE_MOVE_CAMERA;
	}

	if (CInput::GetKeyPress(VK_Q)) {
		// �����_����u���v
		m_vAngle.y -= VALUE_ROTATE_CAMERA;
		if (m_vAngle.y < -180.0f) {
			m_vAngle.y += 360.0f;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_E)) {
		// �����_����u�E�v
		m_vAngle.y += VALUE_ROTATE_CAMERA;
		if (m_vAngle.y >= 180.0f) {
			m_vAngle.y -= 360.0f;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_T)) {
		// �����_�ړ��u��v
		m_vTarget.y += VALUE_MOVE_CAMERA;
	}
	if (CInput::GetKeyPress(VK_B)) {
		// �����_�ړ��u���v
		m_vTarget.y -= VALUE_MOVE_CAMERA;
	}

	if (CInput::GetKeyTrigger(VK_SPACE)) {
		// ���Z�b�g
		Init();
	}

	CDebugProc::Print("[��� �� : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[������ : (%f, %f, %f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("[��� ѷ : (%f)]\n", m_vAngle.y);
	CDebugProc::Print("\n");

	CDebugProc::Print("*** ��� ��� ***\n");
	CDebugProc::Print("ϴ   ��޳ : W\n");
	CDebugProc::Print("���  ��޳ : S\n");
	CDebugProc::Print("���� ��޳ : A\n");
	CDebugProc::Print("з�  ��޳ : D\n");
	CDebugProc::Print("��   ��޳ : Y\n");
	CDebugProc::Print("��   ��޳ : N\n");
	CDebugProc::Print("���� �ݶ� : Z\n");
	CDebugProc::Print("з�  �ݶ� : C\n");
	CDebugProc::Print("\n");

	CDebugProc::Print("*** ������ ��� ***\n");
	CDebugProc::Print("��   ��޳ : T\n");
	CDebugProc::Print("��   ��޳ : B\n");
	CDebugProc::Print("���� �ݶ� : Q\n");
	CDebugProc::Print("з�  �ݶ� : E\n");
	CDebugProc::Print("\n");

	CCamera::Update();
}
