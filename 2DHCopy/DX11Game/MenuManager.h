/**
* @file		MenuManager.h
* @brief	���j���[�𑀍삵���肷��}�l�[�W���[
* @author	�u���z�S
* @date		2022/7/31
*/

/**�C���N���[�h�K�[�h**/
#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

/**�C���N���[�h��**/
#include "mainApp.h"
#include <list>

/**�O���錾**/
class Object;

/**�񋓑̐錾**/
enum MENU_STATE {
	STATE_NONE = 0,
	TITLE_STATE,

	MAX_MENU
};

/**�N���X��`**/
/**
* @brief	���j���[�̏�Ԃ��Ǘ�����N���X
* @detail	�V�[�����Ƃɗp�ӂ��郁�j���[��ς����肷��
*/
class MenuManager {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X*/
	static MenuManager* m_pInstance;
	/** @brief ���j���[���X�g*/
	std::list<Object*> m_MenuList;
	/** @brief ���j���[���X�g�̒��g�������邽�߂̃C�e���[�^�[*/
	std::list<Object*>::iterator m_itr_Menu;
	/** @brief �x������*/
	int m_nDelayCnt;
	/** @brief �쐬���郁�j���[*/
	int m_nCreateMenu;

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	MenuManager();
	/** @brief �I�΂�Ă���{�^���̐F��ς�����A
			   �{�^���������ꂽ��V�[���J�ڂ����肷��֐�*/
	void SelectButton();
	/** @brief ����{�^���������ꂽ�ꍇ�̏���*/
	void PushButton();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�
	/** @brief �I�v�V�����t���O*/
	static bool m_bOption;

	//�����o�֐�
	/** @brief �f�X�g���N�^*/
	~MenuManager();
	/** @brief ������*/
	void Start();
	/** @brief �X�V����*/
	void Update();
	/** @brief �`�揈��*/
	void Draw();
	/** @brief �C���X�^���X����*/
	static MenuManager* GetInsutance();
	/** @brief �C���X�^���X�j��*/
	static void Destroy();
	/** @brief ���j���[�쐬*/
	void Create(int state,int delayCnt = 1);
	/** @brief ���j���[���X�g�̍폜*/
	void DeleteMenu();
	/** @brief ���j���[�I�u�W�F�N�g�̒ǉ�*/
	void AddMenu(Object* pMenuObject);
	/** @brief ���j���[���X�g�̎擾*/
	std::list<Object*>& GetMenuList();
	/** @brief ��郁�j���[�̔ԍ����擾����*/
	int GetCreateMenu();

	/**��肽�����j���[���������牺�ɒǉ����Ă���**/
	/** @brief �^�C�g�����j���[*/
	void CreateTitleMenu();
	/** @brief �|�[�Y���j���[*/
	void CreatePauseMenu();
};

#endif