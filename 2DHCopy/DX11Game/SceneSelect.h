/**
* @file		SceneSelect.h
* @brief	�X�e�[�W�Z���N�g���
* @author	�u���z�S
* @date		2022/8/16
*/

/**�C���N���[�h�K�[�h**/
#ifndef _SCENE_SELECT_H_
#define _SCENE_SELECT_H_

/**�C���N���[�h��**/
#include "Scene.h"
#include <vector>

/**�N���X��`**/
/**
* @brief	�X�e�[�W�I��p�̃N���X
* @detail	�F�X
*/
class StageSelect : public Scene {
private:
	//�����o�ϐ�
	//�x�N�^�[�̓񎟌��z��
	/** @brief �Z��(���g)*/
	using Cell = int;					
	/** @brief �s*/
	using Column = std::vector<Cell>;	
	/** @brief ��*/
	using Row = std::vector<Column>;	
	/** @brief �N���A�󋵂̔z��*/
	static Row m_StageGrid;

	//�����o�֐�
	///** @brief ���[�h*/
	//static bool Load();
	///** @brief �Z�[�u*/
	//static bool Save();

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	StageSelect();
	/** @brief �f�X�g���N�^*/
	~StageSelect() override;
	/** @brief ����������*/
	void Init() override;
	/** @brief �I������*/
	void Uninit() override;
	/** @brief �X�V����*/
	void Update() override;
	/** @brief �`�揈��*/
	void Draw() override;

	///** @brief �I���\���ǂ����̎擾*/
	//static bool GetSelectAble(int stage_num);
	///** @brief �I���\���ǂ����̔z��̎擾*/
	//static Row GetSelectGrid();
	///** @brief CSV�t�@�C���̎擾*/
	//static bool NewGame();
	///** @brief �N���A�󋵂̕ۑ�*/
	//static bool SaveClearInfo(int nStage);
};

#endif