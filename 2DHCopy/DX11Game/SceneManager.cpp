//�C���N���[�h��
#include "SceneManager.h"

//�V�[������������ǉ�����
#include "SceneTitle.h"

//�ǉ�����Ă������V�[���̏���ǉ����Ă���

//�ÓI�����o�ϐ�
//���߂͑��݂��Ă��Ȃ����
SceneManager* SceneManager::m_pInstance = nullptr;

//�C���X�^���X����
SceneManager* SceneManager::Instance() {
	if (!m_pInstance) {
		//����Ă��Ȃ���Ԃ������炱���ō��
		m_pInstance = new SceneManager();
	}
	return m_pInstance;
}

//�C���X�^���X�̍폜
void SceneManager::Destroy() {
	if (m_pInstance) {
		//�C���X�^���X��������Ă����ꍇ�폜����
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

//�R���X�g���N�^
SceneManager::SceneManager()
	:m_CurrentScene(nullptr) {

}

//�f�X�g���N�^
SceneManager::~SceneManager(){
	//�V�[���̒��g���폜(���X�g�������Ă���)
	for (auto scene : m_pSceneList) {
		delete scene;
	}

	//���X�g����ɂ���
	m_pSceneList.clear();

}

//������
void SceneManager::Init() {
	//�K�v�ȃV�[�������X�g�ɒǉ����Ă���
	m_pSceneList.push_back(new SceneTitle());

	//���߂̃V�[����I������
	SetScene(SCENE_TITLE);

}

//�X�V
void SceneManager::Update(){
	//�V�[���X�V
	//�I�΂�Ă���|�C���^�ɓ����Ă���V�[���̍X�V
	m_CurrentScene->Update();
}

//�`��
void SceneManager::Draw() {
	//�@�V�[���`��
	m_CurrentScene->Draw();

}

//�I��
void SceneManager::Uninit() {
	//�C���X�^���X�̔j��
	SceneManager::Destroy();
}

//�V�[���̓���
EScene SceneManager::GetScene() const {
	return m_eSceneState;
}

//�V�[���̐ؑ�
void SceneManager::SetScene(EScene NextScene) {
	//�@��O����
	if (NextScene >= MAX_SCENE) {
		//�z�肵���V�[�����傫�����ɃG���[���N����
		MessageBox(GetMainWnd(), _T("�V�[���؂�ւ����s"), NULL, MB_OK);
		return;
	}

	//���݂̃V�[�����I������
	if (m_CurrentScene) {
		m_CurrentScene->Uninit();
	}

	//���̃V�[���̏�����������
	m_CurrentScene = m_pSceneList[NextScene];		//���݂̃V�[���̏����X�V����
	m_eSceneState = NextScene;						//���邽�߂̕ϐ����X�V����
	m_CurrentScene->Init();							//�I�΂ꂽ�V�[���̏�����������

}



