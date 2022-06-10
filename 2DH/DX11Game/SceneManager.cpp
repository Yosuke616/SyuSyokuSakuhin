//===== �C���N���[�h =====
#include "SceneManager.h"

//#include "fade.h"
//
#include "TitleScene.h"

//===== �ÓI�����o�ϐ� =====
SceneManager* SceneManager::m_pInstance = nullptr;

// �C���X�^���X����
//��񂾂��ǂݍ��ވׂɃC���X�^���X�����Đ�������
SceneManager* SceneManager::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = new SceneManager();
	}

	return m_pInstance;
}

// �C���X�^���X�̍폜
void SceneManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// �R���X�g���N�^
//���߂̓��X�g�ɃV�[�����ǉ�����Ă��Ȃ��̂Ńk���|�C���^���w���Ȃ��Ƃ����Ȃ�
SceneManager::SceneManager()
	: m_CurrentScene(nullptr)
{
}

// �f�X�g���N�^
SceneManager::~SceneManager()
{
	//���ׂ̈ɂQ��폜����̂��@���ꂪ�킩��Ȃ��@�����܂��傤

	//	�V�[�����X�g���폜
	/*for (auto scene : m_SceneList)
		delete scene;*/

	//	���X�g����ɂ���
	m_SceneList.clear();
}

// ������
void SceneManager::Init()
{
	//	�S�V�[�������X�g�ɒǉ�
	m_SceneList.push_back(new SceneTitle());			//	�^�C�g���V�[���̒ǉ�
	//m_SceneList.push_back(SceneGame::GetInstance());	//	�Q�[���V�[���̒ǉ�
	//m_SceneList.push_back(new SceneSelect());			//	�Z���N�g�V�[���̒ǉ�
	//m_SceneList.push_back(new SceneEnding());			//	�G���f�B���O�̒ǉ�

	//	�ŏ��̓^�C�g����ʂ�
	SetScene(SCENE_TITLE);

	//	�t�F�[�h������
	//InitFade();
}

// �X�V
void SceneManager::Update()
{
	//	�V�[���X�V
	//�I�΂�Ă���V�[���������X�V
	m_CurrentScene->Update();

	//	�t�F�[�h�C��/�A�E�g�X�V
	//UpdateFade();
}

// �`��
void SceneManager::Draw()
{
	//	�V�[���`��
	m_CurrentScene->Draw();
	//	�t�F�[�h�C��/�A�E�g�`��
	//DrawFade();

}

// �I��
void SceneManager::Uninit()
{
	// �t�F�[�h�I������
	//UninitFade();

	//	�C���X�^���X�j��
	SceneManager::Destroy();
}

// �V�[���̓���
EScene SceneManager::GetScene() const
{
	return m_eSceneState;
}

// �V�[���̐؂�ւ�
// �����͕ς������V�[����̏��
void SceneManager::SetScene(EScene NextScene)
{
	//	��O����
	if (NextScene >= SCENE_MAX)
	{
		MessageBox(GetMainWnd(), _T("�V�[���؂�ւ����s"), NULL, MB_OK);
		return;
	}

	//	�O�V�[���̏I������
	//�I�΂�Ă���V�[���̏I�������������Ă���
	if (m_CurrentScene)
		m_CurrentScene->Uninit();

	//	���݃V�[���̊J�n
	m_CurrentScene = m_SceneList[NextScene];	//	�V�[���؂�ւ�
	m_eSceneState = NextScene;					//	�V�[����Ԃ̐ݒ�
	m_CurrentScene->Init();						//	������
}