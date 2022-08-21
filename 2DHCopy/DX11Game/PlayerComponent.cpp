//�C���N���[�h��
#include "PlayerComponent.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "TextureManager.h"
#include "imgui.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "ColliderComponent.h"
#include "GravityComponent.h"
#include "sceneGame.h"
#include "Camera.h"
#include "SeeCollComponent.h"
#include "AttackComponent.h"

//�萔��`
/** @brief*/
#define MAX_PLAYER_GRAVITY		(1.0f)	//�v���C���[�ɉ����d�͂̍ő�l	
#define VALUE_JUMP_SPEED		(8.0f)	//�W�����v�̑���
#define VALUE_MOVE_SPEED		(2.0f)	//�ړ��X�s�[�h
#define STAR_TIME				(180)	//���G����
#define HIT_SPEED				(8.0f)	//�Ԃ���΂��ꂽ���̐���
#define KNOCK_BACK_PLAYER		(0.05f)	//�߂�����

/**�O���[�o���ϐ�**/
PLAYER_STATE g_ePlayer;

/**
* @fn		CPlayer::CPlayer
* @brief	���̖��O�̓R���X�g���N�^�@�����̃R���X�g���N�^
* @detail	�g�����X�t�H�[����`������C�j�V�����C�Y��null�ɂ���
*/
CPlayer::CPlayer() :
	m_pPlayer(nullptr),m_pDraw2D(nullptr),m_pInput(nullptr),
	m_OldPos(0.0f,0.0f),					//�ߋ����W
	m_nStar_Time(0),						//���G����
	m_bGround(false),						//�n��ɂ��邩�ǂ���
	m_bJump(false),							//�W�����v���Ă��邩�ǂ���
	m_bPawer_UP(true),						//�p���[�A�b�v���Ă��邩
	m_pAttackObj(nullptr),					//�U������p�̃I�u�W�F�N�g���w����|�C���^
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
	m_pDraw2D = Parent->GetComponent<CAnimMesh>();
	m_pPlayer = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();
	m_pInput = InputManager::Instance();
	g_ePlayer = m_ePlayer;
	//�p���[�A�b�v�Ɩ��G���Ԃ̏�����
	m_bPawer_UP = true;
	m_nStar_Time = 0;
	//���߂͉E�������Ă���
	m_bROL = true;
	//�ǂ��炩��q�b�g�������𔻕ʂ���(0�͉��ɂ��������Ă��Ȃ�)
	m_nHitVec = 0;
	//�G�Ƀq�b�g�����Ƃ��ɉ�����͂̏�����̕ϐ�
	m_bHitFlg = false;
	//�U���t���O�̓I�t�ɂ��Ă���
	m_bAttack = false;
	//�U�����̃N�[���^�C��
	m_nCoolTime = 0;
	//�G�A�_�b�V���̎��Ԃ�0�ɂ��Ă���
	m_nDushCnt = 0;
	//�G�A�_�b�V���̃N�[���^�C����0�ɂ��Ă���
	m_nDushCoolTime = 0;
	//�󒆃t���O�̓I�t�ɂ��Ă���
	m_bAirDushFlg = false;
	//�G�ɂ�莀�S������
	m_bAirDead = false;
	//�~�X�J�E���g��0�ɂ��Ă���
	m_nMissCnt = 0;
}

/**
* @fn		CPlayer::Update
* @brief	�X�V����
*/
void CPlayer::Update() {
	m_OldPos = m_pCollider->GetCenterPos();

	//�X�e�[�^�X�̏����X�V����
	m_ePlayer = g_ePlayer;

	//���E�ړ��̃{�^����������Ă����ꍇ���E�̃t���O�����ւ���
	//�G�A�_�b�V���t���O���Ă���Œ��͕ς��Ȃ��悤�ɂ���
	if (!(m_ePlayer == DUSH_PLAYER)) {
		if (m_pInput->GetKeyPress(DIK_D)) {
			//�E����
			m_bROL = true;
		}
		if (m_pInput->GetKeyPress(DIK_A)) {
			//������
			m_bROL = false;
		}	
	}

	//����
	bHitObj = false;


	//���G��Ԃ������甼�����ɂ���
	if (m_nStar_Time > 0) {
		m_pDraw2D->SetColor(1.0f,0.0f,0.0f);
		m_nStar_Time--;
	}
	else
	{
		m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
	}

	//�������x��0�ɂ���
#ifdef _DEBUG
	if (m_pInput->GetKeyTrigger(DIK_0)) {
		m_pPlayer->Vel.y = 0;
	}
#endif


	switch (m_ePlayer) {
	case IDLE_PLAYER:
#pragma region ---�ҋ@
		//���ʂȃA�N�V�������N�������ꍇ�A�v���C���[�̏�Ԃ�ς���

		//���ړ��͂Ȃ��ɂ���,�c�͏d�͂�������
		m_pPlayer->Vel.x = 0.0f;

		if (m_pInput->GetKeyPress(DIK_D)) {
			//�E�ړ�
			m_ePlayer = RUN_PLAYER;
			ChangeTexture();
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_ePlayer = RUN_PLAYER;
			ChangeTexture();
		}

		//�G�A�_�b�V��
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			m_ePlayer = DUSH_PLAYER;
			ChangeTexture();
			//�d�͂̋@�\���I�t�ɂ���
			Parent->GetComponent<CGravity>()->SetUse(false);
		}

		//�W�����s���O
		if (m_ePlayer == IDLE_PLAYER) {
			if (m_pInput->GetKeyPress(DIK_W)) {
				//�X�e�[�^�X���W�����v�ɕς���
				m_ePlayer = JUMP_PLAYER;
				ChangeTexture();
			}
		}

		//�U������
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//�v���C���[�̏�Ԃ��U���ɕύX����
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}
		break;
