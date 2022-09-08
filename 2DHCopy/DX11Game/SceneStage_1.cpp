/**�C���N���[�h��**/
#include "SceneStage_1.h"
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

#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"

/**
* @fn		SceneStage_1::SceneStage_1
* @brief	�R���X�g���N�^�͒Q���Ă���
*/
SceneStage_1::SceneStage_1() {

}

/**	
* @fn		SceneStage_1::~SceneStage_1
* @brief	�f�X�g���N�^�A���O�̕��͎���
*/
SceneStage_1::~SceneStage_1(){

}

/**
* @fn		SceneStage_1
* @brief	����������
*/
void SceneStage_1::Init() {
	//�Ǘ��N���X�̃C���X�^���X�擾
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
	Object* pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

}

/**
* @fn		SceneStage_1
* @brief	�I������
*/
void SceneStage_1::Uninit() {

}

/**
* @fn		SceneStage_1::Update
* @brief	�X�V����
*/
void SceneStage_1::Update() {

}

/**
* @fn		SceneStage_1::Draw
* @brief	�`�揈��
*/
void SceneStage_1::Draw() {

}
