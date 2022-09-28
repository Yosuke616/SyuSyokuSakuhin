/**�C���N���[�h��**/
#include "SceneStage_1.h"
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
SceneStage_1* SceneStage_1::m_pInstance = nullptr;

/**
* @fn		SceneStage_1::GetInstance
* @brief	�C���X�^���X�擾
* @return	(SceneStage_1*)	�X�e�[�W1�̏�񂪓������|�C���^
*/
SceneStage_1* SceneStage_1::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new SceneStage_1();
	}
	return m_pInstance;
}

/**
* @fn		SceneStage_1::Destroy
* @brief	�C���X�^���X�̔j��
*/
void SceneStage_1::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

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
	m_pPlayer = m_pObjectManager->GetGameObject(PLAYER_NAME);

	for (int i = 0;i < 2;i++) {
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

	//�C�x���g�t���O���I�t�ɂ���
	m_bEventFlg = false;

	//BGM�Đ�
	CSound::Play(TITLE_BGM);

}

/**
* @fn		SceneStage_1
* @brief	�I������
*/
void SceneStage_1::Uninit() {
	CSound::Stop(TITLE_BGM);
}

/**
* @fn		SceneStage_1::Update
* @brief	�X�V����
*/
void SceneStage_1::Update() {
	//���X�g�̃I�u�W�F�N�g�Ƃ̓����蔻��̌v�Z
	for (auto&& obj : m_EventList) {
		//�I�u�W�F�N�g���X�V���邩�ǂ���������
		for (auto&& comp : obj->GetComponentList()) {
			if (comp->m_bUpdateFlag) {
				//��ʊO���ǂ������ׂ��̓X�L�b�v
				if (comp == obj->GetComponent<COutOfRange>()) {
					continue;
				}
				//�����蔻��p�̊֐����Ă�
				if (CollPlayer(obj)) {
					//��肽������
					//�v���C���[���~�X��Ԃɂ���
					//�X�e�[�W�̓Ǎ����ς���
					m_bEventFlg = true;
					SceneGame::GetInstance()->SetStage(STAGE_1_RE);
					auto player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CPlayer>();
					player->SetPlayerState(MISS_PLAYER);
				}
				break;
			}
		}
	}
}

/**
* @fn		SceneStage_1::Draw
* @brief	�`�揈��
*/
void SceneStage_1::Draw() {

}

/**
* @fn		SceneStage_1::SetBaseInfo
* @brief	�C�x���g�̗p�����蔻��̃|�C���^��ۑ�����
			���X�g�ɕۑ�����B
* @detail	�X�e�[�W�}�l�[�W���[�̏��������I������i�K�ŌĂ�
* @param	(list<Object*>)	���X�g�����ĕK�v�Ȃ��̂����܂��ʂ̃��X�g�ɓ����	
*/
void SceneStage_1::SetBaseInfo(std::list<Object*> list) {
	for (auto&& obj : list) {
		if (obj->GetName() == STAGE_1_MISS_EVENT) {
			m_EventList.push_back(obj);
		}
	}
}

/**
* @fn		SceneStage_1::DeleteList
* @brief	�C�x���g���X�g�ɒ��g���������ꍇ�폜����
*/
void SceneStage_1::DeleteList() {
	if (m_EventList.size()) {
		m_EventList.clear();
	}
}

/**
* @fn		SceneStage_1::CollPlayer
* @brief	�v���C���[�Ƃ̓����蔻��p�̊֐�
* @param	(Object*)	�A�N�e�B�u�ɂȂ��Ă���I�u�W�F�N�g�̃|�C���^
* @return	(bool)		true�œ�����܂����@false�œ������ĂȂ���
*/
bool SceneStage_1::CollPlayer(Object* obj) {
	//�v���C���[�ƃ��X�g(�C�x���g�����蔻��)�ɂ������
	auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CCollider>();
	auto TransPlayer = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffSet = Player->GetOffSet();
	//�����̑傫���̌v�Z
	XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2.0f, PlayerSize.y / 2.0f);
	
	auto Coll = obj->GetComponent<CCollider>();
	auto Trans = obj->GetComponent<CTransform>();
	//�����̑傫���̌v�Z
	XMFLOAT2 CollHalfSize = XMFLOAT2(Coll->GetColliderSize().x/2.0f,Coll->GetColliderSize().y/2.0f);

	if (Coll->GetCenterPos().x - CollHalfSize.x + Trans->Vel.x < Player->GetCenterPos().x + PlayerHalfSize.x + TransPlayer->Vel.x &&
		Player->GetCenterPos().x - PlayerHalfSize.x + TransPlayer->Vel.x < Coll->GetCenterPos().x + CollHalfSize.x + Trans->Vel.x) {

		if (Coll->GetCenterPos().y - CollHalfSize.y + Trans->Vel.y < Player->GetCenterPos().y + PlayerHalfSize.y + TransPlayer->Vel.y &&
			Player->GetCenterPos().y - PlayerHalfSize.y + TransPlayer->Vel.y < Coll->GetCenterPos().y + CollHalfSize.y + Trans->Vel.y) {
			return true;
		}
	}

	return false;
}
