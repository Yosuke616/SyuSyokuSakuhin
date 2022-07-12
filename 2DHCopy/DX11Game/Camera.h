//=============================================================================
//
// �J���� �N���X [Camera.h]
// Author : SHIMIZU YOSUKE
//
//=============================================================================
#pragma once
#include "mainApp.h"

class CTransform;

#define GOAL_POS_Z	(-200.0f)

class CCamera
{
private:
	//CTransform* m_pTransform;
	float* m_pPosX;
	bool m_bZoom;

	XMFLOAT3 m_vPos;				// ���_
	XMFLOAT3 m_vTarget;				// �����_
	XMFLOAT3 m_vUp;					// ����x�N�g��
	XMFLOAT2 m_Limit;

	XMFLOAT3 m_vSrcPos;				// ���̎��_

	XMFLOAT3 m_vDestPos;			// ���_�̖ړI�ʒu
	XMFLOAT3 m_vDestTarget;			// �����_�̖ړI�ʒu
	XMFLOAT3 m_vVelocity;			// �ړ���

	float m_fAspectRatio;			// �c����
	float m_fFovY;					// ����p(Degree)
	float m_fNearZ;					// �O���N���b�v����
	float m_fFarZ;					// ����N���b�v����

	XMFLOAT4X4 m_mtxWorld;			// ���[���h �}�g���b�N�X
	XMFLOAT4X4 m_mtxView;			// �r���[ �}�g���b�N�X
	XMFLOAT4X4 m_mtxProj;			// �v���W�F�N�V���� �}�g���b�N�X

	//2022/01/08
	XMFLOAT2 m_vScrSize;	// ��ʃT�C�Y

	XMFLOAT3 m_vAngle;				// �J�����̊p�x
	XMFLOAT3 m_vDestAngle;			// �J�����̖ړI�̌���
	XMFLOAT3 m_vDestTargetAngle;	// �����_�̖ړI�̌���
	float m_fLengthInterval;		// �J�����̎��_�ƒ����_�̋���

	static CCamera* m_pCamera;		// ���݂̃J����

public:
	CCamera();						//	�R���X�g���N�^
	void Init();					//	������
	void Update();					//	�X�V
	void Draw();

	XMFLOAT4X4& CalcWorldMatrix();					//	���_/�����_/����x�N�g�����烏�[���h �}�g���b�N�X�Z�o
	void UpdateMatrix();							//	�r���[/�v���W�F�N�V���� �}�g���b�N�X�X�V
	//setter
	void SetPos(XMFLOAT3& vPos);					//	�J�������W�̐ݒ�
	void SetPos(float x, float y, float z);			//	�J�������W�̐ݒ�
	void SetTarget(XMFLOAT3& vTarget);				//	�����_�̐ݒ�
	void SetTarget(float x, float y, float z);		//	�����_�̐ݒ�
	void SetUpVector(XMFLOAT3& vUp);				//	�A�b�v�x�N�g���̐ݒ�
	void SetUpVector(float x, float y, float z);	//	�A�b�v�x�N�g���̐ݒ�
	void SetWorldMatrix(XMFLOAT4X4& mtxWorld);		//	���[���h�}�g���b�N�X�̐ݒ�
	void SetFovY(float fFovY);						//	�H�H
	void SetAspectRatio(float fAspect);				//	�A�X�y�N�g��̐ݒ�
	void SetRangeZ(float fNearZ, float fFarZ);		//	�j�A�N���b�v�ƃt�@�[�N���b�v�̐ݒ�
	void SetLimit(XMFLOAT2 limit);
	void SetDestPos(float CameraposZ);
	//getter
	XMFLOAT3& GetPos();								//	�J�������W�̎擾
	XMFLOAT3& GetAngle();							//	�J�����̌X���擾
	XMFLOAT4X4& GetWorldMatrix();					//	���[���h		 �}�g���b�N�X�̎擾
	XMFLOAT4X4& GetViewMatrix();					//	�r���[		 �}�g���b�N�X�̎擾
	XMFLOAT4X4& GetProjMatrix();					//	�v���W�F�N�V���� �}�g���b�N�X�̎擾
	float* GetAxisX();
	// Effekseer
	const XMFLOAT3& GetLook() const;
	const XMFLOAT3& GetUp() const;
	float GetAspect() const;
	float GetNearClip() const;
	float GetFarClip() const;
	float GetFOV() const;
	CCamera& operator=(const CCamera& camera);

	static CCamera* Get();							//	�C���X�^���X�擾
	static void Set(CCamera* pCamera);				//	���݂̃J�����̐ݒ�

	//void SetTransform(CTransform* transform);	//	x,y,z�Œ�
	void SetAxisX(float* px);

};