#pragma endregion
	case RUN_PLAYER:
#pragma region ---�ړ�
		if (m_pInput->GetKeyPress(DIK_D)) {
			//�E�ړ�
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_ePlayer = IDLE_PLAYER;
			ChangeTexture();
		}

		//���x�Ńe�N�X�`����ς���
		if(m_pPlayer->Vel.x >= 0) {
			m_bROL = true;
			ChangeTexture();
		}
		else {
			m_bROL = false;
			ChangeTexture();
		}

		//�G�A�_�b�V��
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			m_ePlayer = DUSH_PLAYER;
			ChangeTexture();
			//�d�͂̋@�\���I�t�ɂ���
			Parent->GetComponent<CGravity>()->SetUse(false);
		}

		//�W�����s���O	
		if (m_pInput->GetKeyPress(DIK_W)) {
			//�X�e�[�^�X���W�����v�ɕς���
			m_ePlayer = JUMP_PLAYER;
			ChangeTexture();
		}
		//�U������
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//�v���C���[�̏�Ԃ��U���ɕύX����
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}
	
		break;

#pragma endregion
	case DUSH_PLAYER:
#pragma region ---�G�A�_�b�V��
		//�N�[���^�C������������ł���悤�ɂ���
		if (m_nDushCoolTime <= 0) {
			
			//�󒆂ɂ���ꍇ
			//�v���C���[�ɉ�����Ă���㉺�̗͂��Ȃ���
			if (m_bJump) {
				m_pPlayer->Vel.y = 0.0f;
			}

			//�G�A�_�b�V���������鎞�Ԃ̊Ǘ�
			m_nDushCnt++;
			
			//���E�̌����łǂ����ɑ��x�������邩�����߂�
			if (m_bROL) {
				m_pPlayer->Vel.x = VALUE_MOVE_SPEED * 2;
			}
			else {
				m_pPlayer->Vel.x = -VALUE_MOVE_SPEED * 2;
			}

			//�G�A�_�b�V�����I������Ƃ��̏���
			//�󒆂ɂ��邩�ǂ����Ńv���C���[�̏�Ԃ͕ς���
			if (m_nDushCnt > 30) {
				if (m_bJump) {
					//�󒆂̏ꍇ
					//�v���C���[�̏�Ԃ𗎉���Ԃɂ���
					m_ePlayer = FALL_PLAYER;
					//�v���C���[�̑��x��ς���
					m_pPlayer->Vel.x = 0.0f;
					//�v���C���[�̏d�͂��I���ɂ���
					Parent->GetComponent<CGravity>()->SetUse(true);
					//�G�A�_�b�V���f�t�H���g�ɖ߂�(����������)
					m_nDushCnt = 0;
					//�e�N�X�`���̕ύX
					ChangeTexture();
					//�N�[���^�C���̐ݒ�
					m_nDushCoolTime = 15;
				}
				else {
					//�n��̏ꍇ
					//�v���C���[�̏�Ԃ��f�t�H���g�ɕύX����
					m_ePlayer = IDLE_PLAYER;
					//�v���C���[�̑��x��ς���
					m_pPlayer->Vel.x = 0.0f;
					//�G�A�_�b�V�����Ԃ��f�t�H���g�ɖ߂�(����������)
					m_nDushCnt = 0;
					//�e�N�X�`���̕ύX
					ChangeTexture();
					//�N�[���^�C���̐ݒ�
					m_nDushCoolTime = 15;
					//�v���C���[�̏d�͂��I���ɂ���
					Parent->GetComponent<CGravity>()->SetUse(true);
				}
			}	
		}
		else {
			//�����ĂȂ��ꍇ�f�t�H���g��Ԃɖ߂�
			m_ePlayer = IDLE_PLAYER;
		}


		break;
