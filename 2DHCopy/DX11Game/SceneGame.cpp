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
	//ex).m_GameSceneList.push_back(new ??????());
	//m_GameSceneList.push_back();

	//�ŏ��̃X�e�[�W�̐ݒ�
	m_eStageState = STAGE_1;
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
	pModelManager->AddModel(PATH_ROSALINA_X, ROSALINA_MODEL_X);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();

	for (int i = 0;i < 10;i++) {
		//���f���Ǎ�
		Object* KARI3D = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
		auto BLO = KARI3D->AddComponent<CTransform>();
		auto DRAW_BLO = KARI3D->AddComponent<CDraw3D>();
		auto COLL_BLO = KARI3D->AddComponent<CCollider>();
		KARI3D->AddComponent<CSeeColl>();
		DRAW_BLO->SetModel(pModelManager->GetModel(MINT_GREEN_BLOCK_NUM));

		BLO->SetScale(MAPCHIP_WIDTH, MAPCHIP_HEIGHT, MAPCHIP_Z);
		BLO->SetVelocity(0.0f, 0.0f, 0.0f);
		BLO->SetPosition(35 + i * 40, -75.0f , 0.0f);
		BLO->SetRotate(0.0f, 180.0f, 0.0f);
		COLL_BLO->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		m_pObjectManager->AddObject(KARI3D);
	}

	//�V��
	for (int i = 0;i < 10;i++) {
		//���f���Ǎ�
		Object* KARI3D = new Object(BLOCK_NAME, UPDATE_FIELD, DRAW_FIELD);
		auto BLO = KARI3D->AddComponent<CTransform>();
		auto DRAW_BLO = KARI3D->AddComponent<CDraw3D>();
		auto COLL_BLO = KARI3D->AddComponent<CCollider>();
		KARI3D->AddComponent<CSeeColl>();
		DRAW_BLO->SetModel(pModelManager->GetModel(MINT_GREEN_BLOCK_NUM));

		BLO->SetScale(MAPCHIP_WIDTH, MAPCHIP_HEIGHT, MAPCHIP_Z);
		BLO->SetVelocity(0.0f, 0.0f, 0.0f);
		BLO->SetPosition(35 + i * 40, 75.0f + i * 40, 0.0f);
		BLO->SetRotate(0.0f, 180.0f, 0.0f);
		COLL_BLO->SetCollisionSize(BLOCK_COLL_SIZE_X, BLOCK_COLL_SIZE_Y, BLOCK_COLL_SIZE_Z);
		m_pObjectManager->AddObject(KARI3D);
	}

	Object* Rosa = new Object("Rosalina",UPDATE_PLAYER,DRAW_PLAYER);
	auto Rosa_Tra = Rosa->AddComponent<CTransform>();
	auto Rosa_Dra = Rosa->AddComponent<CDraw3D>();

	Rosa_Dra->SetModel(pModelManager->GetModel(ROSALINA_MODEL_X));
	Rosa_Tra->SetScale(15,15,15);
	Rosa_Tra->SetVelocity(0.0f,0.0f,0.0f);
	Rosa_Tra->SetPosition(-15,0);
	m_pObjectManager->AddObject(Rosa);



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
	TransBox->SetPosition(35.0f, 50.0f);
	CollBox->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
	//CollBox->SetOffset(-2.5f, 0.0f);

	DrawBox->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
	DrawBox->SetAnimSplit(3,3);
	DrawBox->SetSwapFrame(3);
	DrawBox->SetLoop(true);

	m_pObjectManager->AddObject(Box);	
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

	//�X�e�[�W�̐ؑ�
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