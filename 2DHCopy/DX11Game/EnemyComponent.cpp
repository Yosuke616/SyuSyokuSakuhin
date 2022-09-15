/**�C���N���[�h��**/
#include "EnemyComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#include "TransformComponent.h"
#include "Draw3dComponent.h"
#include "ColliderComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "imgui.h"
#include "AttackComponent.h"
#include "SeeCollComponent.h"
#include "sceneGame.h"

/**�萔��`**/
#define MAX_ENEMY_GRAVITY	(1.0f)
#define VALUE_MOVE_ENEMY	(1.5f)

/**
* @fn		CEnemy::CEnemy
* @brief	�V���W�A�R���X�g���N�^�ɏ��
* @detail	�g�����X�t�H�[���E�`��E�����蔻�蓙�̃|�C���^���k���ɂ���
*/
CEnemy::CEnemy() 
	:m_pTransform(nullptr),m_pDraw(nullptr),m_pCollider(nullptr)
	,m_OldPos(0.0f,0.0f)
	,m_bStopCom(false),m_bRoL(false),m_bStopVel(false)
	,m_eEnemy_Type(ENEMY_WALK),m_eEnemy_State(ENEMY_DEFAULT){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CEnemy::~CEnemy
* @brief	�f�X�g���N�^����E�E�E���ꂪ���̐��܂ꂽ�Ӗ��I
*/
CEnemy::~CEnemy() {
	TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
	pMaterial->Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}

/**
* @fn		CEnemy::Start
* @brief	�J�n����
*/
void CEnemy::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pDraw		 = Parent->GetComponent<CDraw3D>();
	m_pCollider	 = Parent->GetComponent<CCollider>();
	m_bRightorLeft = true;
	m_eEnemy_State = ENEMY_DEFAULT;
	m_bStopCom = false;
	m_bStopVel = false;
	m_nBlink = 0;
	m_nDeleteFlg = 0;
}

/**
* @fn		CEnemy::Update
* @brief	�X�V����
*/
void CEnemy::Update() {
	//�ߋ����W���X�V����
	m_OldPos = m_pCollider->GetCenterPos();
	
	switch (m_eEnemy_State)
	{
	case ENEMY_DEFAULT:
		//�G�̎�ނɂ���ċ�����ς��Ă���
		switch (m_eEnemy_Type) {
		case ENEMY_WALK:
			//true�������獶��
			if (m_bRightorLeft) {
				//�ꐶ���ɓ���������l��
				m_pTransform->Vel.x = -VALUE_MOVE_ENEMY;
				m_pTransform->SetRotate(0.0f,90.0f,0.0f);
			}
			else {
				//�E�ɂ��ړ��ł���m�\�͂������炵��
				m_pTransform->Vel.x = VALUE_MOVE_ENEMY;
				m_pTransform->SetRotate(0.0f, -90.0f, 0.0f);
			}
			break;

		default:break;
		}
		break;

	case ENEMY_DELETE:
		if (!m_bStopCom) {

			//�S�ẴR���|�[�l���g�̓������~����
			//������CTransform�A���߂��͂��߂�
			//Draw3D�A���߁[�����߂�
			//���̃R���|�[�l���g����
			for (auto&& object : Parent->GetComponentList()) {
				object->m_bUpdateFlag = false;
			}
			Parent->GetComponent<CTransform>()->m_bUpdateFlag = true;
			Parent->GetComponent<CDraw3D>()->m_bUpdateFlag = true;
			Parent->GetComponent<CEnemy>()->m_bUpdateFlag = true;

			m_bStopCom = true;
			
			//�}�ɐ�����΂��ꂽ�����o�����߂ɑ��x��0�ɂ���
			m_pTransform->Vel = XMFLOAT3(0.0f,0.0f,0.0f);
		}

		if (!m_bStopVel) {
			//�|�W�V������ς���(���x��ς����������R����)
			//�v���C���[�̍��E�̌������擾���ăv���C���[�̌����Ă�������ɔ�΂�
			if (m_bRoL) {
				//�E����U�����ꂽ�ꍇ
				m_pTransform->Vel.x += 1.75f;
				m_pTransform->Vel.y += 2.0f;

				//���x����萔��������t���O��ς���
				if (m_pTransform->Vel.y > 10.0f) {
					m_bStopVel = true;
					m_pTransform->Vel.x = 0.0f;
					m_pTransform->Vel.y = 0.0f;
				}

			}
			else {
				//������U�����ꂽ�ꍇ
				m_pTransform->Vel.x -= 1.75f;
				m_pTransform->Vel.y += 2.0f;

				//���x����萔��������t���O��ς���
				if (m_pTransform->Vel.y > 10.0f) {
					m_bStopVel = true;
					m_pTransform->Vel.x = 0.0f;
					m_pTransform->Vel.y = 0.0f;
				}
			}		

		}

		//�S�Ă̂Ă����_�ł��邩��ς��Ȃ��Ƃ����Ȃ�

		

		//�폜�t���O�������������
		if (m_nDeleteFlg > 5) {
			Parent->Delete();
			Parent->GetComponent<CSeeColl>()->DeleteCollBox();
			SceneGame::GetInstance()->SetScore(100);
		}

		break;
	default:break;
	}
}