#pragma endregion
	case JUMP_PLAYER:
#pragma region ---�W�����v
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
			//�E�ړ�
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_pPlayer->Vel.x = 0.0f;
		}

		//�G�A�_�b�V��
		//�󒆂ł̃G�A�_�b�V���̏����͏����ς���
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			if (!m_bAirDushFlg) {
				m_ePlayer = DUSH_PLAYER;
				ChangeTexture();
				//�󒆂œ�x�g�p�ł��Ȃ��悤�ɂ���
				m_bAirDushFlg = true;
				//�d�͂̋@�\���I�t�ɂ���
				Parent->GetComponent<CGravity>()->SetUse(false);
			}
		}

		//�U������
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//�v���C���[�̏�Ԃ��U���ɕύX����
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}

		break;
#pragma endregion
	case ATTACK_PLAYER:
#pragma region ---�U��
		//�N�[���^�C���̏ꍇ�͓���Ȃ�
		if(m_nCoolTime <= 0){
			//�t���O���I�t�������ꍇ�A
			//�U������t���̌����Ȃ��I�u�W�F�N�g�𐶐�����
			if (!m_bAttack) {
				CreateAttack();
				m_bAttack = true;
			}

			//�󒆂ōU�������ꍇ�����͂��̂܂܂ɂ���
			if (m_bJump) {
				//�W�����v�̏����̌p��������
				m_pPlayer->Vel.y -= GRAVITY;
				if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
					Parent->GetComponent<CGravity>()->SetUse(true);
				}

				//�󒆍U��
				//���E�ړ����ł���悤�ɂ���(�����͒x��)
				if (m_pInput->GetKeyPress(DIK_D)) {
					m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
					//�����蔻���������
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = VALUE_MOVE_SPEED;
				}
				else if (m_pInput->GetKeyPress(DIK_A)) {
					m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = -VALUE_MOVE_SPEED;
				}
			}
			else {
				//���E�ړ����ł���悤�ɂ���(�����͒x��)
				if (m_pInput->GetKeyPress(DIK_D)) {
					m_pPlayer->Vel.x = VALUE_MOVE_SPEED * 0.1;
					//�����蔻���������
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = VALUE_MOVE_SPEED * 0.1;
				}
				else if (m_pInput->GetKeyPress(DIK_A)) {
					m_pPlayer->Vel.x = -VALUE_MOVE_SPEED * 0.1;
					m_pAttackObj->GetComponent<CTransform>()->Vel.x = -VALUE_MOVE_SPEED * 0.1;
				}					
			}
		}
		else {
			//�v���C���[���W�����v���Ă��邩�ǂ����ŃX�e�[�^�X��ς���
			if (m_bJump) {
				m_ePlayer = JUMP_PLAYER;
				ChangeTexture();
			}
			else {
				m_ePlayer = IDLE_PLAYER;
				ChangeTexture();			
			}
		}

		break;
#pragma endregion
	case FALL_PLAYER:
#pragma region ---����
		//�������x�������Ȃ�����v���C���[�̏�Ԃ�ҋ@��Ԃɖ߂�
		if (m_pPlayer->Vel.y >= 0) {
			m_ePlayer = IDLE_PLAYER;
			ChangeTexture();
		}

		//�����t���O�̓I���ɂ���
		m_bJump = true;

		//���E�ړ��͂ł���悤�ɂ��Ă���
		if (m_pInput->GetKeyPress(DIK_D)) {
			//�E�ړ�
			m_pPlayer->Vel.x = VALUE_MOVE_SPEED;
		}
		else if (m_pInput->GetKeyPress(DIK_A)) {
			//���ړ�
			m_pPlayer->Vel.x = -VALUE_MOVE_SPEED;
		}
		else {
			m_pPlayer->Vel.x = 0.0f;
		}

		//�G�A�_�b�V��
		//��������󒆂ł̃G�A�_�b�V���Ȃ̂ŏ�����ς���
		if (m_pInput->GetKeyTrigger(DIK_E)) {
			if (!m_bAirDushFlg) {
				m_ePlayer = DUSH_PLAYER;
				ChangeTexture();
				m_bAirDushFlg = true;
				//�d�͂̋@�\���I�t�ɂ���
				Parent->GetComponent<CGravity>()->SetUse(false);			
			}
		}

		//�U������
		if (m_pInput->GetKeyTrigger(DIK_SPACE)) {
			//�v���C���[�̏�Ԃ��U���ɕύX����
			m_ePlayer = ATTACK_PLAYER;
			ChangeTexture();
		}

		break;
#pragma endregion
	case HIT_PLAYER:
