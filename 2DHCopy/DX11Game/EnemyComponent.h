/**
* @file		EnemyComponent.h
* @brief	�G�̓����𐧌䂷��R���|�[�l���g
* @author	�u���z�S
* @date		2022/8/4
*/

/**�C���N���[�h�K�[�h**/
#ifndef _ENEMY_COMPONENT_H_
#define _ENEMY_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CCollider;
class CDraw3D;

/**�񋓑̐錾**/
//�G�̎��
enum ENEMY_TYPE {
	ENEMY_WALK = 0,

	MAX_ENEMY_TYPE
};

//�G�̏��
enum ENEMY_STATE {
	ENEMY_DEFAULT = 0,
	ENEMY_DELETE,

	MAX_ENEMY_STATE
};

/**�N���X��`**/
/**
* @brief	�G�̓����𐧌䂷��
* @detail	�U������������Ԃ�����
*/
class CEnemy :public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�A�g�k�A��]*/
	CTransform* m_pTransform;
	/** @brief �`����*/
	CDraw3D* m_pDraw;
	/** @brief �����蔻��*/
	CCollider* m_pCollider;
	/** @brief ���E�̂ǂ���������Ă��邩(true�������獶�֍s��)*/
	bool m_bRightorLeft;
	/** @brief �ߋ����W�̕ۑ�*/
	XMFLOAT2 m_OldPos;
	/** @brief �G�̎�ނ��Ǘ�����ϐ�*/
	ENEMY_TYPE m_eEnemy_Type;
	/** @brief �G�̏�Ԃ��Ǘ�����ϐ�*/
	ENEMY_STATE m_eEnemy_State;
	/** @brief ��x�����R���|�[�l���g�̋@�\���~����ׂ̕ϐ�*/
	bool m_bStopCom;
	/** @brief �v���C���[�̍U�����E���瓖�������������瓖���������𔻕ʂ���*/
	bool m_bRoL;
	/** @brief  �r���ő��x��؂�グ��悤�ɂ���*/
	/** @detail true�ɂ��ăt���O���I���ɂ���*/
	bool m_bStopVel;
	/** @brief �_�ŗp�̕ϐ�*/
	int m_nBlink;
	/** @brief ����Ȃ�I�u�W�F�N�g����ϐ�*/
	int m_nDeleteFlg;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CEnemy();
	/** @brief �f�X�g���N�^*/
	~CEnemy()override;
	/** @brief ������*/
	void Start()override;
	/** @brief �X�V����*/
	void Update()override;
	/** @brief �`�揈��*/
	void Draw()override;
	/** @brief �����蔻��̏���*/
	void OnCollisionEnter(Object* pObject)override;
	/** @brief �G�̎�ނ̐ݒ�*/
	void SetEnemyType(ENEMY_TYPE type);
};

#endif