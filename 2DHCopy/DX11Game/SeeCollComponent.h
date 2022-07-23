/**
* @file		SeeCollComponent.h
* @brief	�����蔻���������悤�ɂ���
* @detail   �e�̍��W�Ɠ����蔻��̑傫�����擾���������� 
* @author	�u���z�S
* @date		2022/7/21
*/

//�C���N���[�h�K�[�h
#ifndef _SEECOLL_H_
#define _SEECOLL_H_

//�C���N���[�h��
#include "mainApp.h"
#include "Component.h"

//�O���錾
class CTransform;
class CCollider;
class ObjectManager;
class Object;

//�N���X��`
class CSeeColl :public Component {
private:
	/** @brief ���W*/
	CTransform* m_pTransform;
	/** @brief �����蔻��̏��*/
	CCollider* m_pColl;
	/** @brief object���̂̏���ۑ�����*/
	Object* m_pObject;
protected:

public:
	/** @brief �R���X�g���N�^*/
	CSeeColl();
	/** @brief �f�X�g���N�^*/
	~CSeeColl();
	/** @brief ������*/
	void Start() override;
	/** brief �X�V����*/
	void Update() override;
	/** brief �`�揈��*/
	void Draw() override;
};

#endif