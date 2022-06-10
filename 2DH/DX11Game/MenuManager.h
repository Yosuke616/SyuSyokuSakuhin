/**�|�[�Y
 MenuManager.h
 ���j���[���Ǘ�����N���X�ɂ���
 YOSUKE SHIMIZU
 2022/6/3
 */

#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

 //�C���N���[�h��
#include "mainApp.h"
#include <list>

//�O���錾
class Object;

//�񋓑��錾
enum MENU_STATE
{
	STATE_NONE = 0,
	STATE_TITLE,
	STATE_OPTION,
	STATE_SELECT,
	STATE_GAMEOVER,

	STATE_MAX
};

//�N���X��`
class MenuManager
{
	//=== �����o�ϐ�
private:
	static MenuManager* m_pInstance;	// �C���X�^���X�p
	std::list<Object*>	m_MenuList;		// ���j���[���X�g
	int m_nDelayCnt;					// �x������
	int m_nCreateState;					// �쐬���郁�j���[

public:
	// �I�v�V�����t���O
	static bool m_bOption;

	//=== �����o�֐�
private:
	MenuManager();						// �R���X�g���N�^
public:
	~MenuManager();						// �f�X�g���N�^
	void Start();						// ������
	void Update();						// �X�V
	void Draw();						// �`��
	static MenuManager* GetInstance();	// �C���X�^���X����
	static void Destroy();				// �C���X�^���X�j��

	// ���j���[�쐬
	void Create(int state, int delayCnt = 1);

	// �^�C�g��
	void CreateTitleMenu();

	// ���j���[���X�g�̍폜
	void DeleteMenu();

	// ���j���[�I�u�W�F�N�g�̒ǉ�
	void AddMenu(Object* pMenuObject);

	// ���j���[���X�g�̎擾
	std::list<Object*>& GetMenuList();
};
#endif	// _MENUMANAGER_H_
