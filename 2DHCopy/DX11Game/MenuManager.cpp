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
#include "imgui.h"
#include "SceneManager.h"

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
	//���j���[���X�g���̃I�u�W�F�N�g�̏��������I�������C�e���[�^��擪�Ɏ����Ă���
	m_itr_Menu = m_MenuList.begin();
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

	//�����ɑI������Ă���{�^���Ȃǂ�I�Ԋ֐���ǉ�����
	//�I�΂�Ă���{�^���̐F��ς���Ȃ�
	SelectButton();
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

#ifdef _DEBUG /**imgui�Ńf�o�b�O�̂���o��**/
	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("Button_State");
	Text("B_STATE : %d", m_MenuList.size());
	End();

#endif
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

	//**********************************************************************************************
	//��
	Object* pButton2 = new Object("Button_Kari2", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton2 = pButton2->AddComponent<CTransform>();
	auto drawButton2 = pButton2->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton2->SetPosition(0.0f, -50.0f);
	drawButton2->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton2->SetSize(100.0f, 50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton2);
	//***************************************************************************************************

	//**********************************************************************************************
	//��
	Object* pButton3 = new Object("Button_Kari3", UPDATE_UI, DRAW_UI);
	//�R���|�[�l���g�̒ǉ�
	auto transButton3 = pButton3->AddComponent<CTransform>();
	auto drawButton3 = pButton3->AddComponent<CDraw2D>();

	//�I�u�W�F�N�g�̐ݒ�
	transButton3->SetPosition(0.0f, -100.0f);
	drawButton3->SetTexture(TextureManager::GetInstance()->GetTexture(TITLE_BUTTON_NUM));
	drawButton3->SetSize(100.0f, 50.0f);

	//���j���[���X�g�ɒǉ�
	AddMenu(pButton3);
	//***************************************************************************************************

	//�ǉ����ꂽ�I�u�W�F�N�g�̏��������s��
	Start();
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
	//m_MenuList.insert(itr,pMenuObject);
	m_MenuList.push_back(pMenuObject);

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

/**
* @fn		MenuManager::GetCreateMenu
* @brief	����郁�j���[�̔ԍ���Ԃ�
* @return	(int)	�ǂ̔ԍ��̃��j���[������邩
*/
int MenuManager::GetCreateMenu() {
	return m_nCreateMenu;
}

/**
* @fn		MenuManager::SelectButton
* @brief	�I�΂ꂽ�{�^���̐F�ς���A�����ꂽ���̏���
*/
void MenuManager::SelectButton() {
	//�F��S�ăf�t�H���g�ɖ߂��Ă���
	for (auto&& menuObject : m_MenuList) {
		menuObject->GetComponent<CDraw2D>()->SetColor(1.0f,1.0f,1.0f);
	}

	//�L�[���͂Ń{�^����I�ׂ�悤�ɂ���
	if (InputManager::Instance()->GetKeyTrigger(DIK_W)) {
		//�����
		//�C�e���[�^�[���X�^�[�g�n�_�������ꍇ�I���ɍs������
		if (m_itr_Menu == m_MenuList.begin()) {
			m_itr_Menu = (m_MenuList.end());
			m_itr_Menu--;
		}
		else {
			//�����łȂ��ꍇ��1������
			m_itr_Menu--;
		}
	}
	else if(InputManager::Instance()->GetKeyTrigger(DIK_S)){
		//������
		//�C�e���[�^�[���I�[�ɂ����ꍇ�X�^�[�g�n�_�ɍs������
		std::list<Object*>::iterator itr = m_MenuList.end();
		itr--;
		if (m_itr_Menu == itr) {
			m_itr_Menu = m_MenuList.begin();
		}
		else {
			m_itr_Menu++;
		}
	}


	//�I�΂ꂽ�{�^���̐F��ς���
	switch (m_nCreateMenu) {
	case TITLE_STATE:
		if ((*m_itr_Menu)->GetName() == "Button_Kari") {
			(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(0.0f,1.0f,0.0f);
		}else if ((*m_itr_Menu)->GetName() == "Button_Kari2") {
			(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(0.0f, 1.0f, 0.0f);
		}
		else  if ((*m_itr_Menu)->GetName() == "Button_Kari3") {
			(*m_itr_Menu)->GetComponent<CDraw2D>()->SetColor(0.0f, 1.0f, 0.0f);
		}
		
		break;
	default:break;
	}

	//����{�^���������ꂽ���̏���
	if (InputManager::Instance()->GetKeyTrigger(DIK_RETURN)) {
		PushButton();
	}
}

/**
* @fn		MenuManager::PushButton
* @brief	����{�^���������ꂽ���̏���
* @detail	�V�[�����Ƃɏ�����ς���̂ŃX�C�b�`�����Ƃ�ł��Ȃ��Ȃ�\�z
*/
void MenuManager::PushButton() {
	//���ꂽ�V�[�����Ƃŏ�����ς���
	switch (m_nCreateMenu) {
	case TITLE_STATE:
		if ((*m_itr_Menu)->GetName() == "Button_Kari") {
			//�V�[���J��
			SceneManager::Instance()->SetScene(SCENE_GAME);
		}
		
		break;

	default:break;
	}
}