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
};

#endif