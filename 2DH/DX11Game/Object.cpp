//===== �C���N���[�h =====
#include "Object.h"
#include "Component.h"


// �f�t�H���g�R���X�g���N�^�i�g��Ȃ��j
Object::Object()
	: m_bDelete(false),
	m_eDrawOrder(MAX_DRAW_ORDER), m_eUpdateOrder(MAX_UPDATE_ORDER)
{
}

/**
* @fn		Object::Object(std::string szName, UPDATE_ORDER UpdateOrder, DRAW_ORDER DrawOrder)
* @brief	�I�u�W�F�N�g�������Ɏg�p����R���X�g���N�^
* @param	(string)		�I�u�W�F�N�g��
* @param	(UPDATE_ORDER)	�I�u�W�F�N�g�̍X�V�̏���
* @param	(DRAW_ORDER)	�I�u�W�F�N�g�̕`��̏���
* @detail	�X�V�A�`��̏��Ԃ�ObjectInfo.h
*/
Object::Object(std::string szName, UPDATE_ORDER UpdateOrder, DRAW_ORDER DrawOrder)
	: m_bDelete(false)
{
	m_szName = szName;		//	���O�o�^
	m_eUpdateOrder = UpdateOrder;	//	�X�V���X�g�̏��ʂ��Z�b�g
	m_eDrawOrder = DrawOrder;	//	�`�惊�X�g�̏��ʂ��Z�b�g
}

/**
* @fn		Object::~Object
* @brief	�I�u�W�F�N�g�̍폜
* @detail	�I�u�W�F�N�g�����R���|�[�l���g�̍폜
*/
Object::~Object()
{
	//	�@�\���X�g�̍폜
	for (auto&& component : ComponentList)
	{
		delete component;
		component = nullptr;
	}

	//	�@�\���X�g�폜
	ComponentList.clear();
}

/**
* @fn		Object::Start
* @brief	�I�u�W�F�N�g�̏�����
* @detail	�I�u�W�F�N�g�����R���|�[�l���g�̏�����
*/
void Object::Start()
{
	//	�e�R���|�[�l���g�̏�����
	for (auto&& component : ComponentList)
		component->Start();
}

/**
* @fn		Object::Update
* @brief	�I�u�W�F�N�g�̍X�V
* @detail	�I�u�W�F�N�g�����R���|�[�l���g�̍X�V
*/
void Object::Update()
{
	//	�e�R���|�[�l���g�̍X�V
	for (auto&& component : ComponentList)
	{
		// �X�V���邩(�X�V�����~�߂����Ƃ������false�ɂ���)
		if (component->m_bUpdateFlag == false) continue;

		// �R���|�[�l���g�̍X�V	
		component->Update();
	}


	// �폜�t���O�̊m�F
	ComponentList.remove_if([](Component* pObj)
	{
		if (pObj->GetDeleteFlag())
		{
			delete pObj;
			return true;
		}
		else return false;
	});
}

/**
* @fn		Object::Draw
* @brief	�I�u�W�F�N�g�̕`��
* @detail	�I�u�W�F�N�g�����R���|�[�l���g�̕`��
*/
void Object::Draw()
{
	//	�e�R���|�[�l���g�̕`��
	for (auto&& component : ComponentList)
	{
		// �`��t���O
		if (component->m_bDrawFlag == false) continue;

		// �R���|�[�l���g�̕`��
		component->Draw();
	}
}

/**
* @fn		Object::OnCollisionEnter
* @brief	�I�u�W�F�N�g�Ƃ̏Փˏ���
* @param	(Object*)	�Փ˂�������̃|�C���^
* @detail	�I�u�W�F�N�g�����R���|�[�l���g�̏Փˏ���
*/
void Object::OnCollisionEnter(Object* pObject)
{
	//	�e�R���|�[�l���g�̏Փˎ��̏���
	for (auto&& component : ComponentList)
	{
		// �X�V���邩(�X�V�����~�߂����Ƃ������false�ɂ���)
		if (component->m_bUpdateFlag == false) continue;

		// �Փˏ���
		component->OnCollisionEnter(pObject);
	}
}

/**
* @fn		Object::Delete
* @brief	�I�u�W�F�N�g�̍폜������֐�
*/
void Object::Delete()
{
	m_bDelete = true;
}

// �`�揇�̎擾
DRAW_ORDER Object::GetDrawOrder()
{
	return m_eDrawOrder;
}

// �X�V���̎擾
UPDATE_ORDER Object::GetUpdateOrder()
{
	return m_eUpdateOrder;
}

// �@�\��~
void Object::StopUpdate()
{
	for (auto&& component : ComponentList)
	{
		// �X�V�t���OOFF
		component->m_bUpdateFlag = false;
	}
}

// �@�\�̍ĊJ
void Object::ResumeUpdate()
{
	for (auto&& component : ComponentList)
	{
		// �X�V�t���OON
		component->m_bUpdateFlag = true;
	}
}

// �`���~
void Object::StopDraw()
{
	for (auto&& component : ComponentList)
	{
		// �`��t���OON
		component->m_bDrawFlag = false;
	}
}
// �`��ĊJ
void Object::ResumeDraw()
{
	for (auto&& component : ComponentList)
	{
		// �`��t���OON
		component->m_bDrawFlag = true;
	}
}

// �@�\�ĊJ(�X�V�ƕ`��
void Object::Use()
{
	for (auto&& component : ComponentList)
	{
		// �X�V�ĊJ
		component->m_bUpdateFlag = true;

		// �`��ĊJ
		component->m_bDrawFlag = true;
	}
}

// �@�\�̍ĊJ(�X�V�ƕ`��
void Object::NoUse()
{
	for (auto&& component : ComponentList)
	{
		// �X�V��~
		component->m_bDrawFlag = false;

		// �`���~
		component->m_bDrawFlag = false;
	}
}