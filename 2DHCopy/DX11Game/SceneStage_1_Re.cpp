/**�C���N���[�h��**/
#include "SceneStage_1_Re.h"
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
#include "ColliderComponent.h"
#include "PlayerComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Sound.h"
#include  "OutOfRange.h"
#include "MosaicRollComponent.h"

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
SceneStage_1_Re::SceneStage_1_Re()
	:m_nBossHP(5){

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
		transBG->SetPosition(-720 * 0.5f + i * 720, 0); 
		drawBG->SetTexture(pTexManager->GetTexture(TITLE_BACK_GROUND_NUM));
		drawBG->SetSize(720, 720);
		scrollBG->SetAxizX(&(pPlayer->GetComponent<CTransform>()->Pos.x));
		scrollBG->SetScrollValueX(0.0016f);
		//���X�g�ɒǉ�
		m_pObjectManager->AddObject(objBG);
	}

	//BGM�Đ�
	CSound::Play(TITLE_BGM);

	//���\�����߂�
	SceneGame::GetInstance()->SetLaL(true);

	//�{�X��HP�̐ݒ�
	m_nBossHP = 5;
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
	//���X�g�̒��g���X�V����Ă��邩�𔻕ʂ���
	for (auto&& obj : m_EventList) {
		for (auto&& comp : obj->GetComponentList()) {
			//�X�V����Ă��邩�𔻕ʂ���
			if (comp->m_bUpdateFlag) {
				//��ʊO�ɂǂ������ׂ�z�̓X�L�b�v
				if (comp == obj->GetComponent<COutOfRange>()) {
					continue;
				}
				if (obj->GetName() == STAGE_RE_1_CHANGE_COLL) {
					//�����蔻��p�̊֐����Ă�
					if (CollPlayer(obj)) {
						ChangeObject();
					}
				}
				else if (obj->GetName() == CAMERA_MOVE_NAME) {
					if (CollPlayer(obj)) {
						CCamera::Get()->SetCameraMove(true);
					}
				}
			}
		}
	}

	//�{�X�����������ǂ����𔻕ʂ���
	if (m_nBossHP <= 0) {
		//�J�����̒����_�����ɖ߂�
		CCamera::Get()->SetCameraMove(false);
	}
}

/**
* @fn		SceneStage_1_Re::Draw
* @brief	�`�揈��
*/
void SceneStage_1_Re::Draw() {

}

/**
* @fn		SceneStage_1_Re::ChangeObject
* @brief	�I�u�W�F�N�g��C�ӂ̃^�C�~���O�ŕύX����
*/
void SceneStage_1_Re::ChangeObject() {
	//���U�C�N�𗬂�
	MosaicRoll::Begin();

	//�S�ẴI�u�W�F�N�g�̍X�V���~�߂�
	for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
		obj->StopUpdate();
	}
	//�X�V���X�g����UI�X�V�ȊO�̕���S�ĐF��ς���(�����u)
	for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
		if (obj->GetName() == "Back") {
			obj->GetComponent<CDraw2D>()->SetTexture(TextureManager::GetInstance()->GetTexture(STAGE_1_RE_BG_TEX_NUM));
		}
		else if(obj->GetName() == BLOCK_NAME){
			obj->GetComponent<CDraw3D>()->SetModel(ModelManager::GetInstance()->GetModel(GRASS_BLOCK_RE_NUM));
		}
		else if (obj->GetName() == BLOCK_RARD_NAME) {
			obj->GetComponent<CDraw3D>()->SetModel(ModelManager::GetInstance()->GetModel(RARD_BLOCK_RE_NUM));
		}
	}


	//�X�V�ĊJ
	//�I�u�W�F�N�g��S�čĊJ����
	for (auto&& obj : m_pObjectManager->GetUpdateList()) {
		obj->Use();
	}

	//�ω��I�u�W�F�N�g�̍폜
	for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
		if (obj->GetName() == STAGE_RE_1_CHANGE_COLL) {
			obj->NoUse();
		}
	}

	//���\��ς���
	SceneGame::GetInstance()->SetLaL(false);

	//���U�C�N�I��
	MosaicRoll::End();
}

