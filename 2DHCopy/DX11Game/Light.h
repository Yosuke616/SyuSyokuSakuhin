/**
* @file		Light.h
* @brief	���C�g��ݒ肷�邽�߂̃N���X
* @author	�u���z�S
* @date		2022/7/5
*/

//�C���N���[�h�K�[�h
#ifndef _LIGHT_H_
#define _LIGHT_H_

//�C���N���[�h��
#include "mainApp.h"

//�N���X��`
class CLight
{
	//=== �����o�ϐ� ===
private:
	/** @brief ���ˌ�(�����������Ă���)*/
	XMFLOAT4 m_diffuse;	
	/** @brief ����(�����������ĂȂ����)*/
	XMFLOAT4 m_ambient;		
	/** @brief ���Ȕ���*/
	XMFLOAT4 m_specular;	
	/** @brief ���C�g�̌���*/
	XMFLOAT3 m_direction;
	/** @brief ���C�e�B���O�L��/����*/
	bool m_bEnable;		


	//=== �����o�֐� ===
public:
	/** @brief �R���X�g���N�^*/
	CLight();								
	/** @brief ������*/
	void Init();					
	/** @brief ���ˌ��̎擾*/
	XMFLOAT4& GetDiffuse();					
	/** @brief �����̎擾*/
	XMFLOAT4& GetAmbient();					
	/** @brief ���Ȕ����̎擾*/
	XMFLOAT4& GetSpecular();				
	/** @brief ���������擾*/
	XMFLOAT3& GetDir();						
	/** @brief ���C�gON*/
	void SetEnable(bool bEnable = true);	
	/** @brief ���C�gOFF*/
	void SetDisable(bool bDisable = true);	

	/** @brief ���C�g�̃C���X�^���X�擾*/
	static CLight* Get();					
};

#endif