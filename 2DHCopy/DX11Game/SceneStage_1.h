/**
* @file		SceneStage_1.h
* @brief	�X�e�[�W1�̍쐬
* @author	�u���z�S
* @date		2022/7/30
*/

/**�C���N���[�h�K�[�h**/
#ifndef _STAGE_1_H_
#define _STAGE_1_H_

/**�C���N���[�h��**/
#include "Scene.h"
#include "mainApp.h"

/**�N���X��`**/
class SceneStage_1 :public Scene {
private:
	//�����o�ϐ�

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	/** @brief �R���X�g���N�^*/
	SceneStage_1();
	/** @brief �f�X�g���N�^*/
	~SceneStage_1()override;
	/** @brief ������*/
	void Init()override;
	/** @brief �I������*/
	void Uninit()override;
	/** @brief �X�V����*/
	void Update()override;
	/** @brief �`��*/
	void Draw() override;

	//�����o�֐�
};

#endif