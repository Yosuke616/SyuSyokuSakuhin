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
#define WALK_ENEMY_MODEL_NUM	(0)
#define PATH_WALK_ENEMY			("data/model/WalkEnemy.fbx")
#define MINT_GREEN_BLOCK_NUM	(1)
#define PATH_MINT_GREEN_BLOCK	("data/model/Grass_Block.fbx")
#define RARD_BLOCK_NUM			(2)
#define PATH_RARD_BLOCK			("data/model/Rard_Block.fbx")
#define GRASS_BLOCK_RE_NUM		(3)
#define PATH_GRASS_RE_BLOCK		("data/model/Grass_Block_RE.fbx")
#define RARD_BLOCK_RE_NUM		(4)
#define PATH_RARD_RE_BLOCK		("data/model/Rard_Block_RE.fbx")
#define KOBAN_MODEL_NUM			(5)
#define PATH_KOBAN				("data/model/Koban.fbx")
#define MAGA_MODEL_NUM			(6)
#define PATH_MAGA				("data/model/Magatama.fbx")

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
	static ModelManager* GetInstance();
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