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
	,m_OldPos(0.0f,0.0f){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CEnemy::~CEnemy
* @brief	�f�X�g���N�^����E�E�E���ꂪ���̐��܂ꂽ�Ӗ��I
*/
CEnemy::~CEnemy() {

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
}

/**
* @fn		CEnemy::Update
* @brief	�X�V����
*/
void CEnemy::Update() {
	//�ߋ����W���X�V����
	m_OldPos = m_pCollider->GetCenterPos();

	//true�������獶��
	if (m_bRightorLeft) {
		//�ꐶ���ɓ���������l��
		m_pTransform->Vel.x = -VALUE_MOVE_ENEMY;
	}else{
		//�E�ɂ��ړ��ł���m�\�͂������炵��
		m_pTransform->Vel.x = VALUE_MOVE_ENEMY;
	}

}

/**
* @fn		CEnemy::Draw
* @brief	�`�悷��(�f�o�b�N�ŏo����������)
*/
void CEnemy::Draw() {
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
		return;
	}
}
