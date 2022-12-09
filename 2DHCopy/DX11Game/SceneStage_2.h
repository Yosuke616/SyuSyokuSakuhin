/**
* @file		SceneStage_2.h
* @brief	�X�e�[�W1�̍쐬
* @author	�u���z�S
* @date		2022/11/25
*/

/**�C���N���[�h�K�[�h**/
#ifndef _STAGE_2_H_
#define _STAGE_2_H_

/**�C���N���[�h��**/
#include "Scene.h"
#include "mainApp.h"
#include "Object.h"

/**�N���X��`**/
class SceneStage_2 :public Scene {
private:
	//�����o�ϐ�
	/** @brief �v���C���[�̃|�C���^��ۑ�����ϐ�*/
	Object* m_pPlayer;
	/** @brief �C���X�^���X*/
	static SceneStage_2* m_pInstance;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �C���X�^���X����*/
	static SceneStage_2* GetInstance();
	/** @brief �C���X�^���X�̔j��*/
	static void Destroy();
	/** @brief �R���X�g���N�^*/
	SceneStage_2();
	/** @brief �f�X�g���N�^*/
	~SceneStage_2()override;
	/** @brief ������*/
	void Init()override;
	/** @brief �I������*/
	void Uninit()override;
	/** @brief �X�V����*/
	void Update()override;
	/** @brief �`��*/
	void Draw() override;

};

#endif