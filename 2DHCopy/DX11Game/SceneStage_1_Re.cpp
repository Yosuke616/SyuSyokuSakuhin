/**�C���N���[�h��**/
#include "SceneStage_1_Re.h"
#include "imgui.h"

#include "Object.h"
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
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Sound.h"

/**�ÓI�����o�ϐ�**/
SceneStage_1_Re* SceneStage_1_Re::m_pInstance = nullptr;

/**
* @fn		SceneStage_1_Re::GetInstance
* @brief	�C���X�^���X�𐶐�����
* @return	(SceneStage_1_Re*) �X�e�[�W1(�����[�f�b�h)�̏�񂪓����Ă���|�C���^
*/
SceneStage_1_Re* SceneStage_1_Re::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneStage_1_Re();
	}
	return m_pInstance;
}

/**
* @fn		SceneStage_1_Re::Destroy
* @brief	�C���X�^���X�̔j��
*/
void SceneStage_1_Re::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		SceneStage_1_Re::SceneStage_1_Re
* @brief	�R���X�g���N�^�ɂ��]���j�󂳂ꂽ
*/
SceneStage_1_Re::SceneStage_1_Re() {

}

/**
* @fn		SceneStage_1_Re::~SceneStage_1_Re
* @brief	�f�X�g���N�^�ɂ��]���Đ�����
*/
SceneStage_1_Re::~SceneStage_1_Re() {

}

/**
* @fn		SceneStage_1_Re::Init
* @brief	����������
*/
void SceneStage_1_Re::Init() {
	//�Ǘ��N���X�̃C���X�^���X�擾
	m_pObjectManager = ObjectManager::GetInstance();

	//�e�N�X�`���Ǎ�
	TextureManager* pTexManager = TextureManager::GetInstance();
	pTexManager->AddTexture(PATH_TEX_STAGE_1_RE_BG, STAGE_1_RE_BG_TEX_NUM);

	//���f���Ǎ�
	ModelManager* pModelManager = ModelManager::GetInstance();
	//�u���b�N�̃��f��

	//�v���C���[�̏�Ԃ̎擾
	Object* pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	//�w�i�̕`��
	for (int i = 0;i < 2;i++) {
		//�I�u�W�F�N�g�̐���
		Object* objBG = new Object("Back",UPDATE_BG,DRAW_BG);
		//�R���|�[�l���g�̒ǉ�
		auto transBG = objBG->AddComponent<CTransform>();
		auto drawBG = objBG->AddComponent<CDraw2D>();
		auto scrollBG = objBG->AddComponent<CTexScroll>();
		//�I�u�W�F�N�g�̒ǉ�
		transBG->SetPosition(-720 * 0.5f + i * 720, 0); drawBG->SetTexture(pTexManager->GetTexture(STAGE_1_RE_BG_TEX_NUM));
		drawBG->SetSize(720, 720);
		scrollBG->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));
		scrollBG->SetScrollValueX(0.0016f);
		//���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG);
	}

	//BGM�Đ�
	CSound::Play(TITLE_BGM);

}

/**
* @fn		SceneStage_1_Re::Uninit
* @brief	�I������
*/
void SceneStage_1_Re::Uninit() {
	CSound::Stop(TITLE_BGM);
}

/**
* @fn		SceneStage_1_Re::Update
* @brief	�X�V����
*/
void SceneStage_1_Re::Update() {

}

/**
* @fn		SceneStage_1_Re::Draw
* @brief	�`�揈��
*/
void SceneStage_1_Re::Draw() {

}
