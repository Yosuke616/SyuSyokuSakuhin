/**�C���N���[�h��**/
#include "AttackComponent.h"
#include "TransformComponent.h"
#include "PlayerComponent.h"
#include "SeeCollComponent.h"

/**�萔��`**/
#define ATTACK_FRAME			(60)	//�U����������
#define ATTACK_COOL_TIME		(10)	//���̍U���Ɉڂ��܂ł̎���(�v���C���[)

/**
* @fn		CAttack::CAttack
* @brief	�R���X�g���N�^�͖����d���I�I�I
*/
CAttack::CAttack() 
	:m_pTrans(nullptr),m_pObj(nullptr)
	,m_eAttack(NONE_ATTACK){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CAttack::~CAttack
* @brief	�f�X�g���N�^��߂��Ȃ��񂾂���
*/
CAttack::~CAttack() {
	m_pObj = nullptr;
	m_eAttack = NONE_ATTACK;
}

/**
* @fn		CAttack::Start
* @brief	����������
*/
void CAttack::Start() {
	m_pTrans = Parent->GetComponent<CTransform>();
	//�ǂ̃I�u�W�F�N�g�ɂ��U�������ꍇ��������
	switch (m_eAttack)
	{
	case PLAYER_ATTACK:
		//�U�����ǂ��������Ă��邩���擾
		m_nAttackCnt = ATTACK_FRAME;
		break;

	default:break;
	}
}

/** 
* @fn		CAttack::Update
* @brief	�X�V����
* @detail	�ǂ̃I�u�W�F�N�g���U�����Ă��邩�ŏ�����ς���
*/
void CAttack::Update() {
	switch (m_eAttack)
	{
	case PLAYER_ATTACK:
		//�v���C���[�ɍ��킹�����x�𑫂�
		Parent->GetComponent<CTransform>()->Vel = m_pObj->GetComponent<CTransform>()->Vel;

		//�I�u�W�F�N�g�����ɉ����Ă���
		Parent->GetComponent<CTransform>()->Pos.y -= 4.0f;

		//�U���p�����蔻��̂������������Ă���
		m_nAttackCnt -= 4;
		//�����蔻����폜����
		if (m_nAttackCnt <= 0) {
			//�����蔻��̍폜
			Parent->Delete();

			//���b�V���̍폜
			Parent->GetComponent<CSeeColl>()->DeleteCollBox();

			//�v���C���[�̍U����Ԃ�ς��邽�߂̂��
			//�N�[���^�C���̐ݒ�
			m_pObj->GetComponent<CPlayer>()->SetCoolTime(ATTACK_COOL_TIME);

			//�ĂэU���p�����蔻��𐶐��ł���悤�ɂ���
			m_pObj->GetComponent<CPlayer>()->SetAttackFlg(false);

			//�v���C���[�̏�Ԃ����ɖ߂�
			//�󒆂��ǂ����ŃX�e�[�^�X��ς���
			if (m_pObj->GetComponent<CPlayer>()->GetPlayerJump()) {
				m_pObj->GetComponent<CPlayer>()->SetPlayerState(JUMP_PLAYER);
				m_pObj->GetComponent<CPlayer>()->ChangeTexture();
			}
			else {
				m_pObj->GetComponent<CPlayer>()->SetPlayerState(IDLE_PLAYER);
				m_pObj->GetComponent<CPlayer>()->ChangeTexture();
			}

		}
		break;

	default:break;
	}
}

/**
* @fn		CAttack::GetAttackRoL
* @brief	�U�����ǂ��炩�炳�ꂽ���̃t���O���擾����
* @return	(bool)	�E�Ȃ�true�A���Ȃ�false��Ԃ�
*/
bool CAttack::GetAttackRoL() {
	return m_bRoL;
}

/**
* @fn		CAttack::SetAttackType
* @brief	�N���U���������𔻕ʂ���
* @param	(WHO_ATTACK)	�N���U����������ݒ肷��
*/
void CAttack::SetAttackType(WHO_ATTACK who) {
	m_eAttack = who;
}

/**
* @fn		CAttack::SetSetRoL
* @brief	�E�������Z�b�g����
* @param	(bool)	�E�Ȃ�true�A���Ȃ�false
*/
void CAttack::SetRoL(bool RoL) {
	m_bRoL = RoL;
}

/**
* @fn		CAttack::SetObj
* @brief	�U�������I�u�W�F�N�g�̏��̓������|�C���^���Z�b�g����
* @param	(Object*) Object�̃|�C���^
*/
void CAttack::SetObject(Object* pObj) {
	m_pObj = pObj;
}