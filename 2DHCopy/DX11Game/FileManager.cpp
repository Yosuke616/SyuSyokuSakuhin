/**�C���N���[�h��**/
#include "FileManager.h"
#include "ResourceCSV.h"

/**�ÓI�����o�ϐ�**/
FileManager* FileManager::m_pInstance = nullptr;

/**
* @fn		FileManager::GetInstance
* @brief	�C���X�^���X���擾����
*/
FileManager* FileManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new FileManager();
	}
	return m_pInstance;
}

/** 
* @fn		FileManager::Destroy
* @brief	�C���X�^���X�̔j��
*/
void FileManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
	}
}

/**
* @fn		FileManager::FileManager
* @brief	�R���X�g���N�^�͂������Ȃ�
*/
FileManager::FileManager() {

}

/**  
* @fn		FileManager::~FileManager
* @brief	�f�X�g���N�^�A��������Ⴄ��
*/
FileManager::~FileManager() {
	//�ǂݍ��܂ꂽ�f�[�^�����
	ResourceMapIt it = m_Resources.begin();
	while (it != m_Resources.end())
	{	
		delete it->second;
		it++;
	}
}

/**
* @fn		FileManager::Load
* @brief	���[�h���܂�
*/
bool FileManager::Load(std::string fileName) {
	//�ǂݍ��ݍς݂̃f�[�^�ł���Ώ������Ȃ�
	if (m_Resources.find(fileName) != m_Resources.end()) {
		//�L�[��������Ȃ���Γǂݍ��܂�Ă���
		return true;
	}

	//�L�[���Ȃ��̂œǍ��������J�n
	//�g���q���ƂɃf�[�^�N���X���������m��

	ResourceBase* pResource;
	if (fileName.find(".csv") != std::string::npos) {
		pResource = new ResourceCSV();
	}
	else {
		return false;
	}

	//�f�[�^�̓Ǎ�
	bool result = false;
	result = pResource->Load(fileName.c_str());
	if (!result) {
		//�ǂݍ��߂Ȃ������ꍇ
		delete pResource;
		return false;
	}

	//�}�b�v�ɓo�^
	m_Resources.insert(ResourceMapPair(fileName,pResource));

	return true;
}

/**
* @fn		FileManager::Get
* @brief	�f�[�^���擾����
* @param	(string)	�ǂ��̃f�[�^���擾���邩
* @return	(ResourceBase*)	�擾�����f�[�^��Ԃ�
*/
ResourceBase* FileManager::Get(std::string fileName) {
	//�f�[�^�����݂��邩�ǂ���
	if (m_Resources.find(fileName) == m_Resources.end()) {
		//����������nullptr
		return nullptr;
	}

	//���݂���f�[�^��Ԃ�
	return m_Resources.at(fileName);
}
