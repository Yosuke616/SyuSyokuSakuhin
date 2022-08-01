/**�C���N���[�h��**/
#include "MenuManager.h"
#include "Object.h"
#include <crtdbg.h>
#include "TransformComponent.h"
#include "InputManager.h"
#include "Draw2DComponent.h"
#include "ColliderComponent.h"
#include "TextureManager.h"
#include "sceneGame.h"

/**�ÓI�����o�ϐ�**/
MenuManager* MenuManager::m_pInstance = nullptr;
bool MenuManager::m_bOption = false;

/**
* @fn		MenuManager::MenuManager
* @brief	�L�m�R���̒j�@�ݽ�׸��
*/
MenuManager::MenuManager() 
	:m_nDelayCnt(-1),m_nCreateMenu(STATE_NONE){
	m_bOption = false;
}

/**
* @fn		MenuManager::~MenuManager
* @brief	�S�Ẵ��j���[�I�u�W�F�N�g�̉��
*/
MenuManager::~MenuManager() {
	//�I�u�W�F�N�g���X�g�̉��
	for (auto&& menuObject :m_MenuList) {
		delete menuObject;
	}
	m_MenuList.clear();
}

/**
* @fn		MenuManager::GetInstance
* @brief	�C���X�^���X��
*/
MenuManager* MenuManager::GetInsutance() {
	if (!m_pInstance) {
		m_pInstance = new MenuManager;
	}
	return m_pInstance;
}

/**
* @fn		MenuManager::Destroy
* @brief	�C���X�^���X�̔j��
*/
void MenuManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		MenuManager::Start
* @brief	������
*/
void MenuManager::Start() {
	//���j���[�̏�����
	for (auto&& MenuObject:m_MenuList) {
		MenuObject->Start();
	}
}

/**
* @fn		MenuManager::Update
* @brief	���j���[�̃��X�g�̓����Ă���I�u�W�F�N�g�̍X�V
*/
void MenuManager::Update() {
	if (m_nDelayCnt > 0) {
		//���j���[�쐬��x�点��
		m_nDelayCnt--;

		if (m_nDelayCnt == 0) {
			switch (m_nCreateMenu)
			{
			case TITLE_STATE: CreateTitleMenu(); break;
			default:break;
			}
	
			m_nDelayCnt = -1;
		}
	}

	//���j���[�I�u�W�F�N�g�̍X�V
	for (auto&& object : m_MenuList) {
		object->Update();
	}

	//Collider�̍X�V
	CCollider::GetColliderList().remove_if([](Object* pObj){
		return pObj->GetDeleteFlag();
	});

	//�I�u�W�F�N�g�폜�̊m�F
	m_MenuList.remove_if([](Object* pObj) {
		return pObj->GetDeleteFlag();
	});
}

/**
* @fn		MenuManager::Draw
* @brief	���j���[�I�u�W�F�N�g�̕`��
*/
void MenuManager::Draw() {
	SetZBuffer(true);
	//���j���[�I�u�W�F�N�g�̕`��
	for (auto&& menuObject:m_MenuList) {
		menuObject->Draw();
	}
}

/**
* @fn		MenuManager::Create
* @brief	���j���[�̍쐬
* @param	(int)	�ǂ̃��j���[���쐬���邩�̔ԍ�
* @param	(int)	�ǂ̈ʂ̃t���[���̌�쐬���邩
*/
void MenuManager::Create(int state,int delayCnt) {
	m_nCreateMenu = state;
	m_nDelayCnt = delayCnt;
}

/**
* @fn		MenuManager::CreateTitleMenu
* @brief	�^�C�g�����j���[�̍쐬
*/
void MenuManager::CreateTitleMenu() {
	//�I�u�W�F�N�g���X�g�̍폜
	DeleteMenu();

	//�K�v�ɂȂ��Ă���{�^����ǉ�����
	//��
	Object* pButton = new Object("Button_Kari",UPDATE_UI,DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton = pButton->AddComponent<CTransform>();
	auto drawButton = pButton->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton->SetPosition(0.0f,0.0f);
	drawButton->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton->SetSize(100.0f,50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton);

}

/**
* @fn		MenuManager::CeratePauseMenu
* @brief	�|�[�Y���j���[�̍쐬�ƕ\��
*/
void MenuManager::CreatePauseMenu() {
	//�I�u�W�F�N�g�̃��X�g�̍폜
	DeleteMenu();
}

/**
* @fn		MenuManager::AddMenu
* @brief	���j���[���X�g�ɒǉ�
* @param	(Object*)	�ǉ�����I�u�W�F�N�g�̃|�C���^
*/
void MenuManager::AddMenu(Object* pMenuObject) {
	//�X�V���X�g�ɒǉ�
	std::list<Object*>::iterator itr = m_MenuList.begin();
	for (auto&& UpdateObject : m_MenuList) {
		if (pMenuObject->GetDrawOrder() > UpdateObject->GetDrawOrder()) {
			itr++;
			continue;
		}
		else {
			break;
		}
	}
	m_MenuList.insert(itr,pMenuObject);

	pMenuObject->Start();
}

/**
* @fn		MenuManager::GetMenuList
* @brief	���j���[���X�g�̎擾
*/
std::list<Object*>& MenuManager::GetMenuList() {
	return m_MenuList;
}

/**
* @fn		MenuManager::DeleteMenu
* @brief	���j���[���X�g���J���ɂ���
*/
void MenuManager::DeleteMenu() {
	//�I�u�W�F�N�g���X�g�̍폜
	for (auto&& menuObject : m_MenuList) {
		menuObject->Delete();
	}
	m_MenuList.clear();
}