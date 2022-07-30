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

/**�񋓑̐錾**/
enum PLAYER_STATE {
	IDLE_PLAYER = 0,
	RUN_PLAYER,
	JUMP_PLAYER,
	FALL_PLAYER,

	MAX_PLAYER
};

//�N���X��`
class CPlayer :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�A�g�k�A��]*/
	CTransform* m_pPlayer;
	/** @brief �`����*/
	CDraw2D*    m_pDraw2D;
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
	/** @brief �W�����v�������ǂ����̃t���O*/
	bool m_bJump;

	//�����o�֐�

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
};


#endif