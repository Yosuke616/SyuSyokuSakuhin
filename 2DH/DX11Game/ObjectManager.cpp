//===== �C���N���[�h =====
#include "ObjectManager.h"
#include "Component.h"
#include "Object.h"

//===== �ÓI�����o�ϐ� =====
ObjectManager* ObjectManager::m_pInstance = nullptr;	//	�C���X�^���X

 /**
 * @fn		ObjectManager::ObjectManager
 * @brief	�R���X�g���N�^
 */
ObjectManager::ObjectManager()
{
}

/**
* @fn		ObjectManager::~ObjectManager
* @brief	�f�X�g���N�^
*/
ObjectManager::~ObjectManager()
{
}

/**
 * @fn		ObjectManager::GetInstance
 * @brief	�I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X��
 * @return	�V���O���g���Ő������ꂽObjectManager�N���X�̃|�C���^
*/
ObjectManager* ObjectManager::GetInstance()
{
	if (!m_pInstance)
	{
		//	�C���X�^���X������Ă��Ȃ��ꍇ�͍��
		m_pInstance = new ObjectManager();
	}
	return m_pInstance;
}

/**
* @fn		Object::Start
* @brief	�S�ẴI�u�W�F�N�g�̏�����
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̏�����
*/
void ObjectManager::Start()
{
	//	�S�I�u�W�F�N�g�̏�����
	for (auto&& object : m_UpdateList)
		object->Start();
}

/**
* @fn		Object::Uninit
* @brief	�S�ẴI�u�W�F�N�g�̍폜
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̍폜
*			���̃N���X���������
*/
void ObjectManager::Uninit()
{
	//	�I�u�W�F�N�g���X�g�̍폜
	for (auto&& Object : m_UpdateList)
		delete Object;


	//	���X�g����ɂ���
	m_UpdateList.clear();
	m_DrawList.clear();	//	�X�V���X�g�ƕ`�惊�X�g�͓����I�u�W�F�N�g�������Ă��邽�ߕЕ��Ńf���[�g���邾���ł���

	//	���̃I�u�W�F�N�g�̍폜
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		Object::Update
* @brief	�S�ẴI�u�W�F�N�g�̍X�V
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̍X�V
*			�f���[�g�t���O�������Ă���I�u�W�F�N�g�͍X�V�̍Ō�ō폜���Ă���
*/
void ObjectManager::Update()
{
	// �I�u�W�F�N�g�̍X�V
	for (auto&& object : m_UpdateList)
		object->Update();


	// s�I�u�W�F�N�g�폜�̊m�F
	m_UpdateList.remove_if([](Object* pObj)
	{
		return pObj->GetDeleteFlag();
	});
	m_DrawList.remove_if([](Object* pObj)
	{
		if (pObj->GetDeleteFlag())
		{
			delete pObj;
			return true;
		}
		else return false;
	});
}

//	mode_debug �폜�t���O�̊m�F�̂ݍs��
void ObjectManager::UpdateEdit()
{
	//	�I�u�W�F�N�g�폜�̊m�F
	m_UpdateList.remove_if([](Object* pObj)
	{
		return pObj->GetDeleteFlag();
	});
	m_DrawList.remove_if([](Object* pObj)
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
* @brief	�S�ẴI�u�W�F�N�g�̕`��
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̕`��
*/
void ObjectManager::Draw()
{
	SetZBuffer(true);
	for (auto&& object : m_DrawList)
		object->Draw();

}


/**
* @fn		ObjectManager::AddObject
* @brief	�I�u�W�F�N�g��ǉ�
* @param	(Object*) �ǉ�����I�u�W�F�N�g
 * @detail	�o�^����Ă���D�揇�ʂŒǉ����Ă���
*			ObjectInfo.h�ŗD�揇�ʂ�o�^���Ă��܂�
*/
void ObjectManager::AddObject(Object* pObject)
{
	// �I�u�W�F�N�g������
	pObject->Start();

	//	�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itrUpdate = m_UpdateList.begin();
	for (auto&& UpdateObject : m_UpdateList)
	{
		if (pObject->GetUpdateOrder() > UpdateObject->GetUpdateOrder())
		{
			itrUpdate++;
			continue;
		}
		else break;
	}
	m_UpdateList.insert(itrUpdate, pObject);

	//	�`�惊�X�g�ɒǉ�
	std::list<Object*>::iterator itrDraw = m_DrawList.begin();
	for (auto&& DrawObject : m_DrawList)
	{
		if (pObject->GetDrawOrder() > DrawObject->GetDrawOrder())
		{
			itrDraw++;
			continue;
		}
		else break;
	}
	m_DrawList.insert(itrDraw, pObject);
}


//	+++�I�u�W�F�N�g���X�g�̎擾
std::list<Object*>& ObjectManager::GetUpdateList()
{
	return m_UpdateList;
}

//	+++�X�V���X�g�̎擾
std::list<Object*>& ObjectManager::GetDrawList()
{
	return m_DrawList;
}

/**
* @fn		ObjectManager::DeleteObjectByName
* @brief	�w�肵�����X�g���疼�O�Ɉ�v�����I�u�W�F�N�g���폜����
* @param	(string)		�폜����I�u�W�F�N�g��
* @param	(list<Object*>) ���X�g�̎w��
*/
void ObjectManager::DeleteObjectByName(std::string pszName, std::list<Object*>& list)
{
	for (auto&& obj : list)
	{
		//	���O����v�����I�u�W�F�N�g���폜
		if (obj->GetName() == pszName)
		{
			obj->Delete();
		}
	}
}

/**
* @fn		ObjectManager::etGameObject
* @brief	���O�ŃI�u�W�F�N�g������(�v���C���[���������g�����Ȃ�����)
* @param	(string)		�폜����I�u�W�F�N�g��
* @return	�������ꍇ ���̃I�u�W�F�N�g�|�C���^	�Ȃ������ꍇ nullptr
*/
Object* ObjectManager::GetGameObject(std::string name)
{
	for (auto&& obj : m_UpdateList)
	{
		//	���O����v�����I�u�W�F�N�g���폜
		if (obj->GetName() == name)
		{
			return obj;
		}
	}
	return nullptr;
}


// ���O����v�����I�u�W�F�N�g�����X�g�ɒǉ�
std::list<Object*> ObjectManager::GetObjectList(std::string name)
{
	std::list<Object*> list;
	for (auto&& obj : m_UpdateList)
	{
		if (obj->GetName() == name)
		{
			list.push_back(obj);
		}
	}

	return list;
}

void ObjectManager::NoFunction()
{
	for (auto object : m_UpdateList)
	{
		object->StopUpdate();
	}
}

// �@�\�ĊJ
void ObjectManager::Resume()
{
	for (auto object : m_UpdateList)
	{
		object->ResumeUpdate();
	}
}





void ObjectManager::GetGameObjects(std::string name, std::list<Object*>& resultList)
{
	for (auto && object : m_UpdateList)
	{
		if (object->GetName() == name)
		{
			resultList.push_back(object);
		}
	}
}