/**
* @file		CircleComponent.h
* @brief	�o���A��\�邽�߂̃R���|�[�l���g
* @author	�u���z�S
* @date		2022/10/10
*/

/**�C���N���[�h�K�[�h**/
#ifndef _CIRCLE_COMPONENT_H_
#define _CIRCLE_COMPONENT_H_

/**�C���N���[�h��**/
#include "Component.h"

/**�O���錾**/
class CTransform;
class Object;
class CAnimMesh;

/**�N���X��`**/
/**
* @brief	�T�[�N���𐧌䂷�邽�߂̃R���|�[�l���g
*/
class CCircle : public Component{
private:
	//�����o�ϐ�
	/** @brief ���W�Ƃ��𐧌䂷��|�C���^*/
	CTransform* m_pTransform;
	/** @brief �`����𐧌䂷��|�C���^*/
	CAnimMesh* m_pDraw;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	/** @brief �R���X�g���N�^*/
	CCircle();
	/** @brief �f�X�g���N�^*/
	~CCircle();
	/** @brief ������*/
	void Start() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;
	/** @brief �����蔻��̏���*/
	void OnCollisionEnter(Object* pObject)override;

	//�����o�֐�

};

#endif