#pragma region ---�m�b�N�o�b�N
		//�t���O��true�Ȃ��΂������Ɍ����ė͂�������
		if (!m_bHitFlg) {
			//�G�ɂԂ����Đ�����΂��ꂽ���̔���
			//�Ԃ����������Ƃ͋t�����ɑ��x��������
			//1:������A2:�ォ��A3:�E����A4:������
			switch (m_nHitVec)
			{
			case 1:
				//������ɗ͂�������
				m_pPlayer->Vel.y = HIT_SPEED;
				break;
			case 2:
				//�������ɗ͂�������
				m_pPlayer->Vel.y = -HIT_SPEED;
				break;
			case 3:
				//�������ɗ͂�������
				m_pPlayer->Vel.x = -HIT_SPEED;
				break;
			case 4:
				//�E�����ɗ͂�������
				m_pPlayer->Vel.x = HIT_SPEED;
				break;

			default:break;
			}	
			//�͂����炵�Ă��������ɐ؂�ւ���
			m_bHitFlg = true;
		}
		//��΂��������Ƃ͋t�����ɗ͂������đ��x�𗎂Ƃ��Ă���
		else if (m_bHitFlg) {
			switch (m_nHitVec) {
			case 1:
				m_pPlayer->Vel.y -= 0.1;
				if (m_pPlayer->Vel.y <= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
				}
				break;
			case 2:
				m_pPlayer->Vel.y += KNOCK_BACK_PLAYER;
				if (m_pPlayer->Vel.y >= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
				}
				break;
			case 3:
				m_pPlayer->Vel.x += KNOCK_BACK_PLAYER*5;
				if (m_pPlayer->Vel.x >= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
					//�v���C���[���p���[�A�b�v��ԂȂ�p���[�_�E��������
					if (m_bPawer_UP) {
						//�p���[�A�b�v��Ԃ���������
						m_bPawer_UP = false;					
					}
					else {
						//�~�X��Ԃɂ���
						m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);
						m_ePlayer = MISS_PLAYER;
						ChangeTexture();
					}
					
				}
				break;
			case 4:
				m_pPlayer->Vel.x -= KNOCK_BACK_PLAYER;
				if (m_pPlayer->Vel.x <= 0.0f) {
					m_nHitVec = 0;
					m_bHitFlg = false;
					m_ePlayer = FALL_PLAYER;
				}
				break;
			default:break;
			}
		}
		
		break;
#pragma endregion
	case MISS_PLAYER:
#pragma region ---�~�X
		//�v���C���[�ȊO�̃I�u�W�F�N�g���X�g�b�v����
		ObjectManager::GetInstance()->NoFunction();
		Parent->Use();
		for (auto&& block:ObjectManager::GetInstance()->GetUpdateList()) {
			if (block->GetName() == BLOCK_NAME) {
				block->Use();
			}
		}

		for (auto&& block : ObjectManager::GetInstance()->GetUpdateList()) {
			if (block->GetName() == ENEMY_NAME) {
				block->StopUpdate();
			}
		}

		//���������G�ɂ�莀�S���ύX����
		//true�ŗ�����
		if (m_bAirDead) {
			//������
			//�����񂪂�����x��������Q�[���I�[�o�[�̃��j���[���o��������
			MenuManager::GetInsutance()->Create(GAMEOVER_STATE, 25);

			//�v���C���[�̍X�V���~�߂�
			Parent->StopUpdate();

			//�V�[���Q�[���̃I�v�V�����t���O���I���ɂ���
			SceneGame::GetInstance()->SetPauseOOO(true);
		}
		else {
			//�G�ɂ�莀�S			
			//�~�X�J�E���^�[�𑝂₵���ŃQ�[���I�[�o�[���j���[���o��
			m_nMissCnt++;
			if (m_nMissCnt > 90) {
				//�����񂪂�����x��������Q�[���I�[�o�[�̃��j���[���o��������
				//�e�N�X�`���������I�������
				MenuManager::GetInsutance()->Create(GAMEOVER_STATE, 25);

				//�v���C���[�̍X�V���~�߂�
				Parent->StopUpdate();

				//�V�[���Q�[���̃I�v�V�����t���O���I���ɂ���
				SceneGame::GetInstance()->SetPauseOOO(true);
			}
		}
#pragma endregion
	default: break;
	}

	//�U���̃N�[���^�C���̓}�C�i�X���Ă���
	if (m_nCoolTime > 0) {
		m_nCoolTime--;
	}
	//�_�b�V���̃N�[���^�C�����}�C�i�X���Ă���
	if (m_nDushCoolTime > 0) {
		m_nDushCoolTime--;
	}

	//�v���C���[�̍ŐV��Ԃ�ۑ�����
	g_ePlayer = m_ePlayer;

	//���x�̏�������߂�
	if (m_pPlayer->Vel.x > VALUE_MOVE_SPEED * 2) {
		m_pPlayer->Vel.x = VALUE_MOVE_SPEED * 2;
	}
	if (m_pPlayer->Vel.x < -VALUE_MOVE_SPEED * 2) {
		m_pPlayer->Vel.x = -VALUE_MOVE_SPEED * 2;
	}
	if (m_pPlayer->Vel.y < -MAX_VELOCITY) {
		m_pPlayer->Vel.y = -MAX_VELOCITY;
	}

	/*if (m_pPlayer->Vel.y > VALUE_MOVE_SPEED) {
		m_pPlayer->Vel.y = VALUE_MOVE_SPEED;
	}*/
