#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

/*

	�V�[�����Ǘ�����
	�쐬�� : YOSUKE SHIMIZU

	�X�V
	�u���@�쐬�J�n�@2022/6/3
	

	�V�[���ǉ��ɂ���
	�w�b�_�[��EScene�ɃV�[������o�^���AInit�֐����ŃV�[�����X�g�ɒǉ����鏈����������

*/

//===== �C���N���[�h =====
#include "mainApp.h"
#include "Scene.h"

#include <vector>
#include <list>

//===== �񋓑̒�` =====
enum EScene
{
	SCENE_TITLE,		//	�^�C�g�����
	SCENE_GAME,			//	�Q�[�����
	SCENE_SELECT,		//	�Z���N�g���
	SCENE_ENDING,

	SCENE_MAX
};


//===== �N���X��` =====
class SceneManager
{
	//=== �����o�ϐ�
private:
	static SceneManager* m_pInstance;	//	�C���X�^���X�����p
	vector<Scene*>	 m_SceneList;	//	�V�[�����X�g
	Scene* m_CurrentScene;				//	���݂̃V�[��
	EScene m_eSceneState;				//	�V�[���̏��

	//=== �����o�֐�
protected:
	SceneManager();						//	�R���X�g���N�^
public:
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
#endif //__SCENE_MANAGER_H__