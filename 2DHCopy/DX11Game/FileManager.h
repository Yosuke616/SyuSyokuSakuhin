/**
* @file		FileManager.h
* @brief	CSV�Ƃ��ǂݍ��񂾂菑�����񂾂肷����
* @author	�u���z�S
* @date		2022/7/29
*/

/**�C���N���[�h�K�[�h**/
#ifndef _FILEMANAGER_H_
#define _FILEMANAGER_H_

/**�C���N���[�h��**/
#include <map>
#include "ResourceBase.h"

/**�萔��`**/
//�X�e�[�W���
#define STAGE_1_CSV	("Asserts/Stage_1.csv")
/**�N���X��`**/
/**
* @brief	�A�Z�b�g�ɑ��݂���t�@�C�����Ǘ�����		
* @detail	�X�e�[�W���ƂɓǍ�������߂��肷��
*/
class FileManager {
private:
	//�����o�ϐ�
	/** @brief �t�@�C���̖��O�Ɠǂݍ��ނ��̂�R�Â���*/
	using ResourceMap = std::map<std::string,ResourceBase*>;
	/** @brief �t�@�C����ǂݍ��ނ��߂̃}�b�v�𒲂ׂ�C�e���[�^�[*/
	using ResourceMapIt = ResourceMap::iterator;
	/** @brief �t�@�C��*/
	using ResourceMapPair = std::pair<std::string,ResourceBase*>;

	/** @brief ���\�[�X�̈ꊇ�Ǘ�*/
	ResourceMap m_Resources;
	/** @brief �C���X�^���X*/
	static FileManager* m_pInstance;

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	FileManager();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �C���X�^���X�̎擾*/
	static FileManager* GetInstance();
	/** @brief �C���X�^���X�̔j��*/
	static void Destroy();
	/** @brief �f�X�g���N�^*/
	~FileManager();
	/** @brief ���[�h*/
	bool Load(std::string fileName);

	/** @brief �f�[�^�擾*/
	ResourceBase* Get(std::string fileName);
};

#endif