/**�C���N���[�h��**/
#include "WarpComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "InputManager.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "sceneGame.h"
#include "imgui.h"

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


}

/**
* @fn		CWarp::Update
* @brief	�X�V����
*/
void CWarp::Update() {

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
	if (CollPlayer(pObject)) {
		//�V�[�����Ƃɂǂ����[�v�����邩���l����
		switch (SceneGame::GetInstance()->GetStage())
		{
		case STAGE_1:
			if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
				int i = 1 + 10;
			}
			break;
		case STAGE_1_RE:break;
		default:break;
		}
	}
}

/**
* @fn		CWarp::CollPlayer
* @brief	�v���C���[�Ƃ̓����蔻��̏���
* @param	(Object*)	�v���C���[�Ɠ������������ǂ���
* @return	(bool)		�����������ǂ���
*/
bool CWarp::CollPlayer(Object* pObject) {
	if (pObject->GetName() == PLAYER_NAME) {
		//�v���C���[�̏����擾
		auto Player = pObject->GetComponent<CTransform>();
		auto PlayerColl = pObject->GetComponent<CCollider>();
		auto PlayerPos = PlayerColl->GetCenterPos();
		auto PlayerSize = PlayerColl->GetColliderSize();
		auto PlayerOffSet = PlayerColl->GetOffSet();

		//�v���C���[�̔����̑傫�����擾
		XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2, PlayerSize.y / 2);

		XMFLOAT2 WarpHalfSize = XMFLOAT2(m_pCollider->GetColliderSize().x / 2.0f, m_pCollider->GetColliderSize().y / 2.0f);

		if (m_pCollider->GetCenterPos().x - WarpHalfSize.x + m_pTransform->Vel.x < PlayerPos.x + PlayerHalfSize.x + Player->Vel.x &&
			PlayerPos.x - PlayerHalfSize.x + Player->Vel.x < m_pCollider->GetCenterPos().x + WarpHalfSize.x + m_pTransform->Vel.x) {

			if (m_pCollider->GetCenterPos().y - WarpHalfSize.y + m_pTransform->Vel.y < PlayerPos.y + PlayerHalfSize.y + Player->Vel.y &&
				PlayerPos.y - PlayerHalfSize.y + Player->Vel.y < m_pCollider->GetCenterPos().y + WarpHalfSize.y + m_pTransform->Vel.y) {
				return true;
			}
		}
	}
	return false;
}
