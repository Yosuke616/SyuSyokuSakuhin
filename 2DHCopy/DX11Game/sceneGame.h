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

/**�O���錾**/

//�O���錾
class StageManager;

/*�񋓑̐錾***/
/** @brief �ǂ̃X�e�[�W��ǂݍ��ނ������߂�*/
enum Stage {
	STAGE_1 = 1,

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

};

#endif