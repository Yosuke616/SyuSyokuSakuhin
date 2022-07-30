//�C���N���[�h��
#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Camera.h"

//�萔��`
/** @brief*/
#define MAX_PLAYER_GRAVITY		(1.0f)
#define VALUE_JUMP_SPEED		(8.0f)
#define VALUE_MOVE_SPEED		(1.5f)

/**�O���[�o���ϐ�**/
PLAYER_STATE g_ePlayer;

/**
* @fn		CPlayer::CPlayer
* @brief	���̖��O�̓R���X�g���N�^�@�����̃R���X�g���N�^
* @detail	�g�����X�t�H�[����`������C�j�V�����C�Y��null�ɂ���
*/
CPlayer::CPlayer() :
	m_pPlayer(nullptr),m_pDraw2D(nullptr),m_pInput(nullptr),
	m_OldPos(0.0f,0.0f),m_bGround(false),
	m_bJump(false),
	m_ePlayer(IDLE_PLAYER){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CPlayer::~CPlayer()
* @brief	�f�X�g���N�^�����̂��@���ȊO�̂��
*/
CPlayer::~CPlayer() {

}

/**
* @fn		CPlayer::Start
* @brief	�J�n����
*/
void CPlayer::Start() {
	m_pDraw2D = Parent->GetComponent<CDraw2D>();
	m_pPlayer = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();
	m_pInput = InputManager::Instance();
	g_ePlayer = m_ePlayer;
}

/**
* @fn		CPlayer::Update
* @brief	�X�V����
*/
void CPlayer::Update() {
	m_OldPos = m_pCollider->GetCenterPos();

	//�X�e�[�^�X�̏����X�V����
	m_ePlayer = g_ePlayer;

#if _DEBUG
	if (m_pInput->GetKeyPress(DIK_UPARROW)) {
		//��ړ�
		m_pPlayer->Pos.y += 10.0f;
	}	
	if (m_pInput->GetKeyPress(DIK_DOWNARROW)) {
		//���ړ�
		m_pPlayer->Pos.y += -10.0f;
	}
	if (m_pInput->GetKeyPress(DIK_RIGHTARROW)) {
		//�E�ړ�
		m_pPlayer->Pos.x += 5.0f;
	}
	if (m_pInput->GetKeyPress(DIK_LEFTARROW)) {
		//�E�ړ�
		m_pPlayer->Pos.x += -5.0f;
	}
#endif

	switch (m_ePlayer) {
	case IDLE_PLAYER:
		//���ʂȃA�N�V�������N�������ꍇ�A�v���C���[�̏�Ԃ�ς���

		//���ړ��͂Ȃ��ɂ���,�c�͏d�͂�������
		m_pPlayer->Vel.x = 0.0f;

		if (m_pInput->GetKeyPress(DIK_D)) {
			//�E�ړ�
			m_ePlayer = RUN_PLAYER;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_ePlayer = RUN_PLAYER;
		}

		//�W�����s���O
		if (m_ePlayer == IDLE_PLAYER) {
			if (m_pInput->GetKeyPress(DIK_W)) {
				//�X�e�[�^�X���W�����v�ɕς���
				m_ePlayer = JUMP_PLAYER;
			}
		}
		break;

	case RUN_PLAYER:
		if (m_pInput->GetKeyPress(DIK_D)) {
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_ePlayer = IDLE_PLAYER;
		}

		//�W�����s���O	
		if (m_pInput->GetKeyPress(DIK_W)) {
			//�X�e�[�^�X���W�����v�ɕς���
			m_ePlayer = JUMP_PLAYER;
		}
	
		break;

	case JUMP_PLAYER:
		////������x�グ���炭�킦���͂�����

		//������������
		if (!m_bJump) {
			//�d�͂̃R���|�[�l���g�̋@�\���I�t�ɂ���
			Parent->GetComponent<CGravity>()->SetUse(false);
			m_pPlayer->Vel.y = VALUE_JUMP_SPEED;
			m_bJump = true;
		}
		else {
			m_pPlayer->Vel.y -= GRAVITY;
			if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
				Parent->GetComponent<CGravity>()->SetUse(true);
			}
		}

		//���E�ړ��͂ł���悤�ɂ��Ă���
		if (m_pInput->GetKeyPress(DIK_D)) {
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_pPlayer->Vel.x = 0.0f;
		}

		break;

	case FALL_PLAYER:
		//�������x�������Ȃ�����v���C���[�̏�Ԃ�ҋ@��Ԃɖ߂�
		if (m_pPlayer->Vel.y >= 0) {
			m_ePlayer = IDLE_PLAYER;
		}

		//���E�ړ��͂ł���悤�ɂ��Ă���
		if (m_pInput->GetKeyPress(DIK_D)) {
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_pPlayer->Vel.x = 0.0f;
		}

		//�����t���O�̓I���ɂ���
		m_bJump = true;

		break;
	default: break;
	}

	//�v���C���[�̍ŐV��Ԃ�ۑ�����
	g_ePlayer = m_ePlayer;

	//���x�̏�������߂�
	if (m_pPlayer->Vel.x > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.x < -VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
	}
	if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
		m_pPlayer->Vel.y = -MAX_VELOCITY;
	}

	/*if (m_pPlayer->Vel.y > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.y = VALUE_MOVE_SPEED;
	}*/
}

