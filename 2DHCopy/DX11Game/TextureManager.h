/**
* @file		TextureManager.h
* @brief	�e�N�X�`���̏����Ǘ�����}�l�[�W���[
* @author	�u���z�S
* @date		2022/6/28
*/

//�C���N���[�h�K�[�h
#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

//�C���N���[�h��
#include <unordered_map>
#include "mainApp.h"

//�萔��`
/** @brief �K�v�ɂȂ��Ă���e�N�X�`���������ɏ����Ă���*/
//DX�����
#define DXCHAN_STAND_TEX_NUM				(0)
#define PATH_TEX_DXCHAN_STAND				("data/texture/Stand_Def.PNG")

#define DEBUG_BLOCK_NUM						(1)
#define PATH_TEX_DEBUG_BLOCK				("data/texture/Debug.jpg")

//�^�C�g����ʗp
#define TITLE_BACK_GROUND_NUM				(2)
#define PATH_TEX_TITLE_BACK_GROUND			("data/texture/TitleBack.jpg")

//�{�^��
#define TITLE_BUTTON_NUM					(3)
#define PATH_TEX_TITLE_BUTTON				("data/texture/press_enter.png")

//�N���X��`
/**
* @class	TextureManager
* @brief	�e�N�X�`�����Ǘ�����N���X
*/
class TextureManager {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X�p�ϐ�*/
	static TextureManager* m_pInstance;
	/** @brief �e�N�X�`�����X�g*/
	std::unordered_map<int, ID3D11ShaderResourceView*> m_TextureList;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	TextureManager();

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �f�X�g���N�^*/
	~TextureManager();
	/** @brief �C���X�^���X��*/
	static TextureManager* GetInstance();
	/** @brief �I������*/
	static void Uninit();
	/** @brief �e�N�X�`���̒ǉ�*/
	bool AddTexture(const char* pszFileName,int nTexID);
	/** @brief �e�N�X�`���擾*/
	ID3D11ShaderResourceView* GetTexture(int nTexID);
	/** @brief �e�N�X�`���폜*/
	bool EraseTexture(int nTexID);
	/** @brief �ǂݍ��񂾃e�N�X�`���̐�*/
	int GetSize();
};

#endif