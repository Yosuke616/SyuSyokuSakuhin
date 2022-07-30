/**
* @file		StageManager.h
* @brief	�X�e�[�W���Ƃ̏����Ǘ����邽�߂̃N���X
* @author	�u���z�S
* @date		2022/7/27
*/

/**�C���N���[�h�K�[�h**/
#ifndef _STAGE_MANAGER_H_
#define _STAGE_MANAGER_H_

/**�C���N���[�h��**/
#include "mainApp.h"
#include "Object.h"
#include "Component.h"

#include <map>
#include <vector>

/**�N���X��`**/
/**
* @brief	�X�e�[�W���Ǘ����邽�߂̃}�l�[�W���[
* @detail	�n�`����ǂ�ȃI�u�W�F�N�g�����邩�̊Ǘ�
*/
class StageManager {
private:
	//�����o�ϐ�
	/** @brief �C���X�^���X*/
	static StageManager* m_pInstance;

	//vector�̓񎟌��z��
	/** @brief �Z��*/
	using Cell = int;
	/** @brief �s*/
	using Column = std::vector<Cell>;
	/** @brief ��*/
	using Row = std::vector<Column>;
	/** @brief �X�e�[�W�̔z��*/
	Row m_StageGrid;
	/** @brief �X�e�[�W�̗v�f��x*/
	int m_nBlock_X;
	/** @brief �X�e�[�W�̗v�f��y*/
	int m_nBlock_Y;
	/** @brief �X�e�[�W�̑傫��*/
	XMFLOAT2 m_vStageSize;
	/** @brief ���[���h���W�ł̗����Ɖ���*/
	XMFLOAT2 m_WorldSize;
	/** @brief �X�e�[�W�`��ׂ̈̋N�_*/
	XMFLOAT2 m_BasePos;

	/**�f�o�b�O�p**/
	/** @brief �J�[�\���ɕ\���������*/
	Object* m_pShowObject;
	/** @brief �}�E�X���W�̗v�f��x*/
	int m_Element_X;
	/** @brief �}�E�X���W�̗v�f��y*/
	int m_Element_Y;
	/** @brief �z�u���镨�̎��*/
	int m_nMapSta;

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	StageManager();
	/** @brief �X�e�[�W��Ԃ̕ۑ�*/
	bool Save(int Stage);
	/** @brief �X�e�[�W�̓Ǎ�*/
	bool Load(std::string file);
	
	/**�f�o�b�N**/
	/** @brief �X�e�[�W�I�u�W�F�N�g����*/
	void UpdateShowObject(float fPosX, float fPosY);
	/** @brief �X�e�[�W�I�u�W�F�N�g����*/
	void CreateBlock_Deb(float fPosX, float fPosY, int nNumber, int x, int y, int nCreate_Status);

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �C���X�^���X����*/
	static StageManager* GetInstance();
	/** @brief �C���X�^���X�j��*/
	static void Destroy();
	/** @brief �f�X�g���N�^*/
	~StageManager();
	/** @brief �X�V����*/
	void Update();
	/** @brief �`�揈��*/
	void Draw();

	/** @brief �n�`����*/
	void CreateStage(int Stage_State);

	/** @brief �X�e�[�W�I�u�W�F�N�g����(�Q�[�����[�h)*/
	Object* CreateBlock(float fPosX, float fPosY, int nState, int nBlockID);

	/** @brief �u���b�N�̔ԍ���������擾����*/
	Object* GetBlock(int nNum);
};

#endif