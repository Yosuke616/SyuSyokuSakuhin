//�C���N���[�h��
#include "ModelManager.h"
#include <crtdbg.h>

//�ÓI�����o�ϐ�
ModelManager* ModelManager::m_pInstance = nullptr;

/**
* @fn		ModelManager::ModelManager
* @brief	���...�ݽ�׸�...��...
*/
ModelManager::ModelManager() {

}

/**
* @fn		ModelManager::~ModelManager
* @brief	�f�X�g���N�^���ĉ����낤
*/	
ModelManager::~ModelManager() {
	for (auto buff : m_ModelList) {
		buff.second->Release();
		delete buff.second;
	}
	m_ModelList.clear();
}

/**
* @fn		ModelManager::GetInstance
* @brief	�C���X�^���X��
* @return	(ModelManager*)	���f���}�l�[�W���[�̃|�C���^��Ԃ�
*/
ModelManager* ModelManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new ModelManager;
	}
	return m_pInstance;
}

/**
* @fn		ModelManager::Uninit
* @brief	���f���}�l�[�W���[�J��
*/
void ModelManager::Uninit() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		ModelManager::AddModel
* @brief	���f���̒ǉ�
* @param	(const char*)	�t�@�C���̖��O
* @param	(int)	���Ԗڂ̔ԍ��ɕR�Â��Ă��邩
* @return	(bool)	���f����ǂݍ��߂����ǂ���
*/
bool ModelManager::AddModel(const char* pszFileName, int nModelID) {
	//���f�������ɂ�������Ԃ�
	if (m_ModelList[nModelID] != nullptr) {
		return true;
	}

	//�ϐ��錾
	CAssimpModel* model = new CAssimpModel();

	//���f���f�[�^�̓Ǎ�
	if (!model->Load(GetDevice(),GetDeviceContext(),pszFileName)) {
		MessageBoxA(GetMainWnd(), "���f���f�[�^�ǂݍ��݃G���[", "InitModel", MB_OK);
		return false;
	}

	//�}�b�v�ɒǉ�
	m_ModelList[nModelID] = model;

	return true;
}

/**
* @fn		ModelManager::GetModel
* @brief	���f���̎擾
* @param	(int)	�擾���������f���̔ԍ�
* @return	(CAssimpModel*)	���f���̏���Ԃ�
*/
CAssimpModel* ModelManager::GetModel(int nModelID) {
	//����
	auto itr = m_ModelList.find(nModelID);

	//���ʂ̔���
	if (itr == m_ModelList.end()) {
		//����������
		return nullptr;
	}
	else {
		return (*itr).second;
	}
	return nullptr;
}

/**
* @fn		ModelManager::EraseModel
* @brief	���f���̍폜
* @param	(int)	�����������f���̔ԍ�
* @return	(bool)	���������ǂ���
*/
bool ModelManager::EraseModel(int nModelID){
	//����
	auto Model = this->GetModel(nModelID);

	m_ModelList.erase(nModelID);
	return true;

	return false;
}