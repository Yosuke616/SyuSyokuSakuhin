/*
 Component.h
 �S�ẴR���|�[�l���g�̃x�[�X�N���X
 �u���z�S �^���������l����
 2022/6/7
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

//===== �C���N���[�h�K�[�h ======
#include "mainApp.h"
#include "ObjectInfo.h"

#include "Object.h"

//===== �񋓑̐錾 ======
enum COMPONENT_UPDATE_ORDER	//	�R���|�[�l���g�̍X�V����
{
	COM_UPDATE_1 = 0,		//	������������w��
	COM_UPDATE_2,
	COM_UPDATE_TRANSRATE,	//	�g�����X�t�H�[���R���|�[�l���g�p
	COM_UPDATE_COLLIDER,	//	�Փˏ���
	COM_UPDATE_OFF_SCREEN,	//	��ʊO����
	COM_UPDATE_DRAW,		//	�`��R���|�l���g�p
};

//===== �N���X��` =====
class Component
{
	//=== �����o�ϐ�
private:
	bool m_bDelete = false;										//	�폜�t���O
public:
	Object* Parent = nullptr;									//	�R���|�[�l���g�̏��L��
	COMPONENT_UPDATE_ORDER	m_eUpdateOrder = COM_UPDATE_1;		//	�R���|�[�l���g�̍X�V��

	//=== �����o�֐�
public:
	bool m_bDrawFlag = true;					//	�`��t���O
	bool m_bUpdateFlag = true;					//	�X�V�t���O
	void Delete() { m_bDelete = true; }			//	���̃I�u�W�F�N�g���폜����
	bool GetDeleteFlag() { return m_bDelete; }	//	�폜�t���O
	Component() {}								//	�R���X�g���N�^
	virtual ~Component() {}						//	(���z�֐�)�f�X�g���N�^
	virtual void Start() {}						//	(���z�֐�)������
	virtual void Update() {}					//	(���z�֐�)�X�V
	virtual void Draw() {}						//	(���z�֐�)�`��

/**
 * @fn		OnCollisionEnter
 * @brief	(���z�֐�)�I�u�W�F�N�g�Ƃ̏Փˏ���
 * @param	(Object*) ���̃R���|�[�l���g�̏��L�҂ƏՓ˂����I�u�W�F�N�g�̃|�C���^
 */
	virtual void OnCollisionEnter(Object*) {}
};
#endif // __COMPONENT_H__
