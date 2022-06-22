/**
* @file		�t�@�C����
* @brief	�I�u�W�F�N�g�}�l�[�W���[�N���X
* @author   Shimizu Yosuke
* @date		2022/6/16
*
* @details  �I�u�W�F�N�g���Ǘ����Ăǂ���X�V���A�ǂ��`�悷�邩���Ǘ�����
* @note		�|�C���^�Ƃ�����������g����
*/

//�C���N���[�h�K�[�h
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

//�C���N���[�h��
#include <list>
#include <string>

//�O���錾
class Object;

//�N���X��`
/**
* @brief	�������ꂽ�I�u�W�F�N�g���Ǘ�����N���X
* @details	�C���X�^���X��������邽��1�������݂��Ȃ�
*/
class ObjectManager {
private:
	//�����o�ϐ�
	/** @brief �I�u�W�F�N�g���܂Ƃ߂čX�V���邽�߂̃��X�g*/	
	std::list<Object*>		m_UpdateList;
	/** @brief �I�u�W�F�N�g���܂Ƃ߂ĕ`�悷�邽�߂̃��X�g*/
	std::list<Object*>		m_DrawList;
	/** @brief �C���X�^���X�����p�ϐ�*/
	static ObjectManager*	m_pInstance;

	//�����o�֐�

protected:
	//�����o�ϐ�
	/** @brief �R���X�g���N�^*/
	ObjectManager();

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief*/
	static ObjectManager* GetInstance();
	/** @brief �f�X�g���N�^*/
	~ObjectManager();
	/** @brief ����������*/
	void Start();
	/** @brief �X�V����*/
	void Update();
	/** @brief �X�e�[�W�쐬���̍X�V����*/
	void UpdateEdit();
	/** @brief �`�揈��*/ 
	void Draw();
	/** @brief �I������*/
	void Uninit();

	/** @brief �I�u�W�F�N�g��ǉ�����*/
	void AddObject(Object* pObject);

	/** @brief ���O�����ŃI�u�W�F�N�g���폜����*/
	void DeleteObjectByName(std::string pszName,std::list<Object*>& list);

	/** @brief ���O����v�����I�u�W�F�N�g�����X�g�ɒǉ�*/
	std::list<Object*> GetObjectList(std::string name);

	/** @brief �X�V���~����֐�*/
	void NoFunction();

	/** @brief �X�V���ĊJ����֐�*/
	void Resume();

	//getter
	/** @brief ���O����I�u�W�F�N�g����肷��*/
	Object* GetGameObject(std::string name);
	/** @brief �X�V���X�g�̎擾*/
	std::list<Object*>& GetUpdateList();
	/** @brief �`�惊�X�g�̎擾*/
	std::list<Object*>& GetDrawList();

	/** @brief ���O����v���Ă���I�u�W�F�N�g���X�g��ǉ�����*/
	void GetGameObjects(std::string name,std::list<Object*>& resultList);
};


#endif