#if _DEBUG
	if (m_pInput->GetKeyPress(DIK_UPARROW)) {
		//��ړ�
		m_pPlayer->Pos.y += 5.0f;
	}	
	if (m_pInput->GetKeyPress(DIK_DOWNARROW)) {
		//���ړ�
		m_pPlayer->Pos.y += -5.0f;
	}
	if (m_pInput->GetKeyPress(DIK_RIGHTARROW)) {
		//�E�ړ�
		m_pPlayer->Pos.x += 5.0f;
	}
	if (m_pInput->GetKeyPress(DIK_LEFTARROW)) {
		//�E�ړ�
		m_pPlayer->Pos.x += -5.0f;
	}

	//�e�N�X�`���̕ύX
	if (m_pInput->GetKeyTrigger(DIK_1)) {
		//�f�t�H���g����
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_STAND_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
		m_pDraw2D->SetAnimSplit(3,3);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y,DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(0.0f,0.0f);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(),m_pCollider->GetOffSet());
	}
	if (m_pInput->GetKeyTrigger(DIK_2)) {
		//�f�t�H���g����
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_RUN_Y);
		m_pDraw2D->SetAnimSplit(7,2);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_RUN_X, DXCHAN_COLL_SIZE_Y,DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(DXCHAN_COLL_OFFSET_RUN_X, DXCHAN_COLL_OFFSET_RUN_Y);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(),m_pCollider->GetOffSet());
	}
	if (m_pInput->GetKeyTrigger(DIK_P)) {
		g_ePlayer = IDLE_PLAYER;
		m_nHitVec = 0;
		m_bPawer_UP = true;
	}

	//�d�̗͂L��
	if (m_pInput->GetKeyTrigger(DIK_G)) {
		Parent->GetComponent<CGravity>()->m_bUpdateFlag = false;
	}
	if (m_pInput->GetKeyTrigger(DIK_H)) {
		Parent->GetComponent<CGravity>()->m_bUpdateFlag = true;
	}
#endif
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
	Text("RoL	  : %d",m_bROL);
	Text("P_STATE : %d",g_ePlayer);
	Text("DCool   : %d",m_nDushCoolTime);
	Text("ADFlg   : %d",m_bAirDushFlg);
	End();
#endif // _DEBUG
}

