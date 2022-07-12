//�C���N���[�h��
#include "Light.h"

//���O���
using namespace DirectX;

// �O���[�o���ϐ�
namespace {
	const XMFLOAT3 LIGHT0_DIRECTION = XMFLOAT3(0.0f, -1.0f, 1.0f);			//	���C�g�̌���(��ʂ̉�������)
	const XMFLOAT4 LIGHT0_DIFFUSE = XMFLOAT4(0.9f, 0.9f, 0.9f, 1.0f);		//	���ˌ�(�����������Ă���)	
	const XMFLOAT4 LIGHT0_AMBIENT = XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f);	//	����(�����������ĂȂ����)	
	const XMFLOAT4 LIGHT0_SPECULAR = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);		//	���Ȕ���	

	//	�C���X�^���X�쐬
	CLight g_light;
};

/**
* @fn		CLight::CLight
* @brief	�R���X�g���N�^�������ł����₢�Ă���
*/
CLight::CLight()
{
	//	������
	Init();
}

/**
* @fn		CLight::Init
* @brief	���������܂�
*/
void CLight::Init()
{
	XMFLOAT3 vDir = LIGHT0_DIRECTION;										//	���C�g�̌���(��ʂ̉�������)
	XMStoreFloat3(&m_direction, XMVector3Normalize(XMLoadFloat3(&vDir)));	//	�P�ʃx�N�g���ɂ��ăX�g�A�ɕۑ�

	m_diffuse = LIGHT0_DIFFUSE;		//	���ˌ�(�����������Ă���)	
	m_ambient = LIGHT0_AMBIENT;		//	����(�����������ĂȂ����)
	m_specular = LIGHT0_SPECULAR;	//	���Ȕ���	
	m_bEnable = true;
}

/**
* @fn		CLight::GetDiffuse
* @brief	���ˌ��̎擾
* @return	(XMFLOAT4&)	���ˌ��̏�񂪓����Ă���A�h���X��Ԃ�
*/
XMFLOAT4& CLight::GetDiffuse()
{
	return m_diffuse;
}

/**
* @fn		CLight::GetAmbient
* @brief	�����̎擾
* @return	(XMFLOAT4&)	�����̏�񂪓����Ă���A�h���X��Ԃ�
*/
XMFLOAT4& CLight::GetAmbient()
{
	return m_ambient;
}

/**
* @fn		CLight::GetSpecular
* @brief	���Ȕ����̎擾
* @return	(XMFLOAT4&)	���Ȕ����̏�񂪓����Ă���A�h���X��Ԃ�
*/
XMFLOAT4& CLight::GetSpecular()
{
	return m_specular;
}


/**
* @fn		CLight::GetDir
* @brief	�����������擾����
* @return	(XMFLOAT3&)	���C�g���I���ɂȂ��Ă����ꍇ���̕�����Ԃ�
*/
XMFLOAT3& CLight::GetDir()
{
	//	���C�gON
	if (m_bEnable) return m_direction;

	//	���C�gOFF
	static XMFLOAT3 off(0.0f, 0.0f, 0.0f);
	return off;
}

/**
* @fn		CLight::Get
* @brief	�����C���X�^���X�̎擾������
* @param	(CLight*)	�C���X�^���X���̃|�C���^��Ԃ�
*/
CLight* CLight::Get()
{
	return &g_light;
}

/**
* @fn		CLight::SetEnable
* @brief	���C�g��ON�ɂ���
* @oaram	(bool)	���C�g��t���邩�ǂ�����
*/
void CLight::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

/**
* @fn		CLight::SetDisable
* @brief	���C�g���I�t�ɂ���
* @param	(bool)	���C�g���������ǂ����@�܂�0
*/
void CLight::SetDisable(bool bDisable)
{
	m_bEnable = !bDisable;
}
