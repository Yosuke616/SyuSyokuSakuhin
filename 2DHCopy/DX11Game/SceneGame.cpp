//�C���N���[�h��
#include "sceneGame.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TexScrollComponent.h"
#include "Draw3dComponent.h"

//�ÓI�����o�ϐ�
SceneGame* SceneGame::m_pInstance = nullptr;

/**
* @fn		SceneGame::SceneGame
* @brief	�R���X�g���N�^
* @detail	�ǉ��������X�e�[�W���͂����Œǉ����Ă���
*/
SceneGame::SceneGame()
	:m_bPauseMode(false), m_bEditMode(false) {

	//�����ŃX�e�[�W�����X�g�ɒǉ����Ă���
	//ex).m_GameSceneList.push_back(new ??????());

	//�ŏ��̃X�e�[�W�̐ݒ�

}

/**
* @fn		SceneGame::~SceneGame
* @brief	�f�X�g���N�^�ŃX�e�[�W���Ԃ�����[��
*/
SceneGame::~SceneGame() {
	//�V�[�����X�g���폜����
	for (auto scene : m_GameSceneList) {
		delete scene;
	}

	//���X�g����ɂ���
	m_GameSceneList.clear();

}

/** 
* @fn		SceneGame::Init
* @brief	������������
*/
void SceneGame::Init() {
	//�J����������������
	CCamera::Get()->Init();

	//�ҏW���[�h�ƃ|�[�Y�t���O���I�t�ɂ���
	m_bEditMode = false;
	m_bPauseMode = false;

	//�Q�[���I�u�W�F�N�g�̏�����

	//�Ǘ��N���X�C���X�^���X�擾
	m_pObjectManager = ObjectManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GettInstance();
	pModelManager->AddModel(PATH_MODEL_KARIKARI, KARIKARIMODEL);

	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();


	//���f���Ǎ�
	Object* KARI3D = new Object(BLOCK_KARI, UPDATE_PLAYER, DRAW_PLAYER);
	auto BLO = KARI3D->AddComponent<CTransform>();
	auto DRAW_BLO = KARI3D->AddComponent<CDraw3D>();
	DRAW_BLO->SetModel(pModelManager->GetModel(KARIKARIMODEL));

	BLO->SetScale(50.0f, 50.0f, 50.0f);
	BLO->SetVelocity(0.0f, 0.0f, 0.0f);
	BLO->SetPosition(0.0f, 0.0f, 400.0f);
	BLO->SetRotate(0.0f,180.0f,0.0f);
	m_pObjectManager->AddObject(KARI3D);
}

/**
* @fn		SceneGame::Uninti
* @brief	�I������
*/
void SceneGame::Uninit() {
	//�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();
}

/**
* @fn		SceneGame::Update
* @brief	�X�V����
*/
void SceneGame::Update() {
	//�J�����̍X�V����
	CCamera::Get()->Update();

	//�Q�[�����[�h
	if (m_bEditMode == false) {
		if (m_bPauseMode == false) {
			
		}

		//�Q�[���I�u�W�F�N�g�X�V
		m_pObjectManager->Update();

	}
	else if (m_bEditMode == true) {
		m_pObjectManager->UpdateEdit();
	}

#ifdef _DEBUG
	//	�ҏW���[�h�̐؂�ւ�
	if (InputManager::Instance()->GetKeyTrigger(DIK_F12))
	{
		m_bEditMode = m_bEditMode ^ 1;	// �؂�ւ�

		//CSound::Play(SE_PAUSE);	// ������ǉ�

		//	�؂�ւ��Ɠ����ɃJ�����̒����_�̕ύX
		if (m_bEditMode == true)	CCamera::Get()->SetAxisX(nullptr);	//edit	�v���C���[���璍���X���O��
		else						CCamera::Get()->SetAxisX(&ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>()->Pos.x);	//game	�v���C���[�𒍎��X�ɐݒ�
	}
#endif

}

/** 
* @fn		SceneGame::Draw
* @brief	�`�悷��֐�
* @detail	��������������A�㒅��Y��邱�ꑦ����������
*/
void SceneGame::Draw() {
	//�Q�[���I�u�W�F�N�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	SetZBuffer(false);

#ifdef _DEBUG
	//CCamera::Get()->Draw();
#endif
}

/**
* @fn		SceneGame::GetInstance
* @brief	�C���X�^���X�擾
* @return	(SceneGame*)	�C���X�^���X��Ԃ�
*/
SceneGame* SceneGame::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneGame();
	}
	return m_pInstance;
}

/**
* @fn		SceneGame::Destory
* @brief	�C���X�^���X�j��
*/
void SceneGame::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}