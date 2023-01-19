//�C���N���[�h��
#include "SceneTitle.h"
#include <stdio.h>

#include "Object.h"
#include "ObjInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "imgui.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "Draw3dComponent.h"
#include "AnimMeshComponent.h"
#include "DrawMeshComponent.h"
#include "ColliderComponent.h"
#include "TITLEComponent.h"
#include "Sound.h"
#include "SoundData.h"

#include "Load.h"

/**�萔��`**/

//�R���X�g���N�^
SceneTitle::SceneTitle() {

}

//�f�X�g���N�^
SceneTitle::~SceneTitle() {

}

//������
void SceneTitle::Init() {
	//���[�h��ʂ̃X���b�h�����
	Load::Begin();
	
	//�Ǘ��N���X�̃C���X�^���X�̎擾
	m_pObjectManager = ObjectManager::GetInstance();
	m_pMenuManager = MenuManager::GetInsutance();
	TextureManager* pTexManager = TextureManager::GetInstance();
	
	//�w�i�̃e�N�X�`��
	pTexManager->AddTexture(PATH_TEX_TITLE_BACK_GROUND, TITLE_BACK_GROUND_NUM);
	//UI�֌W�̃e�N�X�`�����ǉ�����
	pTexManager->AddTexture(PATH_TEX_TITLE_BUTTON, TITLE_BUTTON_NUM);
	pTexManager->AddTexture(PATH_TEX_BEGIN, BEGIN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CONTINUE, CONTINUE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_END, END_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BGM, BGM_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_SE, SE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_BRIGHTNESS, BRIGHTNESS_TEX_NUM);
	
	//���
	pTexManager->AddTexture(PATH_ARROW, ARROW_NUM);
	pTexManager->AddTexture(PATH_TEX_KARIYOZORA,KARIYOZORA_TEX_NUM);
	
	//�w�i�p�I�u�W�F�N�g
	ModelManager::GetInstance()->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);
	ModelManager::GetInstance()->AddModel(PATH_RARD_BLOCK, RARD_BLOCK_NUM);

	//�^�C�g���p�e�N�X�`���̐ݒ�
	pTexManager->AddTexture(PATH_TEX_EIGHT_TITLE, EIGHT_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_CLOUD_TITLE,CLOUD_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_NO_TITLE,NO_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_MI_TITLE,MI_TITLE_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_GIRL_TEX,GIRL_TITLE_TEX_NUM);

#pragma region ---�w�i
	//�I�u�W�F�N�g�̐���
	Object* objBG = new Object("BG",UPDATE_BG,DRAW_BG);
	//�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	Draw_BG->SetTexture(pTexManager->GetTexture(KARIYOZORA_TEX_NUM));
	Draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//���X�g�̒ǉ�
	m_pObjectManager->AddObject(objBG);

#pragma endregion

	

#pragma region ---�n�ʕ\��
	for (int i = 0; i < 16;i++) {
		//�I�u�W�F�N�g�̐���
		Object* objGround = new Object("Gruond",UPDATE_MODEL,DRAW_MODEL);
		//�R���|�[�l���g�̒ǉ�
		auto Groundtrans = objGround->AddComponent<CTransform>();
		auto GroundDraw = objGround->AddComponent<CDraw3D>();
		//�I�u�W�F�N�g�̐ݒ�
		GroundDraw->SetModel(ModelManager::GetInstance()->GetModel(MINT_GREEN_BLOCK_NUM));
		Groundtrans->SetPosition(-300.0f+(i* 40.0f), -30.0f, 150.0f);
		Groundtrans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
		//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
		ObjectManager::GetInstance()->AddObject(objGround);
	}
#pragma endregion

