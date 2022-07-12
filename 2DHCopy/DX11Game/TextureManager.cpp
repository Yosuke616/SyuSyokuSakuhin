//�C���N���[�h��
#include "TextureManager.h"
#include "mainApp.h"
#include "Texture.h"
#include <crtdbg.h>

//�ÓI�����o�ϐ�
TextureManager* TextureManager::m_pInstance = nullptr;

/**
* @fn		TextureManager::TextureManager
* @brief	�R���X�g���N�^�Ƃ����͕̂������Ă���
*/
TextureManager::TextureManager() {

}

/**
* @fn		TextureManager::~TextureManager
* @brief	������������f�X�g���N�^��
*/
TextureManager::~TextureManager() {
	for (auto buff : m_TextureList) {
		SAFE_RELEASE(buff.second);
	}
	m_TextureList.clear();
}

/**
* @fn		TextureManager::GetInstance
* @brief	�e�N�X�`���}�l�[�W���[�̃C���X�^���X��
*/
TextureManager* TextureManager::GetInstance() {
	if (!m_pInstance) {
		m_pInstance = new TextureManager;
	}
	return m_pInstance;
}

/**
* @fn		TextureManager::Uninit
* @brief	�e�N�X�`���}�l�[�W���[�̉��
*/
void TextureManager::Uninit() {
	if (m_pInstance) {
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

/**
* @fn		TextureManager::AddTexture
* @brief	�e�N�X�`����ǉ����Ă���
* @param	(char*)	�t�@�C���̖��O�������Ă���|�C���^
* @param	(int)	�e�N�X�`���ƕR�Â���L�[
* @return	�������������s�������̐^�U
*/
bool TextureManager::AddTexture(const char* pszFileName,int nTexID) {
	//�e�N�X�`������������Ԃ�
	if (m_TextureList[nTexID] != nullptr) {
		return true;
	}

	//�ϐ��錾
	/** @brief �e�N�X�`���̏�񂪓����Ă���|�C���^*/
	ID3D11ShaderResourceView* pTex;
	/** @brief �������p�ϐ�*/
	HRESULT hr;

	//�e�N�X�`���̓Ǎ�
	hr = CreateTextureFromFile(GetDevice(),pszFileName,&pTex);

	/** @brief ��ł���*/
	if (FAILED(hr)) {
		_ASSERT_EXPR(false,L"NO_TEXTURE");
		return false;
	}

	//map�ɒǉ�
	m_TextureList[nTexID] = pTex;

	return false;
}

/**
* @fn		TextureManager::GetTexture
* @brief	�e�N�X�`���̎擾
* @param	(int)	�e�N�X�`���ƕR�Â��Ă���L�[
* @return	(ID3D~*)���������g�p�\�ȃe�N�X�`���ԍ��A�����������O
*/
ID3D11ShaderResourceView* TextureManager::GetTexture(int nTexID) {
	//����
	auto itr = m_TextureList.find(nTexID);

	//��������
	if (itr == m_TextureList.end()) {
		//����������
		return nullptr;
	}
	else {
		//��������
		return itr->second;
	}

	return nullptr;
}

/**
* @fn		TextureManager::EraseTexture
* @brief	�e�N�X�`���̍폜
* @param	(int)	�e�N�X�`���ƕR�Â��Ă���L�[
* @return	(bool)	�����e�N�X�`���������Ȃ����̐^�U
*/
bool TextureManager::EraseTexture(int nTexID) {
	//����
	auto Tex = this->GetTexture(nTexID);

	//��������
	if (Tex) {
		m_TextureList.erase(nTexID);
		return true;
	}

	//����������
	return false;
}

/**
* @fn		TextureManager::GetSize
* @brief	���X�g�T�C�Y
* @return	�ǂݍ��܂�Ă���e�N�X�`���̐�
*/
int TextureManager::GetSize() {
	//�T��
	return (int)(m_TextureList.size());
}


