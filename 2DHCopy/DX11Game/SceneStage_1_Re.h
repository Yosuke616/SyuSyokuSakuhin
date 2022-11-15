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
#include "Object.h"

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
	/** @brief ���ꓖ���蔻���ۑ�����ϐ�*/
	std::list<Object*> m_EventList;
	/** @brief ���D���擾�������ǂ����̃t���O*/
	bool m_bOhuda;
	/** @brief �{�X��|���Ă��邩�ǂ����̃t���O*/
	bool m_bBossBreak;
	/** @brief ��x�����u���b�N���󂷃t���O*/
	bool m_bBreak;
	/** @brief �{�X��HP�̊Ǘ�*/
	int m_nBossHP;
	/** @brief �S�[���t���O�ɐG�ꂽ���̔���*/
	bool m_bGoalFlg;
	/** @brief �C�x���g�����蔻��ɓ��������ǂ����̃t���O*/
	bool m_bEventTimeOut;
	/** @brief �N���A�܂ł̗���*/
	bool m_bEventGoal;
	/** @brief ��x�����I�u�W�F�N�g���`�F���W����t���O*/
	bool m_bFirstChange;
	/** @brief �N���A�Ɏ���܂ł̕b��*/
	int m_nTime;

	//�����o�֐�
	/** @brief �^�C���A�E�g�p�̕������*/
	void CreateTimeOut();
	/** @brief �^�C���A�E�g�𓮂���*/
	void EventTimeOut();
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

	/** @brief �I�u�W�F�N�g�̕`���ς��邽�߂̕ϐ�*/
	void ChangeObject();
	/** @brief ���ꓖ���蔻���ۑ����郊�X�g*/
	void SetBaseInfo(std::list<Object*>);
	/** @brief �S�[���p�̓����蔻��*/
	void GoalColl();
	/** @brief �C�x���g�p���X�g�ɒ��g�������Ă��������*/
	void DeleteList();
	/** @brief �C�x���g�Ƃ̓����蔻��̌v�Z*/
	bool CollPlayer(Object* obj);
	/** @brief ���D���擾�������̏��𑗂�*/
	bool GetOhuda();
	/** @brief ���D���擾��������ݒ肷��*/
	void SetOhuda(bool bOhuda);

	/** @brief �{�X��HP��ݒ肷�邽�߂̊֐�*/
	void SetBossHP();
	/** @brief �{�X��HP���擾���邽�߂̊֐�*/
	int GetBossHP();
};

#endif