/**
* @fn		CPlayer::OnCollisionEnter
* @brief	�v���C���[�������ɓ��������Ƃ��̏���
* @param	(Object*)	������̃I�u�W�F�N�g�̃|�C���^
*/
void CPlayer::OnCollisionEnter(Object* pObject) {
	//����
	bHitObj = true;
#pragma region ---BLOCK
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

				//�G�̍U���Ƀq�b�g���Ă���ꍇ��Ԃ͕ς��Ȃ�
				if (m_ePlayer == HIT_PLAYER||m_ePlayer == MISS_PLAYER) {
				
				}
				else {
					//�v���C���[�̏�Ԃ�n�ʂɗ����Ă����Ԃɕς���
					m_ePlayer = IDLE_PLAYER;				
				}
				
				//�v���C���[�̏�Ԃ��~�X�ɂȂ��Ă���ꍇ�Q�[���I�[�o�[�ɂ���
				if (m_ePlayer == MISS_PLAYER) {
					//�v���C���[�ȊO�̃I�u�W�F�N�g���X�g�b�v����
					ObjectManager::GetInstance()->NoFunction();
					Parent->Use();
					for (auto&& block : ObjectManager::GetInstance()->GetUpdateList()) {
						if (block->GetName() == BLOCK_NAME) {
							block->Use();
						}
					}

					for (auto&& block : ObjectManager::GetInstance()->GetUpdateList()) {
						if (block->GetName() == ENEMY_NAME) {
							block->StopUpdate();
						}
					}

					m_pDraw2D->SetColor(1.0f, 1.0f, 1.0f);

					//�����񂪂�����x��������Q�[���I�[�o�[�̃��j���[���o��������
					//�e�N�X�`���������I�������
					MenuManager::GetInsutance()->Create(GAMEOVER_STATE, 25);

					//�v���C���[�̍X�V���~�߂�
					Parent->StopUpdate();

					//�V�[���Q�[���̃I�v�V�����t���O���I���ɂ���
					SceneGame::GetInstance()->SetPauseOOO(true);

				}

				//�v���C���[�̍ŐV��Ԃ�ۑ�����
				if (m_bJump == true) {
					g_ePlayer = m_ePlayer;
					m_bJump = false;
					m_bAirDushFlg = false;
					Parent->GetComponent<CGravity>()->SetUse(true);
					ChangeTexture();
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
		//if (m_pPlayer->Vel.y == -MAX_VELOCITY) {
		//	int i = 1;
		//}
		//else {
		//	int i = 1;
		//}

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
	}
#pragma endregion

#pragma region ---ENEMY
	//���G���Ԃ̏ꍇ�G�Ƃ͓�����Ȃ�
	if (m_nStar_Time <= 0) {
		//�G�ƕ��ނ������̂ɓ��������ꍇ
		if (pObject->GetName() == ENEMY_NAME) {
			//�p���[�A�b�v��Ԃ������łȂ����ŏ������e�����߂�
			if (m_bPawer_UP) {
				//������΂����������߂�
				m_nHitVec = CollEnemy(pObject);

				if (m_nHitVec) {
					////�p���[�A�b�v���Ă���Ƃ������Ƃ͋����Ȃ��Ă���Ƃ������Ƃł���
					//���G�J�E���g�𑝂₷
					m_nStar_Time = STAR_TIME;	
					//�v���C���[�̃X�e�[�^�X��ς���
					//�e�N�X�`�����ς���
					m_ePlayer = HIT_PLAYER;
					g_ePlayer = m_ePlayer;
					ChangeTexture();
				}
			}
			else {
				//����������~�X��Ԃł���
				if (CollEnemy(pObject)) {
					//������΂����������߂�
					m_nHitVec = CollEnemy(pObject);
					m_ePlayer = HIT_PLAYER;
					g_ePlayer = m_ePlayer;
					//���G�J�E���g�𑝂₷
					m_nStar_Time = 999;
					//�e�N�X�`����ς���
					ChangeTexture();
				}
			}

			//��肽������
			//�p���[�A�b�v��ԂŖ���������~�X����

			//�~�X����@�v���C���[�ȊO�̃I�u�W�F�N�g�̍X�V���~�߂ă~�X�̃e�N�X�`���𗬂�
			//�e�N�X�`���𗬂��I�����烁�j���[��ʂ̕\��(�Q�[���I�[�o�[���j���[)
			
		
		
		}
	}
	
#pragma endregion

#pragma region ---�~�X����
	if (pObject->GetName() == "MISS_COLL") {
		//���Ȃ��Ƃ����Ȃ�����
		//�~�X�p�̃��j���[���쐬����
		m_ePlayer = MISS_PLAYER;
		g_ePlayer = m_ePlayer;
	}
#pragma endregion
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

/**
* @fn		CPlayer::CollEnemy
* @brief	�G�Ƃǂ̌������瓖���������Ő�����΂����������߂�
* @detail	1:������A2:�ォ��A3:�E����A4:������
* @param	(Object*)	���������G�̏�񂪂͂����Ă���|�C���^
* @return	(int)		�ǂ������炠��������Ԃ�
*/
int CPlayer::CollEnemy(Object* pObject) {
#pragma region ---�㉺���E4��������̓����蔻��
	////�v���C���[�̏����擾
	//auto Player = Parent->GetComponent<CCollider>();
	//auto PlayerPos = Player->GetCenterPos();
	//auto PlayerSize = Player->GetColliderSize();
	//auto PlayerOffset = Player->GetOffSet();

	////�Ԃ������u���b�N�̏��
	//auto Enemy = pObject->GetComponent<CCollider>();
	//auto EnemyPos = Enemy->GetCenterPos();
	//auto EnemySize = Enemy->GetColliderSize();
	//auto EnemyOffset = Enemy->GetOffSet();

	////���ꂼ�ꔼ���̑傫����ۑ�
	//XMFLOAT2 PlayerHalhSize = XMFLOAT2(PlayerSize.x * 0.5f, PlayerSize.y *0.5f);
	//XMFLOAT2 EnemyHalhSize = XMFLOAT2(EnemySize.x * 0.5f, EnemySize.y * 0.5f);

	////�u���b�N�̓����蔻��̐�
	//float EnemyLeftLine = EnemyPos.x - EnemyHalhSize.x;	//���[
	//float EnemyRightLine = EnemyPos.x + EnemyHalhSize.x;	//�E�[
	//float EnemyUpLine = EnemyPos.y + EnemyHalhSize.y;	//��[
	//float EnemyDownLine = EnemyPos.y - EnemyHalhSize.y;	//���[

	///** @brief �v���C���[���u���b�N�̒�(��[�Ɖ��[�̒�)�ɂ����ꍇ�̏���*/
	//if ((EnemyDownLine < PlayerPos.y && PlayerPos.y < EnemyUpLine) ||
	//	(EnemyDownLine < PlayerPos.y - PlayerHalhSize.y && PlayerPos.y - PlayerHalhSize.y < EnemyUpLine) ||
	//	(EnemyDownLine < PlayerPos.y + PlayerHalhSize.y && PlayerPos.y + PlayerHalhSize.y < EnemyUpLine)) {
	//	//�v���C���[���E�̃u���b�N�ɓ��������ꍇ
	//	if (EnemyLeftLine >= m_OldPos.x + PlayerHalhSize.x &&		//�O�t���[���͂߂荞��ł��Ȃ�
	//		PlayerPos.x + PlayerHalhSize.x > EnemyLeftLine) {		//���݂̃t���[���͂߂荞��ł���

	//		return 3;
	//	}
	//	else if (EnemyRightLine <= m_OldPos.x - PlayerHalhSize.x &&
	//		PlayerPos.x - PlayerHalhSize.x < EnemyRightLine) {

	//		return 4;
	//	}
	//}

	////���S���W�̃Z�b�g
	//PlayerPos = Player->GetCenterPos();

	///**
	//* @brief �v���C���[���u���b�N�̒�(���[�ƉE�[�̒�)�ɂ����ꍇ�̏���
	//*/
	//if ((EnemyLeftLine < PlayerPos.x && PlayerPos.x < EnemyRightLine) ||
	//	(EnemyLeftLine < PlayerPos.x - PlayerHalhSize.x && PlayerPos.x - PlayerHalhSize.x < EnemyRightLine) ||
	//	(EnemyLeftLine < PlayerPos.x + PlayerHalhSize.x && PlayerPos.x + PlayerHalhSize.x < EnemyRightLine)) {

	//	/** @brief �v���C���[����ɏ������*/
	//	if (EnemyUpLine <= m_OldPos.y - PlayerHalhSize.y &&					//�O�t���[���͂߂荞��ł��Ȃ�
	//		PlayerPos.y - PlayerHalhSize.y < EnemyUpLine) {					//���݂̃t���[���͂߂荞��ł���

	//		return 1;
	//	}
	//	/** @brief �������񂱏�Ԃł���*/
	//	else if (EnemyDownLine >= m_OldPos.y + PlayerHalhSize.y &&	//�O�t���[���͂߂荞��ł��Ȃ�
	//		PlayerPos.y + PlayerHalhSize.y > EnemyDownLine) {		//���݂̃t���[���͂߂荞��ł���

	//		return 2;
	//	}
	//}

	//return 0;
#pragma endregion

#pragma region ---�{�b�N�X�ł̓����蔻��
	//�v���C���[�̏����擾
	auto Player = Parent->GetComponent<CCollider>();
	auto PlayerPos = Player->GetCenterPos();
	auto PlayerSize = Player->GetColliderSize();
	auto PlayerOffset = Player->GetOffSet();

	XMFLOAT2 PlayerHalfSize = XMFLOAT2(PlayerSize.x / 2.0f, PlayerSize.y / 2.0f);
	//�����蔻��̑傫���������Ă���
	auto Enemy = pObject->GetComponent<CCollider>();
	auto TEnemy = pObject->GetComponent<CTransform>();
	XMFLOAT2 EnemyHalfSize = XMFLOAT2(Enemy->GetColliderSize().x / 2.0f,Enemy->GetColliderSize().y / 2.0f);

	if (Enemy->GetCenterPos().x - EnemyHalfSize.x + TEnemy->Vel.x < m_pCollider->GetCenterPos().x + PlayerHalfSize.x + m_pPlayer->Vel.x &&
		m_pCollider->GetCenterPos().x - PlayerHalfSize.x + m_pPlayer->Vel.x < Enemy->GetCenterPos().x + EnemyHalfSize.x + TEnemy->Vel.x) {

		if (Enemy->GetCenterPos().y - EnemyHalfSize.y + TEnemy->Vel.y < m_pCollider->GetCenterPos().y + PlayerHalfSize.y + m_pPlayer->Vel.y &&
			m_pCollider->GetCenterPos().y - PlayerHalfSize.y + m_pPlayer->Vel.y < Enemy->GetCenterPos().y + EnemyHalfSize.y + TEnemy->Vel.y) {

			return 3;
		}
	}
	return 0;
#pragma endregion
}

/**
* @fn		CPlayer::ChangeTexture
* @brief	�v���C���[�̃X�e�[�^�X���ς�����Ƃ��A�e�N�X�`����ς���֐�
*/
void CPlayer::ChangeTexture() {
	//�ǂ̃e�N�X�`���ɕύX���邩�̓v���C���[�̏�Ԃ����Č��߂�
	switch (m_ePlayer) {
	case IDLE_PLAYER:
		//�f�t�H���g����
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_STAND_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_Y);
		m_pDraw2D->SetAnimSplit(3, 3);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(0.0f, 0.0f);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(), m_pCollider->GetOffSet());
		break;
	case RUN_PLAYER: 
		//���E�ł����܂��傤�ˁ@
		//�f�t�H���g����
		m_pDraw2D->SetTexture(TextureManager::GetInstance()->GetTexture(DXCHAN_RUN_TEX_NUM));
		m_pDraw2D->SetSize(DXCHAN_SIZE_X, DXCHAN_SIZE_RUN_Y);
		m_pDraw2D->SetAnimSplit(7, 2);
		m_pDraw2D->SetVertex(m_bROL);
		m_pCollider->SetCollisionSize(DXCHAN_COLL_SIZE_RUN_X, DXCHAN_COLL_SIZE_Y, DXCHAN_COLL_SIZE_Z);
		m_pCollider->SetOffset(DXCHAN_COLL_OFFSET_RUN_X, DXCHAN_COLL_OFFSET_RUN_Y);
		Parent->GetComponent<CSeeColl>()->SetCollBox(m_pCollider->GetColliderSize(), m_pCollider->GetOffSet());
		break;
	case DUSH_PLAYER:m_ePlayer = m_ePlayer; break;
	case JUMP_PLAYER:m_ePlayer = m_ePlayer; break;
	case FALL_PLAYER:m_ePlayer = m_ePlayer; break;
	case HIT_PLAYER:
		break;
	case MISS_PLAYER:break;
	default:break;
	}	
}

