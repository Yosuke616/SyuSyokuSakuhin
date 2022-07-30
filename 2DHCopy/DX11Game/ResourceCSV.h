/**
* @file		ResourceCSV.h
* @brief	CSV�̒��g�����ۂɂ̂������肷����
* @author	�u���z�S
* @date		2022/7/29
*/

/**�C���N���[�h�K�[�h**/
#ifndef _RESOURCECSV_H_
#define _RESOURCECSV_H_

/**�C���N���[�h��**/
#include <string>
#include <vector>
#include "ResourceBase.h"

/**�N���X��`**/
/**
* @brief	CSV�ǂݍ��񂶂Ⴄ��[��
* @detail	���ۂɃt�@�C���̒��܂œ���ǂݍ��񂾂菑�����񂾂肷��
*/
class ResourceCSV : public ResourceBase {
private:
	//�����o�ϐ�
	/** @brief �񎟌��z���ǂݍ��ވ�*/
	/** @brief �Z��*/
	using Cell = std::string;
	/** @brief �s*/
	using Column = std::vector<Cell>;
	/** @brief ��*/
	using Row = std::vector<Column>;

	/** @brief �ǂݍ���CSV�f�[�^�̊i�[��*/
	Row m_Grid;

	//�����o�֐�

protected:
	//�����o�ϐ�

	//�����o�֐�

public:
	//�����o�ϐ�

	//�����o�֐�
	/** @brief �R���X�g���N�^*/
	ResourceCSV();
	/** @brief �f�X�g���N�^*/
	~ResourceCSV();

	/** @brief ���[�h*/
	bool Load(std::string fileName)override;
	/** @brief �Z�[�u*/
	bool Save(std::string fileName)override;

	/**�Q�b�^�[**/
	/** @brief �Z���̏ꏊ�ɓ����Ă��镶���̎擾*/
	std::string GetStr(int x,int y);
	/** @brief �Z���ɋL������Ă��镶���𐔒l�Ŏ擾*/
	int GetInt(int x,int y);
	/** @brief �Z���ɋL������Ă��镶���𐔒l(float�^)�Ŏ擾*/
	float GetFloat(int x,int y);
	/** @brief �s�̐����擾*/
	int GetRowSize();
	/** @brief ��̐����擾*/
	int GetColumnSize(int row);

	/**�Z�b�^�[**/
	/** @brief �Z���ɕ�������͂���*/
	void SetStr(int x,int y,std::string str);
	/** @brief �Z���ɐ��l����͂���*/
	void SetInt(int x,int y,int num);
	/** @brief �Z���ɐ��l(�����_����)����͂���*/
	void SetFloat(int x,int y,float num);
	/** @brief �s�̐����w��*/
	void SetRowSize(int Size);
	/** @brief ��̐����w��*/
	void SetColumnSize(int row,int size);

};

#endif