/**
* @fn		CPlayer::Draw
* @brief	�v���C���[���f�o�b�O�\��
*/
void CPlayer::Draw() {
#ifdef _DEBUG
	//	debug
	XMFLOAT2 a = Parent->GetComponent<CCollider>()->GetCenterPos();

	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Player");
	Text("Pos	  : %3.0f %3.0f %3.0f", m_pPlayer->Pos.x, m_pPlayer->Pos.y, m_pPlayer->Pos.z);
	Text("Vel	  : %.0f %.0f", m_pPlayer->Vel.x, m_pPlayer->Vel.y);
	Text("P_STATE : %d",g_ePlayer);
	End();
#endif // _DEBUG
}

/**
* @fn		CPlayer::OnCollisionEnter
* @brief	�v���C���[�������ɓ��������Ƃ��̏���
* @param	(Object*)	������̃I�u�W�F�N�g�̃|�C���^
*/
void CPlayer::OnCollisionEnter(Object* pObject) {
	//��(�u���b�N)
	if(pObject->GetName() == BLOCK_NAME){
		//�v���C���[�̏����擾
		auto Player = Parent->GetComponent<CCollider>();
		auto PlayerPos = Player->GetCenterPos();
		auto PlayerSize = Player->GetColliderSize();
		auto PlayerOffset = Player->GetOffSet();

		//�Ԃ������u���b�N�̏��
		auto Block = pObject->GetComponent<CCollider>();
		auto BlockPos = Block->GetCenterPos();
		auto BlockSize = Block->GetColliderSize();
		auto BlockOffset = Block->GetOffSet();

		//���ꂼ�ꔼ���̑傫����ۑ�
		XMFLOAT2 PlayerHalhSize = XMFLOAT2(PlayerSize.x * 0.5f,PlayerSize.y *0.5f);
		XMFLOAT2 BlockHalhSize = XMFLOAT2(BlockSize.x * 0.5f,BlockSize.y * 0.5f);

		//�u���b�N�̓����蔻��̐�
		float BlockLeftLine  = BlockPos.x - BlockHalhSize.x;	//���[
		float BlockRightLine = BlockPos.x + BlockHalhSize.x;	//�E�[
		float BlockUpLine	 = BlockPos.y + BlockHalhSize.y;	//��[
		float BlockDownLine  = BlockPos.y - BlockHalhSize.y;	//���[

		/**
		* @brief �v���C���[���u���b�N�̒�(���[�ƉE�[�̒�)�ɂ����ꍇ�̏���
		*/
		if ((BlockLeftLine < PlayerPos.x && PlayerPos.x < BlockRightLine) ||
			(BlockLeftLine < PlayerPos.x - PlayerHalhSize.x && PlayerPos.x - PlayerHalhSize.x < BlockRightLine) ||
			(BlockLeftLine < PlayerPos.x + PlayerHalhSize.x && PlayerPos.x + PlayerHalhSize.x < BlockRightLine)) {
			
			/** @brief �v���C���[����ɏ������*/
			if (BlockUpLine <= m_OldPos.y - PlayerHalhSize.y &&					//�O�t���[���͂߂荞��ł��Ȃ�
							   PlayerPos.y - PlayerHalhSize.y < BlockUpLine) {	//���݂̃t���[���͂߂荞��ł���
				//y�̑��x���Ȃ���
				m_pPlayer->Vel.y = 0.0f;

				//���W�␳
				m_pPlayer->Pos.y = BlockUpLine + PlayerHalhSize.y - PlayerOffset.y;	

				//�v���C���[�̏�Ԃ�n�ʂɗ����Ă����Ԃɕς���
				m_ePlayer = IDLE_PLAYER;

				//�v���C���[�̍ŐV��Ԃ�ۑ�����
				if (m_bJump == true) {
					g_ePlayer = m_ePlayer;
					m_bJump = false;
				}
			}
			/** @brief �������񂱏�Ԃł���*/
			else if (BlockDownLine >= m_OldPos.y + PlayerHalhSize.y &&	//�O�t���[���͂߂荞��ł��Ȃ�
				PlayerPos.y + PlayerHalhSize.y > BlockDownLine) {		//���݂̃t���[���͂߂荞��ł���
				   //y�̑��x���Ȃ���
				m_pPlayer->Vel.y = 0.0f;
				//���W�␳
				m_pPlayer->Pos.y = BlockDownLine - PlayerHalhSize.y - PlayerOffset.y;
			}
		}

		//�������x�����𒴂����痎������ɂ���
		if (m_pPlayer->Vel.y == -MAX_VELOCITY) {
			int i = 1;
		}
		else {
			int i = 1;
		}

		//���S���W�̃Z�b�g
		PlayerPos = Player->GetCenterPos();

		/** @brief �v���C���[���u���b�N�̒�(��[�Ɖ��[�̒�)�ɂ����ꍇ�̏���*/
		if ((BlockDownLine < PlayerPos.y && PlayerPos.y < BlockUpLine) ||
			(BlockDownLine < PlayerPos.y - PlayerHalhSize.y && PlayerPos.y - PlayerHalhSize.y < BlockUpLine) ||
			(BlockDownLine < PlayerPos.y + PlayerHalhSize.y && PlayerPos.y + PlayerHalhSize.y < BlockUpLine)) {
			//�v���C���[���E�̃u���b�N�ɓ��������ꍇ
			if (BlockLeftLine >= m_OldPos.x + PlayerHalhSize.x &&		//�O�t���[���͂߂荞��ł��Ȃ�
				PlayerPos.x + PlayerHalhSize.x > BlockLeftLine) {		//���݂̃t���[���͂߂荞��ł���
				//�ړ��ʂ��폜����
				m_pPlayer->Vel.x = 0.0f;

				//���W�␳
				m_pPlayer->Pos.x = BlockLeftLine - PlayerHalhSize.x - PlayerOffset.x;
			}
			else if (BlockRightLine <= m_OldPos.x - PlayerHalhSize.x &&
				PlayerPos.x - PlayerHalhSize.x < BlockRightLine) {
				//�ړ��ʂ��폜����
				m_pPlayer->Vel.x = 0.0f;

				//���W�̕␳
				m_pPlayer->Pos.x = BlockRightLine + PlayerHalhSize.x - PlayerOffset.x;
			}
		}
		return;
	}
}

/**
* @fn		CPlayer::SetPlayerStatus
* @brief	�v���C���[�̏�Ԃ��Z�b�g����
* @detail	�C�x���g�̎��Ƀv���C���[���X�g�b�v������ĊJ������
* @param	(PLAYER_STATUS)	�w�肵�����v���C���[�̏��
*/
void CPlayer::SetPlayerState(PLAYER_STATE PlayerSta) {
	m_ePlayer = g_ePlayer = PlayerSta;
}