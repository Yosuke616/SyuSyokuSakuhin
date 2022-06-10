#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>

//2022/1/28/Tutorial_CSV Shimizu Yosuke ---------------------
#include "TitleScene.h"
//-----------------------------------------------------------

//===== �ÓI�����o�ϐ� =====
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

// �C���X�^���X��
MenuManager* MenuManager::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new MenuManager;
	}

	return m_pInstance;
}

// �C���X�^���X�̔j��
void MenuManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::MenuManager
* @brief	�R���X�g���N�^
*/
MenuManager::MenuManager()
	: m_nDelayCnt(-1), m_nCreateState(STATE_NONE)
{
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	�S�Ẵ��j���[�I�u�W�F�N�g�̊J��
*/
MenuManager::~MenuManager()
{
	//	�I�u�W�F�N�g���X�g�̍폜
	for (auto&& menuObject : m_MenuList)
		delete menuObject;
	m_MenuList.clear();
}

/**
* @fn		MenuManager::~Start
* @brief	������
*/
void MenuManager::Start()
{
	//	���j���[�̏�����
	for (auto&& MenuObject : m_MenuList)
		MenuObject->Start();
}

/**
* @fn		MenuManager::Update
* @brief	�S�ẴI�u�W�F�N�g�̍X�V
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̍X�V
*/
void MenuManager::Update()
{
	if (m_nDelayCnt > 0)
	{
		// ���j���[�쐬��x�点��
		m_nDelayCnt--;

		if (m_nDelayCnt == 0)
		{
			switch (m_nCreateState)
			{
			case STATE_TITLE:	 CreateTitleMenu(); break;
			case STATE_OPTION:	  break;
			case STATE_SELECT:	  break;
			case STATE_GAMEOVER:  break;
			default:
				break;
			}

			m_nDelayCnt = -1;
		}
	}

	//	���j���[�I�u�W�F�N�g�̍X�V
	for (auto&& object : m_MenuList)
	{
		object->Update();
	}

	//	�I�u�W�F�N�g�폜�̊m�F
	m_MenuList.remove_if([](Object* pObj)
	{
		return pObj->GetDeleteFlag();
	});
}

/**
* @fn		MenuManager::DrawMenu
* @brief	���j���[�I�u�W�F�N�g�̕`��
* @detail	�e�X�̃I�u�W�F�N�g�����R���|�[�l���g�̕`��
*/
void MenuManager::Draw()
{
	SetZBuffer(true);
	//	���j���[�I�u�W�F�N�g�`��
	for (auto&& menuObject : m_MenuList)
		menuObject->Draw();
}

//  �w��t���[����Ƀ��j���[�쐬
void MenuManager::Create(int state, int delayCnt)
{
	m_nCreateState = state;

	m_nDelayCnt = delayCnt;
}

// �^�C�g�����j���[�\��
void MenuManager::CreateTitleMenu()
{
	// �I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();
}

/**
* @fn		MenuManager::AddMenu
* @brief	���j���[���X�g�ɒǉ�
* @param	(Object*) �ǉ�����I�u�W�F�N�g
*/
void MenuManager::AddMenu(Object* pMenuObject)
{
	//	�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList)
	{
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder())
		{
			itr++;
			continue;
		}
		else break;
	}
	//	���j���[���X�g�ɒǉ�
	m_MenuList.insert(itr, pMenuObject);


	// �I�u�W�F�N�g������
	pMenuObject->Start();
}

// ���j���[���X�g�̎擾
std::list<Object*>& MenuManager::GetMenuList()
{
	return m_MenuList;
}

// ���j���[���X�g����ɂ���
void MenuManager::DeleteMenu()
{
	//	�I�u�W�F�N�g���X�g�̍폜
	for (auto&& menuObject : m_MenuList)
		menuObject->Delete();

	m_MenuList.clear();
}



