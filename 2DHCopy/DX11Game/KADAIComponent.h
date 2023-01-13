/**
* @file		KADAIComponent.h
* @brief	GR���I��点�邽�߂����̃R���|�[�l���g
* @author	�u���z�S
* @date		2023/1/3
*/

/**�C���N���[�h�K�[�h**/
#ifndef _KADAI_COMPONENT_H_
#define _KADAI_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class  CTransform;
class  Object;
class  CCollider;
class  CAnimMesh;

/**�N���X��`**/
class CKADAI : public Component {
private:
	//�����o�ϐ�
	/** @brief ���W�Ƃ��̏��*/
	CTransform* m_pTrans;
	/** @brief �`����*/
	CAnimMesh* m_pDraw;
	/** @brief �����蔻����*/
	CCollider* m_pColl;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	
	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	CKADAI();
	/** @brief �f�X�g���N�^*/
	~CKADAI();
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;
	/** @brief �Փ˔���*/
	void OnCollisionEnter(Object* pObject) override;

};

#endif