#pragma region ---�n�ʒ��g
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 16;j++) {
			//�I�u�W�F�N�g�̐���
			Object* objRard = new Object("GroundRard",UPDATE_MODEL,DRAW_MODEL);
			//�R���|�[�l���g�̒ǉ�
			auto RardTrans = objRard->AddComponent<CTransform>();
			auto RardDraw = objRard->AddComponent<CDraw3D>();
			//�I�u�W�F�N�g�̐ݒ�
			RardDraw->SetModel(ModelManager::GetInstance()->GetModel(RARD_BLOCK_NUM));
			RardTrans->SetPosition(-300.0f + (j * 40.0f),-70.0f + (40.0f * -(i)), 150.0f);
			RardTrans->SetScale(BLOCK_SIZE_X, BLOCK_SIZE_Y, BLOCK_SIZE_Z);
			//�I�u�W�F�N�g�}�l�[�W���[
			ObjectManager::GetInstance()->AddObject(objRard);
		}
	}	
#pragma endregion

#pragma region ---�ޏ������
	//�I�u�W�F�N�g�̐���
	Object* Medium = new Object("Miko",UPDATE_PLAYER,DRAW_PLAYER);
	//�R���|�[�l���g�̒ǉ�
	auto TransMedium = Medium->AddComponent<CTransform>();
	auto DrawMedium = Medium->AddComponent<CAnimMesh>();
	Medium->AddComponent<CTITLE>();
	//�I�u�W�F�N�g�̐ݒ�
	DrawMedium->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
	TransMedium->SetPosition(-350.0f,35.0f,150.0f);
	DrawMedium->SetAnimSplit(7,2);
	DrawMedium->SetSwapFrame(3);
	DrawMedium->SetLoop(true);
	//�I�u�W�F�N�g�̒ǉ�
	ObjectManager::GetInstance()->AddObject(Medium);
#pragma endregion 