/**
* @fn		CPlayer::CreateAttack
* @brief	�U������p�̃I�u�W�F�N�g�𐶐�����
*/
void CPlayer::CreateAttack() {
	//�I�u�W�F�N�g�̐���
	Object* obj = new Object(ATTACK_NAME,UPDATE_PLAYER,DRAW_PLAYER);
	//�R���|�[�l���g�̒ǉ�
	auto trans = obj->AddComponent<CTransform>();
	auto draw = obj->AddComponent<CDrawMesh>();
	auto coll = obj->AddComponent<CCollider>();
	auto attack = obj->AddComponent<CAttack>();
	obj->AddComponent<CSeeColl>();
	//�I�u�W�F�N�g�̐ݒ�@
	draw->SetTexture(TextureManager::GetInstance()->GetTexture(DEBUG_BLOCK_NUM));
	//�N�̍U������ݒ肷��
	attack->SetAttackType(PLAYER_ATTACK);
	attack->SetObject(ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME));
	//���E�ō��o���ꏊ��ύX����
	if (m_bROL) {
		trans->SetPosition(m_pPlayer->Pos.x+ OFFSET_DX_X,m_pPlayer->Pos.y + OFFSET_DX_Y);	
	}
	else {
		trans->SetPosition(m_pPlayer->Pos.x - OFFSET_DX_X, m_pPlayer->Pos.y + OFFSET_DX_Y);
	}
	attack->SetRoL(m_bROL);

	coll->SetCollisionSize(ATTACK_COLL_X, ATTACK_COLL_Y);
	//�I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	ObjectManager::GetInstance()->AddObject(obj);

	//�U���I�u�W�F�N�g���w���Ēu����|�C���^�Ŏw���Ă���
	m_pAttackObj = obj;
}

