/**
* @file		Load.h
* @brief	���[�h�����ႤƮ��
* @author	�u���z�S
* @date		2022/8/22
*/

/**�C���N���[�h�K�[�h**/
#ifndef _LOAD_H_
#define _LOAD_H_

/**�C���N���[�h��**/
#include "Scene.h"
#include <vector>

/**�N���X��`**/
/**
* @brief  ���[�h��ʗp�N���X
* @detail ��ʓ]���Ŏg������
*/
class Load {
private:
	//�����o�ϐ�
	/** @brief �X���b�h�n���h��*/
	static HANDLE m_handle;
	/** @brief ���[�h�����ǂ���*/
	static bool m_bLoading;
	/** @brief ���[�h���Ԃ̃J�E���^*/
	static int  m_nLoadCount;
	/** @brief ���[�h��ʂ̃I�u�W�F�N�g���X�g*/
	static std::vector<Object*> m_LoadObject;

	//�����o�֐�
	/** @brief ���[�h��ʃI�u�W�F�N�g�̏�����*/
	static void Init();
	/** @brief ���[�h��ʂ̃I�u�W�F�N�g�̏I������*/
	static void Uninit();
	/** @brief ���[�h��ʂ̍X�V�ƕ`��*/
	static unsigned __stdcall LoadScreen();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	Load();
	/** @brief �f�X�g���N�^*/
	~Load();

	/** @brief ���[�h��ʂ̃X���b�h*/
	static void Begin();
	/** @brief ���[�h��ʂ͏I���*/
	static void End();
	/** @brief ���[�h��ʂ̃t���[�����擾*/
	static int GetLoadCount();
};

#endif