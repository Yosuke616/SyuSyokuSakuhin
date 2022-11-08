/**�C���N���[�h��**/
#include "WarpComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "TextureManager.h"

#include "Object.h"
#include "ObjInfo.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "PlayerComponent.h"
#include "sceneGame.h"
#include "Camera.h"
#include "imgui.h"
#include "fade.h"

#include "SceneStage_1.h"

/**
* @fn		CWarp::CWarp
* @brief	�R���X�g���N�^�G���W��
*/
CWarp::CWarp() 
	:m_pTransform(nullptr),m_pCollider(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CWarp::CWarp
* @brief	�f�X�g���N�^�݂����Ńe���V�����オ��Ȃ�
*/
CWarp::~CWarp() {

}

/**
* @fn		CWarp::Start	
* @brief	�J�n����
*/
void CWarp::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();
	m_bWarpFlg = false;
	m_StartPos = m_pTransform->Pos;
}

/**
* @fn		CWarp::Update
* @brief	�X�V����
*/
void CWarp::Update() {
	//���[�v�t���O���I���ɂȂ����珈�����J�n����
	if (m_bWarpFlg) {
		//�X�e�[�W���Ƃɂǂ����[�v�����邩�͌��߂�

		//��肽������
		//��ʂ�^���Âɂ�����(�t�F�[�h�A�E�g) 
		//�I�u�W�F�N�g�̍X�V���~�߂�
		//�`��̍X�V���~�߂�
		//UI�̕\��������
		//�v���C���[�ƃJ�����̈ʒu���΂�(4580,-573.139771+770)
		//�����_���Ȃ���(�Œ�J�����ɂ���)
		//�w�i��ύX����
		//�X�V�ƕ`��̍ĊJ
		//UI�̕\�����ĊJ
		//�t�F�[�h�C��������

		switch (SceneGame::GetInstance()->GetStage()) {
		case STAGE_1:
			//�����̓t�F�[�h�A�E�g�ɕς�����
			if (GetFade() == FADE_IN) {

				ChangeCamera();
				//�I�u�W�F�N�g���ĊJ������
				for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
					obj->Use();
				}
				//�Ō�Ƀv���C���[�̏�Ԃ�ύX������
				ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME)->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);

				//��x�ݒ��ς�����t���O��߂��ē���Ȃ��悤�ɂ���
				m_bWarpFlg = false;

				break;
			}

			if (GetFade() != FADE_OUT) {
				StartFadeOut(0);
				
				//�v���C���[�̏�Ԃ����[�v���Ă����Ԃɂ���
				Object* player = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
				player->GetComponent<CPlayer>()->SetPlayerState(WARP_PLAYER);

				//�����Ă���I�u�W�F�N�g�̍X�V���X�g�b�v����
				for (auto&& obj : ObjectManager::GetInstance()->GetUpdateList()) {
					if (obj->GetName() == WARP_NAME ||obj->GetName() == BLOCK_NAME || obj->GetName() == BLOCK_RARD_NAME || obj->GetName() == PLAYER_NAME) {
						continue;
					}
					obj->StopUpdate();
				}
				//UI�`����X�g�b�v����
				for (auto&& obj : ObjectManager::GetInstance()->GetDrawList()) {
					if (obj->GetDrawOrder() == DRAW_UI) {
						obj->StopDraw();
					}
				}
				break;
			}
			break;
		case STAGE_1_RE:break;
		}
	}
}

/**
* @fn		CWarp::Draw
* @brief	�`�揈��
*/
void CWarp::Draw() {

}

/**
* @fn		CWarp::OnCliisionEnter
* @brief	�����蔻��̏���
* @param	(Object*)	��������������
*/
void CWarp::OnCollisionEnter(Object* pObject) {
	
}

/**
* @fn		CWarp::SetWarp
* @brief	���[�v�������ǂ������Z�b�g����֐�
* @param	(bool)	true�Ń��[�v���J�n����
*/
void CWarp::SetWarp(bool bWarp) {
	m_bWarpFlg = bWarp;
}

/**
* @fn		CWarp::ChangeCamera
* @brief	�J�����̒����_��؂��āA�J�����ƃv���C���[�̍��W��ύX����
*/
void CWarp::ChangeCamera() {
	//�v���C���[�̃I�u�W�F�N�g�̃|�C���^���擾����
	Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);

	//�X�e�[�W(���[�v�悩�ǂ�������������)���Ƃɏꏊ��ς���
	switch (SceneGame::GetInstance()->GetStage())
	{
	case STAGE_1:
		if (SceneStage_1::GetInstance()->GetWarpPoint() == 0) { 
			//0�Ԃɔ�΂���
			auto point = ObjectManager::GetInstance()->GetGameObject(WARP_NUMBER_0)->GetComponent<CTransform>();

			//�J�����̍��W���Z�b�g����
			CCamera::Get()->SetPos(point->Pos.x, obj->GetComponent<CTransform>()->Pos.y, -300.0f);
			obj->GetComponent<CTransform>()->Pos.x = point->Pos.x+20;
			obj->GetComponent<CTransform>()->Pos.y = point->Pos.y+50;
			//���̃I�u�W�F�N�g���ړ�������
			m_pTransform->Pos.x = 4400;
			m_pTransform->Pos.y = 75;
			//�J�����̈ړ����E��ύX����
			SceneStage_1::GetInstance()->SetWarpPoint(1);
		}
		else if(SceneStage_1::GetInstance()->GetWarpPoint() == 1){
			//0�Ԃɔ�΂���
			auto point = ObjectManager::GetInstance()->GetGameObject(WARP_NUMBER_1)->GetComponent<CTransform>();

			//�J�����̍��W���Z�b�g����
			CCamera::Get()->SetPos(point->Pos.x, obj->GetComponent<CTransform>()->Pos.y, -300.0f);
			obj->GetComponent<CTransform>()->Pos.x = point->Pos.x;
			obj->GetComponent<CTransform>()->Pos.y = point->Pos.y+50;
			//���̃I�u�W�F�N�g���ړ�������
			m_pTransform->Pos.x = m_StartPos.x;
			m_pTransform->Pos.y = m_StartPos.y;
			//�J�����̈ړ����E��ύX����
			SceneStage_1::GetInstance()->SetWarpPoint(0);
		}
		break;
	case STAGE_1_RE:break;

	default:break;
	}
}
