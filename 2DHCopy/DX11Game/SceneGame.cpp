//�C���N���[�h��
#include "sceneGame.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "FileManager.h"
#include "StageManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "Camera.h"
#include "imgui.h"


#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TexScrollComponent.h"
#include "Draw3dComponent.h"
#include "PlayerComponent.h"
#include "SeeCollComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "GravityComponent.h"

//�X�e�[�W�֌W�̃C���N���[�h
#include "SceneStage_1.h"

//�ÓI�����o�ϐ�
SceneGame* SceneGame::m_pInstance = nullptr;

/**
* @fn		SceneGame::SceneGame
* @brief	�R���X�g���N�^
* @detail	�ǉ��������X�e�[�W���͂����Œǉ����Ă���
*/
SceneGame::SceneGame()
	:m_bPauseMode(false), m_bEditMode(false) 
	,m_pStageManager(nullptr){

	//�����ŃX�e�[�W�����X�g�ɒǉ����Ă���
	m_GameSceneList.push_back(new SceneStage_1());
	m_GameSceneList.push_back(new SceneStage_1());

	//�ŏ��̃X�e�[�W�̐ݒ�
	m_eStageState = STAGE_1;
	m_CurrentScene = m_GameSceneList[m_eStageState];
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
	ModelManager* pModelManager = ModelManager::GetInstance();
	m_pStageManager = StageManager::GetInstance();
	pModelManager->AddModel(PATH_ROSALINA_X, ROSALINA_MODEL_X);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();

	//��
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);

	//�I�u�W�F�N�g�̍쐬
	//��
	Object* Box = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	auto CollBox = Box->AddComponent<CCollider>();
	Box->AddComponent<CPlayer>();
	Box->AddComponent<CGravity>();
	Box->AddComponent<CSeeColl>();
	//�ݒ艼
	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	TransBox->SetPosition(35.0f, 150.0f);
	CollBox->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
	//CollBox->SetOffset(-2.5f, 0.0f);

	DrawBox->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
	DrawBox->SetAnimSplit(3,3);
	DrawBox->SetSwapFrame(3);
	DrawBox->SetLoop(true);

	m_pObjectManager->AddObject(Box);	

	//�X�e�[�W�����
	m_pStageManager->CreateStage(m_eStageState);
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

	//�X�e�[�W�X�V
	m_pStageManager->Update();

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

	//�����Ńv���C���[�̑��x������
	//�������̑��x���������Ă����ꍇ��������Ƃ���
	if (m_pObjectManager->GetGameObject(PLAYER_NAME)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		if (Player->GetComponent<CTransform>()->Vel.y <= -MAX_VELOCITY) {
			Player->GetComponent<CPlayer>()->SetPlayerState(FALL_PLAYER);
		}
	}

#ifdef _DEBUG
	//��������̏�����������X�e�[�^�X��ς���
	if (InputManager::Instance()->GetKeyTrigger(DIK_P)) {
		auto Player = m_pObjectManager->GetGameObject(PLAYER_NAME);
		Player->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);
	}

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
* @fn		SceneGame::SetStage
* @brief	�X�e�[�W�̐؂�ւ�
* @param	(Stage)	���̃X�e�[�W�ԍ�
*/
void SceneGame::SetStage(Stage NextStage) {
	//��O����
	if (NextStage >= MAX_STAGE) {
		MessageBox(GetMainWnd(), _T("�X�e�[�W�؂�ւ����s"), NULL, MB_OK);
		return;
	}

	//���݂̃X�e�[�W�̏I��
	if (m_CurrentScene) {
		m_CurrentScene->Uninit();
	}

	//�X�e�[�W�̐ؑ�
	m_CurrentScene = m_GameSceneList[NextStage];
	m_eStageState = NextStage;

}

/** 
* @fn		SceneGame::GetStage	
* @brief	�X�e�[�W�̏����擾����
* @return	(Stage)	���݂̃X�e�[�W�ԍ���Ԃ�
*/
Stage SceneGame::GetStage() {
	return m_eStageState;
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