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
#include "Object.h"

/**�N���X��`**/
class SceneStage_1 :public Scene {
private:
	//�����o�ϐ�
	/** @brief �v���C���[�̃|�C���^��ۑ�����ϐ�*/
	Object* m_pPlayer;
	/** @brief �C���X�^���X*/
	static SceneStage_1* m_pInstance;
	/** @brief �C�x���g�ɓ��������Ƃ��p�̃t���O*/
	bool m_bEventFlg;
	/** @brief �C�x���g�p�̃I�u�W�F�N�g���i�[����ϐ�*/
	std::list<Object*> m_EventList;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �C���X�^���X����*/
	static SceneStage_1* GetInstance();
	/** @brief �C���X�^���X�̔j��*/
	static void Destroy();
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

	/** @brief �ϐ��̐ݒ肾��[*/
	void SetBaseInfo(std::list<Object*>);
	/** @brief �C�x���g�p���X�g�ɒ��g�������Ă��������*/
	void DeleteList();
	/** @brief �C�x���g�����蔻��ƃv���C���[�Ƃ̓����蔻��̏���*/
	bool CollPlayer(Object* obj);
};

#endif