/**
* @file		ModelManager.h
* @brief	���f�����Ǘ����邽�߂̃}�l�[�W���[
* @author	�u���z�S
* @date		2022/7/7
*/
//�C���N���[�h�K�[�h
#ifndef _MODEL_MANAGER_H_
#define _MODEL_MANAGER_H_

//�C���N���[�h��
#include <unordered_map>
#include "mainApp.h"
#include "AssimpModel.h"

//�萔��`
//��
#define KARIKARIMODEL			(0)
#define PATH_MODEL_KARIKARI		("data/model/RosaRosa1.fbx")
#define BLOCK_KARIKARI			(1)
#define PATH_MODEL_BLOCKKARI	("data/model/BLOCK_KARI.fbx")

//�N���X��`
/**
 * @class	ModelManager
 * @brief	���f���}�l�[�W���[
*/
class ModelManager {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X*/
	static ModelManager* m_pInstance;
	/** @brief ���f�����X�g*/
	std::unordered_map<int, CAssimpModel*>	m_ModelList;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	ModelManager();

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �f�X�g���N�^*/
	~ModelManager();
	/** @brief �C���X�^���X��*/
	static ModelManager* GettInstance();
	/** @brief ���f���}�l�[�W���[�̉��*/
	static void Uninit();
	/** @brief ���f���̒ǉ�*/
	bool AddModel(const char* pszFileName,int nModelID);
	/** @brief ���f���̎擾*/
	CAssimpModel* GetModel(int nModelID);
	/** @brief ���f���폜*/
	bool EraseModel(int nModelID);

};

#endif