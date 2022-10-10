/**�C���N���[�h��**/
#include "ItemComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "imgui.h"

#include <stdlib.h>

/**�萔��`**/
/** @brief ���ʂ��㉺���鑬��*/
#define MAGATAMA_SPEED	(0.05f)
 
/**
* @fn		CItem::CItem
* @brief	�R���X�g���N�^�I�����C��
*/
CItem::CItem()
	:m_pTransform(nullptr),m_pDraw(nullptr),m_pCollider(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CItem::~CItem
* @brief	�T��f�X�g���N�^
*/
CItem::~CItem() {

}

/**
* @fn		CItem::Start
* @brief	�J�n����
*/
void CItem::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw3D>();
	m_pCollider  = Parent->GetComponent<CCollider>();
	m_fUpDown = 0.0f;
	m_fRotTime = 90.0f;
	m_bUpDown = false;
	m_bPlayer = false;
	m_bSetUp  = false;
	m_bFirstBlad = false;
}

/**
* @fn		CItem::Update
* @brief	�A�C�e���I�u�W�F�N�g�̍X�V
* @detail	�I�����ꂽ�A�C�e���œ��e�͕ω�����
*/
void CItem::Update() {
	//�I�����ꂽ�A�C�e���œ��e��ς���
	switch (m_eItem) {
	case ITEM_KOBAN:
		//��]������(Y����])
		m_pTransform->Rotate.y += 5.0f;
		break;
	case ITEM_MAGA:
		//�v���C���[�Ɏ擾���ꂽ���ǂ����ŏ�����ς���
		if (m_bPlayer) {
			//��]������(Z����])
			m_pTransform->Rotate.z += 5.0f;

			//�����蔻����폜����
			m_pCollider->m_bUpdateFlag = false;

			//�v���C���[�̏�ɂ��Ă��邩���Ă��Ȃ���
			if (m_bSetUp) {
				Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
				XMFLOAT3 player_pos = obj->GetComponent<CTransform>()->Pos;

				m_fRotTime += 0.05f;
				if (m_fRotTime > 360) {
					m_fRotTime = 0;
				}

				m_pTransform->Pos.x = player_pos.x + (cos(m_fRotTime) * 75);
				m_pTransform->Pos.y = player_pos.y + (sin(m_fRotTime) * 75);
			}
			else {
				if (!m_bFirstBlad) {
					//�E��ɗ͂�������
					m_pTransform->Vel.x -= 2.0f;
					m_pTransform->Vel.y -= 2.0f;
					m_bFirstBlad = true;
				}

				//�v���C���[�̓���Ɏ����Ă���
				Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);
				XMFLOAT3 player_pos = obj->GetComponent<CTransform>()->Pos;
				player_pos.y += 75.0f;

				//�v���C���[�̌��ʂƂ̃x�N�g�����擾����
				XMFLOAT2 direction = XMFLOAT2(player_pos.x - m_pTransform->Pos.x, player_pos.y - m_pTransform->Pos.y);
				
				//XMFLOAT2��XMVECTOR�^�ɂ�ϊ�
				XMVECTOR xmvNorm = XMLoadFloat2(&direction);
				//�x�N�g���̐��K��
				xmvNorm = XMVector2Normalize(xmvNorm);
				//���K�����ꂽXMVECTOR�^��XMFLOAT2�^�ɓ���锠�̗p��
				XMFLOAT2 vNorm;
				//XMVECTOR��XMFLOAT2�ɕϊ�
				XMStoreFloat2(&vNorm,xmvNorm);

				//���x�̐���----------------------------------
				m_pTransform->Vel.x += vNorm.x * 0.5f * 2;
				if (m_pTransform->Vel.x > 2.5f) {
					m_pTransform->Vel.x = 2.5f;
				}
				m_pTransform->Vel.y += vNorm.y * 0.5f * 2;
				if (m_pTransform->Vel.y > 2.5f) {
					m_pTransform->Vel.y = 2.5f;
				}
				//--------------------------------------------

				//x,y���ꂼ��̍��W��������瑬�x��0�ɂ���
				float X = (player_pos.x - m_pTransform->Pos.x);
				float Y = (player_pos.y - m_pTransform->Pos.y);

				//��Βl�̐ݒ肷��
				X = abs(X);
				Y = abs(Y);

				if (X < 10.0f) {
					m_pTransform->Vel.x = 0.0f;
				}
				if (Y < 10.0f) {
					m_pTransform->Vel.y = 0.0f;
				}

				//�v���C���[�̏�̍��W�Ɠ����ɂȂ����甲����
				if (X < 10.0f && Y < 10.0f){
					m_bSetUp = true;
				}

			}
		}
		else {
			//�㉺�Ɉړ�������
			//��]������(Z����])
			m_pTransform->Rotate.z += 1.0f;

			if (m_bUpDown) {
				//��Ɉړ�������
				m_pTransform->Vel.y = MAGATAMA_SPEED;
				m_fUpDown += MAGATAMA_SPEED;

				//������x�オ�����牺�ɉ�����
				if (m_fUpDown > 2.0f) {
					m_bUpDown = false;
				}
			}
			else {
				//���Ɉړ�������
				m_pTransform->Vel.y = -MAGATAMA_SPEED;
				m_fUpDown -= MAGATAMA_SPEED;

				//������x�����������ɏグ��
				if (m_fUpDown < -2.0f) {
					m_bUpDown = true;
				}
			}
		}

		break;
	case ITEM_OHUDA:
		//�����̉�]�ƌ��ʂ̃t���t�����o��
		//��]������(Y����])
		m_pTransform->Rotate.y -= 5.0f;
		if (m_bUpDown) {
			//��Ɉړ�������
			m_pTransform->Vel.y = MAGATAMA_SPEED;
			m_fUpDown += MAGATAMA_SPEED;

			//������x�オ�����牺�ɉ�����
			if (m_fUpDown > 2.0f) {
				m_bUpDown = false;
			}
		}
		else {
			//���Ɉړ�������
			m_pTransform->Vel.y = -MAGATAMA_SPEED;
			m_fUpDown -= MAGATAMA_SPEED;

			//������x�����������ɏグ��
			if (m_fUpDown < -2.0f) {
				m_bUpDown = true;
			}
		}
		break;
	}
}

/** 
* @fn		CItem::Draw
* @brief	�A�C�e���I�u�W�F�N�g�̕`��
*/
void CItem::Draw() {
	
}

/**
* @fn		CItem::OnCollisionEnter
* @brief	�����蔻��̏���
* @param	(Object*)	��������������
*/
void CItem::OnCollisionEnter(Object* obj) {

}

/**
* @fn		CItem::SetItem
* @brief	�A�C�e���̎�ނ����߂�
* @param	(ITEM_TYPE)	���A�C�e��
*/
void CItem::SetItem(ITEM_TYPE type) {
	m_eItem = type;
}

/** 
* @fn		CItem::GetItem
* @brief	�ݒ肳��Ă���A�C�e���̎�ނ��擾����
* @return	(ITEM_TYPE)	���̃I�u�W�F�N�g�̃A�C�e���^�C�v 
*/
ITEM_TYPE CItem::GetItem() {
	return m_eItem;
}

/**
* @fn		CItem::SetPlayer
* @brief	�v���C���[�Ɏ擾���ꂽ���ǂ�����ݒ肷��
* @param	(bool)	�v���C���[�Ɏ擾���ꂽ��true
*/
void CItem::SetPlayer(bool player) {
	m_bPlayer = player;
}