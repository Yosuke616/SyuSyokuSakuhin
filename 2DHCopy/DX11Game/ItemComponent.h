/**
* @file		ItemComponent.h
* @brief	�X�R�A�A�b�v�A�p���[�A�b�v�A�L�[�A�C�e���̊Ǘ�
* @detail	�A�C�e���}�l�[�W���[�I�ȗv�f������
* @author	�u���z�S
* @date		2022/10/5
*/

/**�C���N���[�h�K�[�h**/
#ifndef _ITEM_COMPONENT_H_
#define _ITEM_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CCollider;
class CDraw3D;

/**�񋓑̐錾**/
enum ITEM_TYPE {
	ITEM_KOBAN,
	ITEM_MAGA,
	ITEM_OHUDA,

	MAX_ITEM
};

/**�N���X��`**/
/**
* @brief	�A�C�e���̓����𐧌䂷��R���|�[�l���g
* @detail	�}�l�[�W���[�I�Ȏg�����ŃZ�[�u�Ƃ�������
*/
class CItem : public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�Ƃ����߂܂�*/
	CTransform* m_pTransform;
	/** @brief ���f���Ƃ��̐ݒ�*/
	CDraw3D* m_pDraw;
	/** @brief �����蔻��̏��*/
	CCollider* m_pCollider;
	/** @brief �����̃A�C�e���̎��*/
	ITEM_TYPE m_eItem;
	/** @brief �㉺�Ɉړ������邽�߂̕ϐ�*/
	float m_fUpDown;
	/** @brief �㉺�Ɉړ�������t���O
			   true�ŏ�@false�ŉ�*/
	bool m_bUpDown;
	/** @brief �p���[�A�b�v�A�C�e�����擾���ꂽ���̃t���O*/
	bool m_bPlayer;
	/** @brief �w��̏ꏊ�ɕt���������Ă��Ȃ����𔻒肷��t���O
			   false�łĂ��Ȃ��@true�ł��Ă���*/
	bool m_bSetUp;
	/** @brief ��x�����t�����ɐݒ肷��*/
	bool m_bFirstBlad;
	/** @brief ��]�����邽�߂̕ϐ�*/
	float m_fRotTime;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CItem();
	/** @brief �f�X�g���N�^*/
	~CItem();
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;
	/** @brief �����蔻��̏���*/
	void OnCollisionEnter(Object* pObject)override;
	/** @brief �A�C�e���̎�ނ̐ݒ�*/
	void SetItem(ITEM_TYPE type);
	/** @brief �A�C�e���̎�ނ̎擾*/
	ITEM_TYPE GetItem();
	/** @brief �v���C���[�Ɏ擾���ꂽ����ݒ肷��*/
	void SetPlayer(bool player);

};

#endif