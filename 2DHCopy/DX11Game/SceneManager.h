/*
	�u���z�S
	2022/6/8�@����J�n
	�ǂ̃V�[�����X�V���ĕ`�悷�邩���Ǘ�����N���X

*/

//�C���N���[�h�K�[�h
#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

//�C���N���[�h��
#include "mainApp.h"
#include "Scene.h"

#include <vector>

//�񋓑��錾(�V�[�������ɔ������g�͑��₵�Ă���)
enum EScene {
	SCENE_TITLE = 0,

	MAX_SCENE
};

//�N���X��`
class SceneManager {
private:
	//�����o�ϐ�
	static SceneManager* m_pInstance;		//�C���X�^���X�����p
	std::vector<Scene*> m_pSceneList;		//�V�[�����X�g(�^�C�g���Ƃ��Q�[����ʂƂ�)
	Scene* m_CurrentScene;					//���݂̃V�[��
	EScene m_eSceneState;					//�ǂ̃V�[�����I�΂�Ă��邩�Ƃ��𑗂�����Z�b�g�����肷�邽��


	//�����o�֐�

protected:
	SceneManager();						//	�R���X�g���N�^

	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	static SceneManager* Instance();	//	�C���X�^���X��
	static void Destroy();				//	�C���X�^���X�̍폜

	~SceneManager();					//	�f�X�g���N�^
	void Init();						//	������
	void Update();						//	�X�V
	void Draw();						//	�`��
	void Uninit();						//	�I��

	void SetScene(EScene NextScene);	//	�V�[���؂�ւ�
	EScene GetScene() const;			//	�V�[�����̎擾

};


#endif