/**
* @fn		CEnemy::Draw
* @brief	�`�悷��(�f�o�b�N�ŏo����������)
*/
void CEnemy::Draw() {
	//�F��ς���
	if (m_eEnemy_State == ENEMY_DELETE) {
		//�_�ł����Ă����������
		m_nBlink++;
		//�G��Y�̃X�s�[�h��2�Ŋ����ċ����ŕ\��
		if (m_nBlink < 5) {
			//����
			//���f���̐F��ς���
			TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
			pMaterial->Kd = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (m_nBlink < 10) {
			//�t����
			TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
			pMaterial->Kd = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		}
		else {
			m_nBlink = 0;
			m_nDeleteFlg++;
		}
	}
	else {
		TAssimpMaterial* pMaterial = m_pDraw->GetModel()->GetMate();
		pMaterial->Kd = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

#ifdef _DEBUG
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Enemy");
	Text("Pos	  : %3.0f %3.0f %3.0f", m_pTransform->Pos.x, m_pTransform->Pos.y, m_pTransform->Pos.z);
	Text("Vel	  : %.0f %.0f", m_pTransform->Vel.x, m_pTransform->Vel.y);
	End();
#endif
}

/**
* @fn		CEnemy::OnCollisionEnter
* @brief	�G���������ɓ��������Ƃ��̏���
* @param	(Object*)	������̃I�u�W�F�N�g�̏�񂪓������|�C���^
*/
void CEnemy::OnCollisionEnter(Object* pObject) {
	//��(�u���b�N)
#pragma region ---�u���b�N
	if (pObject->GetName() == BLOCK_NAME) {
		//�G�̏����擾
		auto Enemy = Parent->GetComponent<CCollider>();
		auto EnemyPos = Enemy->GetCenterPos();
		auto EnemySize = Enemy->GetColliderSize();
		auto EnemyOffset = Enemy->GetOffSet();

		//�Ԃ������u���b�N�̏��
		auto Block = pObject->GetComponent<CCollider>();
		auto BlockPos = Block->GetCenterPos();
		auto BlockSize = Block->GetColliderSize();
		auto BlockOffset = Block->GetOffSet();

		//���ꂼ�ꔼ���̑傫����ۑ�
		XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y *0.5f);
		XMFLOAT2 BlockHalhSize = XMFLOAT2(BlockSize.x * 0.5f, BlockSize.y * 0.5f);

		//�u���b�N�̓����蔻��̐�
		float BlockLeftLine = BlockPos.x - BlockHalhSize.x;	//���[
		float BlockRightLine = BlockPos.x + BlockHalhSize.x;	//�E�[
		float BlockUpLine = BlockPos.y + BlockHalhSize.y;	//��[
		float BlockDownLine = BlockPos.y - BlockHalhSize.y;	//���[

		/**
		* @brief �G�l�~�[���u���b�N�̒�(���[�ƉE�[�̒�)�ɂ����ꍇ�̏���
		*/
		if ((BlockLeftLine < EnemyPos.x && EnemyPos.x < BlockRightLine) ||
			(BlockLeftLine < EnemyPos.x - EnemyHalhSize.x && EnemyPos.x - EnemyHalhSize.x < BlockRightLine) ||
			(BlockLeftLine < EnemyPos.x + EnemyHalhSize.x && EnemyPos.x + EnemyHalhSize.x < BlockRightLine)) {

			/** @brief �G����ɏ������*/
			if (BlockUpLine <= m_OldPos.y - EnemyHalhSize.y &&					//�O�t���[���͂߂荞��ł��Ȃ�
				EnemyPos.y - EnemyHalhSize.y < BlockUpLine) {	//���݂̃t���[���͂߂荞��ł���
 //y�̑��x���Ȃ���
				m_pTransform->Vel.y = 0.0f;

				//���W�␳
				m_pTransform->Pos.y = BlockUpLine + EnemyHalhSize.y - EnemyOffset.y;
			}
			/** @brief �������񂱏�Ԃł���*/
			else if (BlockDownLine >= m_OldPos.y + EnemyHalhSize.y &&	//�O�t���[���͂߂荞��ł��Ȃ�
				EnemyPos.y + EnemyHalhSize.y > BlockDownLine) {		//���݂̃t���[���͂߂荞��ł���
				   //y�̑��x���Ȃ���
				m_pTransform->Vel.y = 0.0f;
				//���W�␳
				m_pTransform->Pos.y = BlockDownLine - EnemyHalhSize.y - EnemyOffset.y;
			}
		}

		//�������x�����𒴂����痎������ɂ���
		if (m_pTransform->Vel.y == -MAX_VELOCITY) {
			int i = 1;
		}
		else {
			int i = 1;
		}

		//���S���W�̃Z�b�g
		EnemyPos = Enemy->GetCenterPos();

		/** @brief �G���u���b�N�̒�(��[�Ɖ��[�̒�)�ɂ����ꍇ�̏���*/
		if ((BlockDownLine < EnemyPos.y && EnemyPos.y < BlockUpLine) ||
			(BlockDownLine < EnemyPos.y - EnemyHalhSize.y && EnemyPos.y - EnemyHalhSize.y < BlockUpLine) ||
			(BlockDownLine < EnemyPos.y + EnemyHalhSize.y && EnemyPos.y + EnemyHalhSize.y < BlockUpLine)) {
			//�G���E�̃u���b�N�ɓ��������ꍇ
			if (BlockLeftLine >= m_OldPos.x + EnemyHalhSize.x &&		//�O�t���[���͂߂荞��ł��Ȃ�
				EnemyPos.x + EnemyHalhSize.x > BlockLeftLine) {		//���݂̃t���[���͂߂荞��ł���
				//�ړ��ʂ��폜����
				m_pTransform->Vel.x = 0.0f;

				//���W�␳
				m_pTransform->Pos.x = BlockLeftLine - EnemyHalhSize.x - EnemyOffset.x;
				//���]������
				m_bRightorLeft = true;
			}
			else if (BlockRightLine <= m_OldPos.x - EnemyHalhSize.x &&
				EnemyPos.x - EnemyHalhSize.x < BlockRightLine) {
				//�ړ��ʂ��폜����
				m_pTransform->Vel.x = 0.0f;

				//���W�̕␳
				m_pTransform->Pos.x = BlockRightLine + EnemyHalhSize.x - EnemyOffset.x;
				//���]������
				m_bRightorLeft = false;
			}
		}
	}
#pragma endregion
	//�v���C���[�̍U��
	if (pObject->GetName() == ATTACK_NAME) {
		//�G�̏����擾
		auto Enemy = Parent->GetComponent<CCollider>();
		auto EnemyPos = Enemy->GetCenterPos();
		auto EnemySize = Enemy->GetColliderSize();
		auto EnemyOffset = Enemy->GetOffSet();
		//�����̑傫��
		XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y *0.5f);

		//�����蔻��̏��
		auto TAttack = pObject->GetComponent<CTransform>();
		auto cAttack = pObject->GetComponent<CCollider>();
		//�����̑傫��
		XMFLOAT2 AttackHalfSize = XMFLOAT2(cAttack->GetColliderSize().x/2.0f,cAttack->GetColliderSize().y/2.0f);

		if (cAttack->GetCenterPos().x - AttackHalfSize.x + TAttack->Vel.x < m_pCollider->GetCenterPos().x + EnemyHalhSize.x + m_pTransform->Vel.x &&
			m_pCollider->GetCenterPos().x - EnemyHalhSize.x + m_pTransform->Vel.x < cAttack->GetCenterPos().x + AttackHalfSize.x + TAttack->Vel.x) {
			if (cAttack->GetCenterPos().y - AttackHalfSize.y + TAttack->Vel.y < m_pCollider->GetCenterPos().y + EnemyHalhSize.y + m_pTransform->Vel.y &&
				m_pCollider->GetCenterPos().y - EnemyHalhSize.y + m_pTransform->Vel.y < cAttack->GetCenterPos().y + AttackHalfSize.y + TAttack->Vel.y) {
				//�U�����q�b�g�����̂œG�I�u�W�F�N�g���폜����
				m_eEnemy_State = ENEMY_DELETE;
				m_bRoL = pObject->GetComponent<CAttack>()->GetAttackRoL();
			}
		}
	}
}

/**
* @fn		CEnemy::SetEnemyType
* @brief	�G�̎�ނ����߂�֐�(3��ނ��炢�H)
* @param	(ENEMY_TYPE) �G�̎�ނ����߂Ă��炤
*/
void CEnemy::SetEnemyType(ENEMY_TYPE type) {
	m_eEnemy_Type = type;
}