/**
* @file		SceneStage_1_Re.h
* @brief	�X�e�[�W1(Reloaded)�̍쐬
* @author	�u���z�S
* @date		2022/7/30
*/

/**�C���N���[�h�K�[�h**/
#ifndef _STAGE_1_RE_H_
#define _STAGE_1_RE_H_

/**�C���N���[�h��**/
#include "Scene.h"
#include "mainApp.h"

/**�N���X��`**/
/**
* @brief	�X�e�[�W1
* @detail	���\���o�[�Y
*/
class SceneStage_1_Re : public Scene {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X*/
	static SceneStage_1_Re* m_pInstance;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �C���X�^���X����*/
	static SceneStage_1_Re* GetInstance();
	/** @brief �C���X�^���X�̔j��*/
	static void Destroy();
	/** @brief �R���X�g���N�^*/
	SceneStage_1_Re();
	/** @brief �f�X�g���N�^*/
	~SceneStage_1_Re();
	/** @brief ������*/
	void Init() override;
	/** @brief �I������*/
	void Uninit() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;
};

#endif