#pragma region ---�^�C�g��
	//��
	//�I�u�W�F�N�g�̐���
	Object* EIGHT = new Object(TITLE_PURPLE_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto TransEight = EIGHT->AddComponent<CTransform>();
	auto DrawEight = EIGHT->AddComponent<CDraw2D>();
	EIGHT->AddComponent<CTITLE>();
	//�I�u�W�F�N�g�̐ݒ�
	DrawEight->SetTexture(pTexManager->GetTexture(EIGHT_TITLE_TEX_NUM));
	DrawEight->SetSize(TITLE_WHITE_SIZE_X,TITLE_WHITE_SIZE_Y);
	TransEight->SetPosition(-500.0f,225.0f,0.0f);
	//�I�u�W�F�N�g�̒ǉ�
	m_pObjectManager->AddObject(EIGHT);

	//�_
	//�I�u�W�F�N�g
	Object* CLOUD = new Object(TITLE_PURPLE_NAME,UPDATE_UI,DRAW_UI);
	//�R���|�l���g�̒ǉ�
	auto TransCloud = CLOUD->AddComponent<CTransform>();
	auto DrawCloud = CLOUD->AddComponent<CDraw2D>();
	CLOUD->AddComponent<CTITLE>();
	//�I�u�W�F�N�g�̐ݒ�@�@
	DrawCloud->SetTexture(pTexManager->GetTexture(CLOUD_TITLE_TEX_NUM));
	DrawCloud->SetSize(TITLE_WHITE_SIZE_X,TITLE_WHITE_SIZE_Y);
	TransCloud->SetPosition(-370.0f,225.0f,0.0f);
	//�I�u�W�F�N�g�̒ǉ�
	m_pObjectManager->AddObject(CLOUD);

	//�m
	//�I�u�W�F�N�g�̐���
	Object* NO_TITLE = new Object(TITLE_PURPLE_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�l���g�̒ǉ�
	auto TransNO = NO_TITLE->AddComponent<CTransform>();
	auto DrawNO = NO_TITLE->AddComponent<CDraw2D>();
	NO_TITLE->AddComponent<CTITLE>();
	//�I�u�W�F�N�g�̐ݒ�@�@
	DrawNO->SetTexture(pTexManager->GetTexture(NO_TITLE_TEX_NUM));
	DrawNO->SetSize(TITLE_WHITE_SIZE_X, TITLE_WHITE_SIZE_Y);
	TransNO->SetPosition(-275.0f, 175.0f, 0.0f);
	//�I�u�W�F�N�g�̒ǉ�
	m_pObjectManager->AddObject(NO_TITLE);

	//��
	//�I�u�W�F�N�g�̐���
	Object* MI_TITLE = new Object(TITLE_WHITE_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�l���g�̒ǉ�
	auto TransMI = MI_TITLE->AddComponent<CTransform>();
	auto DrawMI = MI_TITLE->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�@�@
	DrawMI->SetTexture(pTexManager->GetTexture(MI_TITLE_TEX_NUM));
	DrawMI->SetSize(TITLE_WHITE_SIZE_X, TITLE_WHITE_SIZE_Y);
	TransMI->SetPosition(-150.0f, 225.0f, 0.0f);
	//�I�u�W�F�N�g�̒ǉ�
	m_pObjectManager->AddObject(MI_TITLE);

	//��
	//�I�u�W�F�N�g�̐���
	Object* GIRL_TITLE = new Object(TITLE_WHITE_NAME, UPDATE_UI, DRAW_UI);
	//�R���|�l���g�̒ǉ�
	auto TransGirl = GIRL_TITLE->AddComponent<CTransform>();
	auto DrawGirl = GIRL_TITLE->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�@�@
	DrawGirl->SetTexture(pTexManager->GetTexture(GIRL_TITLE_TEX_NUM));
	DrawGirl->SetSize(TITLE_WHITE_SIZE_X, TITLE_WHITE_SIZE_Y);
	TransGirl->SetPosition(-20.0f, 225.0f, 0.0f);
	//�I�u�W�F�N�g�̒ǉ�
	m_pObjectManager->AddObject(GIRL_TITLE);

#pragma endregion
	
#pragma region ---�J�n�n�_�ƏI���n�_
	//�I�u�W�F�N�g�̐���(�I���n�_)
	Object* objEnd_TITLE = new Object("TITLE_End_Pos", UPDATE_DEBUG, DRAW_DEBUG);
	//�R���|�[�l���g�̒ǉ�
	auto objEnd_Trans = objEnd_TITLE->AddComponent<CTransform>();
	auto objEnd_Draw = objEnd_TITLE->AddComponent<CAnimMesh>();
	objEnd_TITLE->AddComponent<CTITLE>();
	//�I�u�W�F�N�g�̐ݒ�
	objEnd_Draw->SetColor(1.0f, 1.0f, 0.0f);
	objEnd_Trans->SetPosition(0.0f, -100.0f);
	objEnd_Draw->SetSize(10.0f, 10.0f);
	objEnd_Draw->SetLoop(true);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	ObjectManager::GetInstance()->AddObject(objEnd_TITLE);

	//�I�u�W�F�N�g�̐���(�J�n�n�_)
	Object* objStart = new Object("TITLE_Start_Pos",UPDATE_DEBUG,DRAW_DEBUG);
	//�R���|�[�l���g�̒ǉ�
	auto objStart_Trans = objStart->AddComponent<CTransform>();
	auto objStart_Draw = objStart->AddComponent<CAnimMesh>();
	objStart->AddComponent<CTITLE>();
	//�I�u�W�F�N�g�̐ݒ�
	objStart_Draw->SetColor(1.0f,1.0f,0.0f);
	objStart_Trans->SetPosition(0.0f,250.0f);
	objStart_Draw->SetSize(10.0f,10.0f);
	objStart_Draw->SetLoop(true);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	ObjectManager::GetInstance()->AddObject(objStart);
#pragma endregion

	//���j���[�̍쐬
	m_pMenuManager->CreateTitleMenu();
	
	CSound::Play(TITLE_BGM);

	//���[�h�I������
	Load::End();
}

//�I������
void SceneTitle::Uninit() {
	//���y���~�߂�
	CSound::Stop(TITLE_BGM);

	//���j���[�̏I��
	m_pMenuManager->Destroy();

	//�I�u�W�F�N�g���X�g�̏I������
	m_pObjectManager->Uninit();
}

//�X�V����
void SceneTitle::Update() {
	//�I�u�W�F�N�g���X�g�̍X�V
	m_pObjectManager->Update();

	//���j���[��ʂ̍X�V
	m_pMenuManager->Update();
}

//�`�揈��
void SceneTitle::Draw(){
	//�I�u�W�F�N�g���X�g�̕`��
	m_pObjectManager->Draw();

	//���j���[��ʂ̍X�V
	m_pMenuManager->Draw();
}
