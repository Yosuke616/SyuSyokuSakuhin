//�C���N���[�h��
#include "sceneGame.h"

#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
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
	pModelManager->AddModel(PATH_MODEL_BLOCKKARI, BLOCK_KARIKARI);

	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();

	//for (int i = 0;i < 10;i++) {
		//���f���Ǎ�
		Object* KARI3D = new Object(BLOCK_KARI, UPDATE_FIELD, DRAW_FIELD);
		auto BLO = KARI3D->AddComponent<CTransform>();
		auto DRAW_BLO = KARI3D->AddComponent<CDraw3D>();
		auto COLL_BLO = KARI3D->AddComponent<CCollider>();
		KARI3D->AddComponent<CSeeColl>();
		DRAW_BLO->SetModel(pModelManager->GetModel(BLOCK_KARIKARI));

		BLO->SetScale(10.0f, 10.0f, 10.0f);
		BLO->SetVelocity(0.0f, 0.0f, 0.0f);
		BLO->SetPosition(35, 0.0f, 0.0f);
		BLO->SetRotate(0.0f, 180.0f, 0.0f);
		COLL_BLO->SetCollisionSize(70.0f, 70.0f, 70.0f);
		m_pObjectManager->AddObject(KARI3D);
	//}


	//��
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);

	//�I�u�W�F�N�g�̍쐬
	//��
	Object* Box = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CAnimMesh>();
	auto CollBox = Box->AddComponent<CCollider>();
	Box->AddComponent<CPlayer>();
	//Box->AddComponent<CSeeColl>();
	//�ݒ艼
	TransBox->SetPosition(35.0f, 50.0f);
	CollBox->SetCollisionSize(30.0f, 30.0f, 100.0f);
	CollBox->SetOffset(7.4f, 0.0f);

	DrawBox->SetTexture(pTexManager->GetTexture(DXCHAN_STAND_TEX_NUM));
	DrawBox->SetSize(30.0f, 30.0f);
	//DrawBox->SetAnimSplit(3,3);
	//DrawBox->SetSwapFrame(10);
	DrawBox->SetLoop(true);

	m_pObjectManager->AddObject(Box);

	/*CollBox->SetCollisionSize(350.0f,320.0f,100.0f);
	CollBox->SetOffset(10.0f,75.0f);
	CollBox->SetCollisionSize(10.0f, 60.0f, 100.0f);
	CollBox->SetOffset(7.4f, 0.0f);*/

	//Object* Box2 = new Object(PLAYER_NAME, UPDATE_PLAYER, DRAW_PLAYER);
	//auto TransBox2 = Box2->AddComponent<CTransform>();
	//auto DrawBox2 = Box2->AddComponent<CDraw3D>();
	//auto CollBox2 = Box2->AddComponent<CCollider>();
	//auto PlayBox2 = Box2->AddComponent<CPlayer>();
	//Box2->AddComponent<CSeeColl>();
	//DrawBox2->SetModel(pModelManager->GetModel(BLOCK_KARIKARI));

	////�ݒ艼
	//TransBox2->SetScale(10.0f,10.0f,10.0f);
	//TransBox2->SetPosition(50.0f, 50.0f);
	//TransBox2->SetVelocity(0.0f, 0.0f, 0.0f);
	//TransBox2->SetRotate(0.0f, 180.0f, 0.0f);
	//CollBox2->SetCollisionSize(50.0f, 50.0f, 50.0f);

	//m_pObjectManager->AddObject(Box2);
	
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