/**
* @file		MosaicRollComponent.h
* @brief	���U�C�N�Ń��[�����܂�
* @author	�u���z�S
* @date		2022/9/28
*/

/**�C���N���[�h�K�[�h**/
#ifndef _MOSAICROLLCOMPONENT_H_
#define _MOSAICROLLCOMPONENT_H_

/**�C���N���[�h��**/
#include "Scene.h"
#include <vector>

/**�N���X��`**/
/**
* @brief	���U�C�N�ł������ĕς���
* @detail	��ʑS�ẴI�u�W�F�N�g�̌����ڂ�ς���^�C�~���O�ŌĂ�
*/
class MosaicRoll {
private:
	//�����o�ϐ�
	/** @brief �X���b�h�n���h��*/
	static HANDLE m_handle;
	/** @brief ���[�h�����ǂ���*/
	static bool m_bLoading;
	/** @brief ���[�h���Ԃ̃J�E���^*/
	static float m_fLoadCount;
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
	MosaicRoll();
	/** @brief �f�X�g���N�^*/
	~MosaicRoll();

	/** @brief ���[�h��ʂ̃X���b�h*/
	static void Begin();
	/** @brief ���[�h��ʂ͏I���*/
	static void End();
	/** @brief ���[�h��ʂ̃t���[�����擾*/
	static int GetLoadCount();
};

#endif