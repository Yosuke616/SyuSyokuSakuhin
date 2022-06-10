/*
	Object.h
	�x�[�X�ƂȂ�I�u�W�F�N�g�N���X
	�u��
	2022/6/7
 */

 //===== �C���N���[�h�K�[�h ======
#ifndef __OBJECT_H__
#define __OBJECT_H__

//===== �C���N���[�h ======
#include "mainApp.h"
#include "ObjectInfo.h"

#include <list>
#include <string>

//===== �O���錾 =====
class Component;



//===== �N���X��` ======
class Object
{
private:
	//=== �����o�ϐ�
	std::list<Component*>	ComponentList;	//	�@�\���X�g
	std::string		m_szName;				//	�I�u�W�F�N�g��
	UPDATE_ORDER	m_eUpdateOrder;			//	�X�V�̏���
	DRAW_ORDER		m_eDrawOrder;			//	�`��̏���
	bool			m_bDelete;				//	�f���[�g�t���O

public:
	//=== �����o�֐�
	Object();									//	�f�t�H���g�R���X�g���N�^(�g��Ȃ�)
	Object(std::string szName					//	�R���X�g���N�^
		, UPDATE_ORDER UpdateOrder
		, DRAW_ORDER DrawOrder);

	~Object();									//	�f�X�g���N�^
	void Start();								//	�J�n
	void Update();								//	�X�V
	void Draw();								//	�`��
	void OnCollisionEnter(Object*);				//	�Փˏ����i�ڐG�����^�C�~���O�Ŏ��s�j

	void StopUpdate();	// �@�\��~(�X�V����
	void ResumeUpdate();// �@�\�ĊJ(�X�V����

	// �@�\�ĊJ(�X�V�ƕ`��
	void Use();
	void NoUse();


	void StopDraw();		// �`���~(�X�V
	void ResumeDraw();		// �`��ĊJ(�X�V

	//getter
	DRAW_ORDER GetDrawOrder();				//	�`�揇�̎擾
	UPDATE_ORDER GetUpdateOrder();			//	�X�V���̎擾
	//setter
	void Delete();							//	�I�u�W�F�N�g�̍폜

/**
 * @fn		GetComponent()
 * @brief	�R���|�[�l���g���擾
 * @return	�e���v���[�g�Ŏw�肵���N���X�̃|�C���^	���s�̏ꍇnullptr
 * @detail	�I�u�W�F�N�g�����R���|�[�l���g(�@�\)���X�g����
 *			�w�肵���R���|�[�l���g�N���X�̃|�C���^���擾����֐�
 */
	template<class T>
	T* GetComponent()
	{
		for (auto&& com : ComponentList)
		{
			//	�e���v���[�g�Ŏw�肵���N���X�Ɍ^�L���X�g����
			T* buff = dynamic_cast<T*>(com);

			//	������
			if (buff != nullptr)
				return buff;
		}

		//	���s�����Ƃ�
		return nullptr;
	}

	/**
	 * @fn		AddComponent()
	 * @brief	�I�u�W�F�N�g�ɃR���|�[�l���g��ǉ�����
	 * @return	�e���v���[�g�Ŏw�肵���N���X�̃|�C���^
	 * @detail	�I�u�W�F�N�g�����R���|�[�l���g(�@�\)���X�g�Ɏw�肵���R���|�[�l���g��ǉ�����
	 *			���̎��A�R���|�[�l���g�̏��L�҃I�u�W�F�N�g�̃|�C���^���ݒ肳���
	 */
	template<class T>
	T* AddComponent()
	{
		T* component = new T();		//	�R���|�[�l���g�̐���
		component->Parent = this;	//	���̃R���|�[�l���g�̎�����̐ݒ�

		//	�X�V���������߂�
		std::list<Component*>::iterator itr = ComponentList.begin();
		for (auto&& com : ComponentList)
		{
			//	���݂̃I�u�W�F�N�g�̋@�\���擾
			Component* buff = (Component*)com;

			//	�X�V�̗D��x�������Ƃ��A���[�v���甲����
			if (component->m_eUpdateOrder <= buff->m_eUpdateOrder)	break;
			else	itr++;	//	�X�V������x�点��
		}

		//	�R���|�[�l���g���X�g�ɑ}��
		ComponentList.insert(itr, component);

		return component;
	}

	// ���O�̎擾
	constexpr const std::string& GetName() const
	{
		return m_szName;
	}

	// �폜�t���O�̎擾
	constexpr const bool GetDeleteFlag() const
	{
		return m_bDelete;
	}
};
#endif // __OBJECT_H__