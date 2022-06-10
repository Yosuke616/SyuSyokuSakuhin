/*
	ObjectManager.h
	���ׂẴI�u�W�F�N�g���Ǘ�����N���X
	�u���z�S
	2022/6/7
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __COBJECT_MANAGER_H__
#define __COBJECT_MANAGER_H__

//===== �C���N���[�h ======
#include <list>
#include <string>


//===== �S���錾 =====
class Object;


//===== �N���X��` ======
class ObjectManager
{
	//=== �����o�ϐ�
private:
	std::list<Object*>		m_UpdateList;	//	�X�V���X�g
	std::list<Object*>		m_DrawList;		//	�`�惊�X�g
	static ObjectManager*	m_pInstance;	//	�C���X�^���X


	//=== �����o�֐�
protected:
	ObjectManager();						//	�R���X�g���N�^
public:
	static ObjectManager* GetInstance();	//	�C���X�^���X�̎擾
	~ObjectManager();						//	�f�X�g���N�^
	void Start();							//	�J�n
	void Update();							//	mode_game  �Q�[���X�V
	void UpdateEdit();						//	mode_debug �X�e�[�W�쐬���̍X�V
	void Draw();							//	�`��
	void Uninit();							//	�I��

	//	�I�u�W�F�N�g�ǉ�
	void AddObject(Object* pObject);

	// ���O�ŃI�u�W�F�N�g���폜
	void DeleteObjectByName(std::string pszName, std::list<Object*>& list);

	// ���O����v�����I�u�W�F�N�g�����X�g�ɒǉ�
	std::list<Object*> GetObjectList(std::string name);

	// �@�\��~(�X�V����
	void NoFunction();
	// �@�\�ĊJ(�X�V����
	void Resume();



	//getter
	Object* GetGameObject(std::string name);//	���O����I�u�W�F�N�g����肷��
	std::list<Object*>& GetUpdateList();	//	�X�V���X�g�̎擾
	std::list<Object*>& GetDrawList();		//	�`�惊�X�g�̎擾

	// ���O����v���Ă���I�u�W�F�N�g�����X�g�ɒǉ�����
	void GetGameObjects(std::string name, std::list<Object*>& resultList);
};
#endif // __OBJECT_MANAGER_H__