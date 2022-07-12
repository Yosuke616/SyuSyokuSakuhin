//�C���N���[�h��
#include "SceneTitle.h"
#include <stdio.h>

#include "Object.h"
#include "ObjectManager.h"
#include "TextureManager.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"

//�R���X�g���N�^
SceneTitle::SceneTitle() {

}

//�f�X�g���N�^
SceneTitle::~SceneTitle() {

}

//������
void SceneTitle::Init() {
	//�Ǘ��N���X�̃C���X�^���X�̎擾
	m_pObjectManager = ObjectManager::GetInstance();
	
	//�e�N�X�`���̓Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();

	//��
	pTexManager->AddTexture(PATH_TEX_KARI, KARI_TEX_NUM);

	//�I�u�W�F�N�g�̍쐬
	//��
	Object* Box = new Object(PLAYER_NAME,UPDATE_PLAYER,DRAW_PLAYER);
	auto TransBox = Box->AddComponent<CTransform>();
	auto DrawBox = Box->AddComponent<CDraw2D>();
	//�ݒ艼
	TransBox->SetPosition(25.0f,25.0f);
	DrawBox->SetTexture(pTexManager->GetTexture(KARI_TEX_NUM));
	DrawBox->SetSize(100.0f,100.0f);
	

	m_pObjectManager->AddObject(Box);
}

//�I������
void SceneTitle::Uninit() {
	//�I�u�W�F�N�g���X�g�̏I������
	m_pObjectManager->Uninit();
}

//�X�V����
void SceneTitle::Update() {
	//�I�u�W�F�N�g���X�g�̍X�V
	m_pObjectManager->Update();
}

//�`�揈��
void SceneTitle::Draw(){
	//�I�u�W�F�N�g���X�g�̕`��
	m_pObjectManager->Draw();
}