/**
* @fn		CPlayer::GetPlayerROL
* @brief	�v���C���[�����E�ǂ���������Ă��邩���擾����֐�
* @return	(bool)	�E�Ȃ��true�A���Ȃ�false��Ԃ�
*/
bool CPlayer::GetPlayerROL() {
	return m_bROL;
}

/**
* @fn		CPlayer::SetCoolTime
* @brief	�U����̃N�[���^�C���̐ݒ�
* @param	(int)	�ǂ̈ʂ������玟�̍U���Ɉڂ�邩
*/
void CPlayer::SetCoolTime(int Cooltime) {
	m_nCoolTime = Cooltime;
}

/**
* @fn		CPlayer::SetAttackFlg
* @brief	�U�����Ăтł���悤�ɂ���t���O
* @param	(bool)	�U�����Ă�����
*/
void CPlayer::SetAttackFlg(bool Attack) {
	m_bAttack = Attack;
}

/** 
* @fn		CPlayer::GetPlayerJump
* @brief	�W�����v���Ă��邩���擾����
* @return	(bool)	
*/
bool CPlayer::GetPlayerJump() {
	return m_bJump;
}

/**
* @fn		CPlayer::GetPlayerSta
* @brief	�v���C���[�̏�Ԃ��擾����
* @return	(PLAYER_STATE)	���݂̃v���C���[�̏�Ԃ�Ԃ�
*/
PLAYER_STATE CPlayer::GetPlayerSta() {
	return g_ePlayer;
}