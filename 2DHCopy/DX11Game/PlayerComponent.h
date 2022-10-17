/**
* @file		PlayerComponent.h
* @brief	�v���C���[�̑��������R���|�[�l���g
* @author	�u���z�S
* @date		2022/7/14
*/

//�C���N���[�h�K�[�h
#ifndef _PLAYER_COMPONENT_H_
#define _PLAYER_COMPONENT_H_

//�C���N���[�h��
#include "Component.h"

//�O���錾
class InputManager;
class CTransform;
class Object;
class CCollider;
class CDraw2D;
class CAnimMesh;

/**�񋓑̐錾**/
enum PLAYER_STATE {
	IDLE_PLAYER = 0,
	START_PLAYER,		//�X�^�[�g���Ĉړ�������
	RUN_PLAYER,			//���ړ�
	DUSH_PLAYER,		//�G�A�_�b�V���������̂���肽���i����ĂȂ��j
	JUMP_PLAYER,		//�W�����v���Ă���
	ATTACK_PLAYER,		//�U��
	FALL_PLAYER,		//�������Ă���
	HIT_PLAYER,			//�G�Ƀq�b�g�����Ƃ�
	MISS_PLAYER,		//������Q�[���I�[�o�[
	TIMEOUT_PLAYER,		//���Ԑ؂�ɂȂ����Ƃ��̉��o
	CLEAR_PLAYER,		//�N���A�������
	
	STOP_PLAYER,		//�v���C���[�̑����~�߂����Ƃ�

	MAX_PLAYER
};

//�N���X��`
class CPlayer :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�A�g�k�A��]*/
	CTransform* m_pPlayer;
	/** @brief �`����*/
	CAnimMesh*    m_pDraw2D;
	/** @brief �����蔻��*/
	CCollider* m_pCollider;
	/** @brief �C���v�b�g�}�l�[�W���[�̎擾*/
	InputManager* m_pInput;
	/** @brief �ߋ����W�̕ۑ�*/
	XMFLOAT2 m_OldPos;
	/** @brief �n��ɂ��邩�ǂ����̃t���O*/
	bool m_bGround;
	/** @brief �v���C���[�̏�Ԃ𔻕ʂ���񋓑�*/
	PLAYER_STATE m_ePlayer;
	/** @brief �W�����v���������ǂ����̃t���O*/
	bool m_bJump;
	/** @brief �p���[�A�b�v���Ă��邩�ǂ����̃t���O*/
	bool m_bPawer_UP;
	/** @brief ���G����*/
	int m_nStar_Time;
	/** @brief �E�������Ă��邩���������Ă��邩(true)�͉E*/
	bool m_bROL;
	/** @brief �ǂ������瓖���������𔻕ʂ���(�f�t�H���g��0)*/
	int m_nHitVec;
	/** @brief  �q�b�g�����Ƃ���x�����͂������đ��x�����炷���߂̃t���O*/
	bool m_bHitFlg;
	/** @brief �U�������Ă��邩�ǂ����̔���*/
	bool m_bAttack;
	/** @brief �U���ƍU���̃N�[���^�C���Ǘ��ϐ�*/
	int m_nCoolTime;
	/** @brief �����蔻��p�I�u�W�F�N�g���w���Ēu����|�C���^*/
	Object* m_pAttackObj;

	//���ɓ������Ă��邩�̔���(����)
	bool bHitObj = false;

	/** @brief �G�A�_�b�V���̒����p�̃J�E���^*/
	int m_nDushCnt;
	/** @brief �G�A�_�b�V���̃N�[���^�C��*/
	int m_nDushCoolTime;
	/** @brief  �󒆂ł̃G�A�_�b�V������*/
	/** @detail true�ł�����x�o���Ȃ�*/
	bool m_bAirDushFlg;
	/** @brief  �G�ɓ������ă~�X������������������*/
	/** @detail �G�ɓ������Ď��񂾏ꍇfalse,�������̏ꍇtrue*/
	bool m_bAirDead;
	/** @brief �G�ɂԂ����莞�ԗ���������Q�[���I�[�o�[���j���[���o��*/
	int m_nMissCnt;
	/** @brief �X�e�[�W�Z���N�g�ɍs�����߂̎���(�����f�B���C)*/
	bool m_bClearFlg;

	/** @brief �A�C�e�����擾�����Ƃ��ɐݒ肷��*/
	Object* m_pMaga;
	/** @bried ���D���擾���Ă��邩*/
	bool m_bOhuda;

	/** @brief ���[�v�n�_�ɂ���Ƃ��̓W�����v�ł��Ȃ�����*/
	bool m_bWarpPoint;

	//�����o�֐�
	/** @brief �U���{�^���������ꂽ�ۂɐ�p�I�u�W�F�N�g�̐���*/
	void CreateAttack();
	/** @brief �v���C���[���ǂ̕�������G�ɓ����������𔻕�*/
	int CollEnemy(Object* pObject);

	/** @brief �G�ꂽ���ǂ����ɂ��Ă̔���*/
	bool CollBox(Object* pObject);

protected:
	//�����o�ϐ�

	//�����o�֐�
	
public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CPlayer();
	/** @brief �f�X�g���N�^*/
	~CPlayer() override;
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V*/
	void Update() override;
	/** @brief �`��*/
	void Draw() override;
	/** @brief �Փˏ���*/
	void OnCollisionEnter(Object* pObject) override;
	/** @brief �v���C���[�̏�Ԃ�ύX���邽��*/
	void SetPlayerState(PLAYER_STATE PlayerSta);
	/** @brief �v���C���[�̃e�N�X�`����ύX���邽�߂̊֐�*/
	void ChangeTexture();
	/** @brief �v���C���[���ǂ����̕��������߂Ă��邩��*/
	bool GetPlayerROL();
	/** @brief �U����̃N�[���^�C����ݒ肷��*/
	void SetCoolTime(int CoolTime);
	/** @brief �ĂэU���ł���悤�ɂ���*/
	void SetAttackFlg(bool Attack);
	/** @brief �v���C���[���W�����v���Ă��邩���擾����*/
	bool GetPlayerJump();
	/** @brief �v���C���[�̏�Ԃ��擾����֐�*/
	PLAYER_STATE GetPlayerSta();
	/** @brief �N���A�������ǂ������擾����֐�*/
	bool GetClearFlg();
	/** @brief ���D���擾�������ǂ������擾����*/
	bool GetOhuda();
	/** @brief ���[�v�|�C���g�ɐG�ꂽ���ǂ�����ݒ肷��*/
	void SetWarpPoint(bool bPoint);
};


#endif