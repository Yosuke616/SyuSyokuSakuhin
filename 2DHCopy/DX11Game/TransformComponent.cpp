//�C���N���[�h��
#include "TransformComponent.h"

/**
* @fn		CTransform
* @brief	�R���X�g���N�^����������
*/
CTransform::CTransform() {
	//�R���|�[�l���g���X�g�̍X�V�����̐ݒ�
	m_eUpdateOrder = COM_UPDATE_TRANSRATE;

	Pos			= XMFLOAT3(0.0f,0.0f,0.0f);
	Vel			= XMFLOAT3(0.0f,0.0f,0.0f);
	Acc			= XMFLOAT3(0.0f,0.0f,0.0f);
	Scale		= XMFLOAT3(0.0f,0.0f,0.0f);
	Rotate		= XMFLOAT3(0.0f,0.0f,0.0f);
}

/**
* @fn		~CTransform
* @brief	���Ȃ��Ȃ����f�X�g���N�^
*/
CTransform::~CTransform() {

}

/**
* @fn		CTransform::Update
* @breif	�X�V����
*/
void CTransform::Update() {
	//�����x�����Z
	Vel.x += Acc.x;
	Vel.y += Acc.y;
	Vel.z += Acc.z;
	//���W�ɑ��x�����Z
	Pos.x += Vel.x;
	Pos.y += Vel.y;
	Pos.z += Vel.z;
}

/**
* @fn		CTransform::SetPosition
* @brief	���W�̐ݒ�
* @param	(float)		�ݒ肵����x���W
* @param	(float)		�ݒ肵����y���W
* @param	(float)		�ݒ肵����z���W
*/
void CTransform::SetPosition(float fx,float fy,float fz) {
	Pos.x = fx;
	Pos.y = fy;
	Pos.z = fz;
}

/**
* @fn		CTransform::SetVelocity
* @brief	���x�̐ݒ�
* @param	(float)		�ݒ肵�������x(x)
* @param	(float)		�ݒ肵�������x(y)
* @param	(float)		�ݒ肵�������x(z)
*/
void CTransform::SetVelocity(float fx,float fy,float fz) {
	Vel.x = fx;
	Vel.y = fy;
	Vel.z = fz;
}

/**
* @fn		CTransform::SetScale
* @brief	�傫���̐ݒ�
* @param	(float)		�ݒ肵�����傫��(x)
* @param	(float)		�ݒ肵�����傫��(y)
* @param	(float)		�ݒ肵�����傫��(z)
*/
void CTransform::SetScale(float fx,float fy,float fz) {
	Scale.x = fx;
	Scale.y = fy;
	Scale.z = fz;
}

/**
* @fn		CTransform::SetRotate
* @brief	�p�x(��])�̐ݒ�
* @param	(float)		�ݒ肵�����X��(��])(x)
* @param	(float)		�ݒ肵�����X��(��])(y)
* @param	(float)		�ݒ肵�����X��(��])(z)
*/
void CTransform::SetRotate(float fx,float fy,float fz) {
	Rotate.x = fx;
	Rotate.y = fy;
	Rotate.z = fz;
}
