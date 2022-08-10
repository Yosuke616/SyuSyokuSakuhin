/**
* @file		AttackComponent.h
* @brief	�v���C���[��G�̍U������������֐�
* @author	�u���z�S
* @date		2022/8/10
*/

/**�C���N���[�h�K�[�h**/
#ifndef _ATTACK_COMPONENT_H_
#define _ATTACK_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CCollider;
class CDrawMesh;

/**�񋓑̐錾**/
/** �N���U�����Ă��邩�𔻕ʂ���񋓑�*/
enum WHO_ATTACK {
	NONE_ATTACK = 0,
	PLAYER_ATTACK,

	MAX_WHO_ATTACK
};

/**�N���X��`**/
/**
* @brief	�U���̓����𐧌䂷��
* @detail	�U�����s�����҂̂���ċ�����ς���
*/
/**�K�v�Ȃ������ȕϐ��͏����Ă���**/
class CAttack :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�Ƃ��̏��*/
	CTransform* m_pTrans;
	/** @brief  ���E�̂ǂ��炩��U�����ꂽ���𔻕ʂ���ϐ�*/
	/** @detail true�͉E����*/
	bool m_bRoL;
	/** @brief �ǂ̃I�u�W�F�N�g�ɂ��U����*/
	WHO_ATTACK m_eAttack;
	/** @brief �U���p�̓����蔻��̑��݂��Ă�������*/
	int m_nAttackCnt;
	/** @brief �v���C���[�̃I�u�W�F�N�g�𓾂�*/
	Object* m_pObj;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CAttack();
	/** @brief �f�X�g���N�^*/
	~CAttack()override;
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief ���E�ǂ��炩��̍U�������擾����*/
	bool GetAttackRoL();
	/** @brief �ǂ̎�ނ̃I�u�W�F�N�g�ɂ��U������ݒ肷��*/
	void SetAttackType(WHO_ATTACK who);
	/** @brief ���E�ǂ����������Ă��邩�Z�b�g����֐�*/
	void SetRoL(bool RoL);
	/** @brief �I�u�W�F�N�g�̏����擾����*/
	void SetObject(Object* pObj);

};

#endif