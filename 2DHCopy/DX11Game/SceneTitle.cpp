//�C���N���[�h��
#include "SceneTitle.h"
#include <stdio.h>

#include "Object.h"
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

#pragma region ---�w�i
	//�I�u�W�F�N�g�̐���
	Object* objBG = new Object("BG",UPDATE_BG,DRAW_BG);
	//�R���|�[�l���g�̒ǉ�
	objBG->AddComponent<CTransform>();
	auto Draw_BG = objBG->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	Draw_BG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
	Draw_BG->SetSize(SCREEN_WIDTH,SCREEN_HEIGHT);
	//���X�g�̒ǉ�
	m_pObjectManager->AddObject(objBG);

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
