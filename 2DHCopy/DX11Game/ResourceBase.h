/**
* @file		ResourceBase.h
* @brief	�O������ǂݍ��ރf�[�^���Ǘ�
* @author	�u���z�S
* @date		2022/7/29
*/

/**�C���N���[�h�K�[�h**/
#ifndef _RESOURCEBASE_H_
#define _RESOURCEBASE_H_

/**�C���N���[�h��**/
#include <string>

/**�N���X��`**/
/**
* @brief	�O���f�[�^���Ǘ�����
* @detail	�܂�O���f�[�^���Ǘ����邱��
*/
class ResourceBase {
private:
	//�����o�ϐ�

	//�����o�֐�

protected:
	//�����o�ϐ�
	/** @brief �t�@�C���̖��O*/
	std::string m_fileName;

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	ResourceBase();
	/** @brief �f�X�g���N�^*/
	~ResourceBase();

	/** @brief ���[�h*/
	virtual bool Load(std::string fileName) = 0;
	/** @brief �Z�[�u*/
	virtual bool Save(std::string fileName) = 0;
};


#endif