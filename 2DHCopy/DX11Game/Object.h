/*
	�I�u�W�F�N�g�̃x�[�X�N���X
	�u���z�S
	2022/6/9 �쐬�J�n
*/

#ifndef _OBJECT_H_
#define _OBJECT_H_

//�C���N���[�h��
#include "mainApp.h"
#include "ObjInfo.h"

#include <list>
#include <string>

//�O���錾
class Component;

//�N���X��`
class Object {
private:
	//�����o�ϐ�
	std::list<Component*> ComponentList;	//�@�\���X�g
	std::string		m_szName;				//�I�u�W�F�N�g�̖��O
	UPDATE_ORDER	m_eUpdateOrder;			//�X�V�̏���
	DRAW_ORDER      m_eDrawOrder;			//�`��̏���
	bool			m_bDelete;				//�폜�t���O

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	Object();								//�f�t�H���g�R���X�g���N�^
	Object(std::string szName,				//�C�j�V�����C�Y�t���R���X�g���N�^
		UPDATE_ORDER UpdateOrder,
		DRAW_ORDER DrawOrder);

	~Object();								//�f�X�g���N�^
	void Start();							//�J�n
	void Update();							//�X�V
	void Draw();							//�`��
	void OnCollisionEnter(Object*);			//�Փ˔���

	void StopUpdate();						//�@�\��~(�X�V)
	void ResumeUpdate();					//�@�\�ĊJ(�X�V)

	void Use();								//�@�\�ĊJ(�`��ƍX�V)
	void NoUse();							//�@�\�ĊJ(�`��ƍX�V)

	void StopDraw();						//�`���~
	void ResumeDraw();						//�`��ĊJ

	//�Q�b�^�[
	DRAW_ORDER GetDrawOrder();				//���̃I�u�W�F�N�g�̕`�揇�̎擾
	UPDATE_ORDER GetUpdateOrder();			//���̃I�u�W�F�N�g�̍X�V���̎擾
	/** @brief �R���|�[�l���g���X�g�𓾂�*/
	std::list<Component*> GetComponentList();

	//�Z�b�^�[
	void Delete();							//�I�u�W�F�N�g�̍폜

	//GetComponent
	//�I�u�W�F�N�g�������Ă���R���|�[�l���g���擾����
	//�R���|�[�l���g���X�g����w�肵���|�C���^���擾����֐�

	template<class T>
	T* GetComponent() {
		for (auto&& com : ComponentList) {
			//�e���v���[�g�Ŏw�肵���N���X�ɃL���X�g����
			T* buff = dynamic_cast<T*>(com);

			//������
			if (buff != nullptr) {
				return buff;
			}
		}
		//���s�����Ƃ�
		return nullptr;
	}

	//AddComponent()
	//�I�u�W�F�N�g�ɃR���|�[�l���g�ɒǉ�����
	//�I�u�W�F�N�g�����R���|�[�l���g���X�g�Ɏw�肵���R���|�[�l���g��ǉ�����
	//���̂Ƃ��A�R���|�[�l���g�̏��L�҂̃I�u�W�F�N�g�̃|�C���^���w�肳���
	template<class T>
	T* AddComponent() {
		T* component = new T();		//�R���|�[�l���g�̐���
		component->Parent = this;	//�R���|�l���g�̎�����̐ݒ�

		//�X�V���������߂�
		std::list<Component*>::iterator itr = ComponentList.begin();
		for (auto&& com : ComponentList) {
			//���݂̃I�u�W�F�N�g�̋@�\���擾
			Component* buff = (Component*)com;

			//�X�V�̗D��x�������Ƃ��A���[�v��������
			if (component->m_eUpdateOrder <= buff->m_eUpdateOrder) {
				break;
			}
			else {
				//�X�V������x�点��
				itr++;
			}
		}

		//�R���|�[�l���g���X�g�ɑ}��
		ComponentList.insert(itr,component);
		
		return component;
	}

	//���O���擾����
	constexpr const std::string& GetName() const {
		return m_szName;
	}

	//�폜�t���O�̎擾
	constexpr const bool GetDeleteFlag() const {
		return m_bDelete;
	}
};



#endif