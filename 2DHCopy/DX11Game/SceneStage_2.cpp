/**�C���N���[�h��**/
#include "SceneStage_2.h"
#include "imgui.h"

#include "ObjInfo.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "StageManager.h"
#include "Camera.h"

#include "Component.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "Draw3dComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "TexScrollComponent.h"
#include "OutOfRange.h"
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Sound.h"

/**�ÓI�����o�ϐ�**/
SceneStage_2* SceneStage_2::m_pInstance = nullptr;

/**
* @fn		Scenestage_2::GetInstance
* @brief	�C���X�^���X�̎擾�Ɛ���
* @return	(SceneStage_2*)	�X�e�[�W2�̏�񂪓������|�C���^
*/
SceneStage_2* SceneStage_2::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneStage_2();
	}
	return m_pInstance;
}

/**
* @fn		SceneStage_2::Destroy
* @brief	�C���X�^���X�̔j��
*/
void SceneStage_2::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		SceneStage_2::SceneStage_2
* @brief	�ʂɂ��Ȃ��ׂ̈ɃR���X�g���N�^���Ă�󂶂�Ȃ��񂾂�����
*/
SceneStage_2::SceneStage_2() {
	
}

/**
* @fn		SceneStage_2::~SceneStage_2
* @brief	�c���f�X�g���N�^
*/
SceneStage_2::~SceneStage_2() {

}

/**
* @fn		SceneStage_2::Init
* @brief	����������
*/
void SceneStage_2::Init() {
	//�Ǘ��N���X�̎擾
	m_pObjectManager = ObjectManager::GetInstance();

	//�e�N�X�`���Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();
	/*�v���C���[�̃e�N�X�`��*/
	pTexManager->AddTexture(PATH_TEX_DXCHAN_STAND, DXCHAN_STAND_TEX_NUM);

	//���f���Ǎ�
	ModelManager* pModelManager = ModelManager::GetInstance();
	//�u���b�N�ƃ��[�b�^
	pModelManager->AddModel(PATH_WALK_ENEMY, WALK_ENEMY_MODEL_NUM);
	pModelManager->AddModel(PATH_MINT_GREEN_BLOCK, MINT_GREEN_BLOCK_NUM);

	//�v���C���[��Ԃ̎擾
	m_pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	for (int i = 0; i < 2; i++) {
		//�w�i�̕`��
		Object* objBG = new Object("Back", UPDATE_BG, DRAW_BG);
		//�R���|�[�l���g�̒ǉ�
		auto transBG = objBG->AddComponent<CTransform>();
		auto drawBG = objBG->AddComponent<CDraw2D>();
		auto scrollBG = objBG->AddComponent<CTexScroll>();
		//�I�u�W�F�N�g�̒ǉ�
		transBG->SetPosition(-720 * 0.5f + i * 720, 0);
		drawBG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
		drawBG->SetSize(720, 720);
		scrollBG->SetAxizX(&(m_pPlayer->GetComponent<CTransform>()->Pos.x));
		scrollBG->SetScrollValueX(0.0016f);
		//���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG);
	}
	//BGM�Đ�
	CSound::Play(TITLE_BGM);

}

/**
* @fn		SceneStage_2::Uninit
* @brief	�I������
*/
void SceneStage_2::Uninit() {
	CSound::Stop(TITLE_BGM);
}

/**
* @fn		SceneStage_2
* @brief	�X�V����
*/
void SceneStage_2::Update() {

}

/**
* @fn		SceneStage_2::Draw
* @brief	�`�揈��
*/
void SceneStage_2::Draw() {

}