/**�C���N���[�h��**/
#include "SceneSelect.h"

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "ModelManager.h"
#include "imgui.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "FileManager.h"
#include "ResourceCSV.h"
#include "sceneGame.h"
#include "Draw3dComponent.h"
#include "AnimMeshComponent.h"

/**�\���̒�`**/
StageSelect::Row StageSelect::m_StageGrid;

/**
* @fn		StageSelect::StageSelect
* @brief	�R���X�g���N�^�͓�x����
*/
StageSelect::StageSelect() {
	//StageSelect::Load();
}

/**
* @fn		StageSelect::~StageSelect
* @brief	�����A�΂݂���f�X�g���N�^����яo����
*/
StageSelect::~StageSelect() {

}

/**
* @fn		StageSelect::Init()
* @brief	����������
*/
void StageSelect::Init() {
	//���[�h���܂�
	//�i�s�󋵂̓Ǎ�
	//StageSelect::Load();

	//�Ǘ��N���X�擾
	m_pObjectManager = ObjectManager::GetInstance();
	m_pMenuManager = MenuManager::GetInsutance();
	TextureManager* pTexManager = TextureManager::GetInstance();
	ModelManager* pModelManager = ModelManager::GetInstance();

	//�J�����̏�����
	CCamera::Get()->Init();

	//�e�N�X�`���̓Ǎ�
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DXCHAN_RUN, DXCHAN_RUN_TEX_NUM);
	pTexManager->AddTexture(PATH_TEX_DEBUG_BLOCK, DEBUG_BLOCK_NUM);

	//���f���̓Ǎ�
	pModelManager->AddModel(PATH_ROSALINA_X, ROSALINA_MODEL_X);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//�K�v�ȃI�u�W�F�N�g�̒ǉ�
	//���������ɖ����I�u�W�F�N�g�������ɐ�������	
#pragma region ---�w�i
	//�I�u�W�F�N�g�̐���
	Object* objBG = new Object("BG", UPDATE_BG, DRAW_BG);
	//�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CAnimMesh>();
	//�I�u�W�F�N�g�̐ݒ�
	objBG->GetComponent<CTransform>()->SetPosition(0.0f,0.0f,0.0f);
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
	Draw_BG->SetSize(450, 260);
	Draw_BG->SetColor(0.0f,0.0f,1.0f);
	Draw_BG->SetAnimSplit(1,1);
	Draw_BG->SetLoop(true);
	//���X�g�̒ǉ�
	m_pObjectManager->AddObject(objBG);

#pragma endregion
	//���j���[�̍쐬
	m_pMenuManager->CreateSelectMenu();
}

/**
* @fn		StageSelect::Uninit
* @brief	�I������
*/
void StageSelect::Uninit() {
	//�I�u�W�F�N�g�̏I��
	m_pObjectManager->Uninit();
	//���j���[�̏I��
	m_pMenuManager->DeleteMenu();
}

/**
* @fn		StageSelect::Update
* @brief	�X�V����
*/
void StageSelect::Update(){
	//�I�u�W�F�N�g���X�g�̍X�V
	m_pObjectManager->Update();
	//���j���[�̍X�V
	m_pMenuManager->Update();
}

/** 
* @fn		StageSelect::Draw
* @brief	�`�揈��
*/
void StageSelect::Draw() {
	//�I�u�W�F�N�g���X�g�̕`��
	SetZBuffer(true);
	m_pObjectManager->Draw();
	m_pMenuManager->Draw();
	SetZBuffer(false);
}

/**
* @fn		StageSelect::GetSelectAble
* @brief	�X�e�[�W���I���\���ǂ���������
* @param	(int)	�ԍ��Ŏ��ʂ���
* @return	(bool)	�I���ł��邩�ǂ����̌��ʂ�Ԃ�
*/
//bool StageSelect::GetSelectAble(int stage_num) {
//	return 0;
//}



