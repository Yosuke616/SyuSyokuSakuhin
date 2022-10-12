/**
* @file		sceneGame.h
* @brief	���̓Q�[���V�[���ł���
* @author	�u���z�S
* @date		2022/6/30
*/

//�C���N���[�h�K�[�h
#ifndef _SCENEGAME_H_
#define _SCENEGAME_H_

//�C���N���[�h
#include "Scene.h"
#include "mainApp.h"
#include "AssimpModel.h"
#include <vector>
#include <unordered_map>

/**�萔��`**/
#define STAGE_TIME	(500)

//�O���錾
class StageManager;

/*�񋓑̐錾***/
/** @brief �ǂ̃X�e�[�W��ǂݍ��ނ������߂�*/
enum Stage {
	STAGE_1 = 0,
	STAGE_1_RE,

	MAX_STAGE
};

//�N���X��`
/**
* @brief		�Q�[���V�[���ׂ̈̃N���X
* @detail		�ǂ̃X�e�[�W��ǂݍ��ށA�Q�[�������邤���ŕK�v�ȏ��݂̂�ǂݍ���
*/
class SceneGame :public Scene {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X*/
	static SceneGame* m_pInstance;
	/** @brief �V�[���̃��X�g(�����ɂǂ�ǂ�X�e�[�W��ǉ����Ă���)*/
	std::vector<Scene*> m_GameSceneList;
	/** @brief ���݂ǂ̃V�[����*/
	Scene* m_CurrentScene;
	/** @brief �ǂ̃X�e�[�W��*/
	Stage m_eStageState;

	/** @brief �������������V�[��*/
	Scene* m_pInitScene;

	/** @brief �X�e�[�W�}�l�[�W���[�����\��*/
	StageManager* m_pStageManager;

	/** @brief �N���A�����Ƃ��ɃZ���N�g��ʂɖ߂��܂ł̎���*/
	int m_nSelectCnt;

	/** @brief �X�e�[�W�̃X�R�A���Ǘ�����ϐ�*/
	int m_nScore;

	/** @brief �X�e�[�W�̕b�����Ǘ�����ϐ�*/
	int m_nTimer;

	/** @brief �����\�𔻒f����ϐ� true�ŕ\�Afalse�ŗ�*/
	bool m_bLoveandLovers;
	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	SceneGame();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	/** @brief �ҏW���[�h*/
	bool m_bEditMode;
	/** @brief ���j���[���*/
	bool m_bPauseMode;


	//�����o�֐�
	/** @brief �C���X�^���X�擾*/
	static SceneGame* GetInstance();
	/** @brief �C���X�^���X�j��*/
	static void Destroy();

	/** @brief �f�X�g���N�^*/
	~SceneGame() override;
	/** @brief ������*/
	void Init() override;
	/** @brief �I������*/
	void Uninit() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;

	/** @brief �V�[���̐ؑ�*/
	void SetStage(Stage NextStage);
	/** @brief �V�[�����̎擾*/
	Stage GetStage();
	/** @brief �|�[�Y�t���O���I���ɂ���*/
	void SetPauseOOO(bool ooo);

	/** @brief �X�R�A���擾����*/
	int GetScore();
	/** @brief �X�R�A�����Z����*/
	void SetScore(int num);

	/** @brief �b�����擾����*/
	int GetTime();
	/** @brief �b����ݒ肷��*/
	void SetTime();
	/** @brief �X�e�[�W�̏�񂪓������x�N�^�[��Ԃ��֐�*/
	std::vector<Scene*> GetCurrentStage();
	/** @brief �����\�����擾����֐�*/
	bool GetLaL();
	/** @brief �����\����r���ŃZ�b�g���邽�߂̊֐�*/
	void SetLaL(bool lal);
	/** @brief ���݂ǂ̃X�e�[�W���ǂ������擾����*/
	Scene* GetStageCurrent();
};

#endif