/**
* @fn		SceneStage_1_Re::SetBaseInfo
* @brief	���ꓖ���蔻������X�g�ɕۑ����邽�߂̕ϐ�
* @param	(std::list<Object*>)	�Q�[���̃��X�g
*/
void SceneStage_1_Re::SetBaseInfo(std::list<Object*> objList) {
	for (auto&& obj : objList) {
		if (obj->GetName() == STAGE_RE_1_CHANGE_COLL || obj->GetName() == CAMERA_MOVE_NAME) {
			m_EventList.push_back(obj);
		}
	}
}

/**
* @fn		SceneStage_1_Re::DeleteList
* @brief	�C�x���g���X�g�ɒ��g���������ꍇ�폜����
*/
void SceneStage_1_Re::DeleteList() {
	if (m_EventList.size()) {
		m_EventList.clear();
	}
}

/**
* @fn		SceneStage_1_Re::CollPlayer
* @brief	�v���C���[�Ƃ̓����蔻������֐�
* @param	(Object*)	�A�N�e�B�u�ɂȂ��Ă���I�u�W�F�N�g�̃|�C���^	
* @return	(bool)	true�œ�����܂��� false�ł������ĂȂ���
*/
bool SceneStage_1_Re::CollPlayer(Object* obj){
	//�v���C���[�̏��
	auto Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CCollider>();
	auto TransPlayer = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CTransform>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffSet = Player->GetOffSet(); Player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CCollider>();

	//�����̑傫���̌v�Z
	XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2.0f, PlayerSize.y / 2.0f);

	auto Coll = obj->GetComponent<CCollider>();
	auto Trans = obj->GetComponent<CTransform>();

	//�����̑傫���̌v�Z
	XMFLOAT2 CollHalfSize = XMFLOAT2(Coll->GetColliderSize().x / 2.0f, Coll->GetColliderSize().y / 2.0f);

	if (Coll->GetCenterPos().x - CollHalfSize.x + Trans->Vel.x < Player->GetCenterPos().x + PlayerHalfSize.x + TransPlayer->Vel.x &&
		Player->GetCenterPos().x - PlayerHalfSize.x + TransPlayer->Vel.x < Coll->GetCenterPos().x + CollHalfSize.x + Trans->Vel.x) {

		if (Coll->GetCenterPos().y - CollHalfSize.y + Trans->Vel.y < Player->GetCenterPos().y + PlayerHalfSize.y + TransPlayer->Vel.y &&
			Player->GetCenterPos().y - PlayerHalfSize.y + TransPlayer->Vel.y < Coll->GetCenterPos().y + CollHalfSize.y + Trans->Vel.y) {
			return true;
		}
	}
	return false;
}

/**
* @fn		SceneStage_1_Re::GetOhuda
* @brief	���D���擾�������̃t���O
* @return	(bool)	���D���擾���Ă��邩
*/
bool SceneStage_1_Re::GetOhuda() {
	return m_bOhuda;
}

/**
* @fn		SceneStage_1_Re::SetOhuda
* @brief	���D���擾�ł��Ă��邩���Z�b�g����֐�
* @param	(bool)	���D���擾�ł��Ă��邩�ǂ����̊֐�
*/
void SceneStage_1_Re::SetOhuda(bool bOhuda) {
	m_bOhuda = bOhuda;
}

/**
* @fn		SceneStage_1_Re::SetBossHP
* @brief	�{�X��HP�����炵�Ă����֐�
*/
void SceneStage_1_Re::SetBossHP() {
	m_nBossHP--;
}

/**
* @fn		SceneStage_1_Re::GetBossHP
* @brief	�{�X��HP���擾���邽�߂̊֐�
* @return	(int)	�{�X��HP
*/
int SceneStage_1_Re::GetBossHP() {
	return m_nBossHP;
}