/**���ӊ��N�̂��(���̕s��)**/
#define _CRT_SECURE_NO_WARNINGS

/**�C���N���[�h��**/
#include "ResourceCSV.h"
#include <sstream>

/**
* @fn		ResourceCSV::ResourceCSV
* @brief	�ȂɂȂ��Ă�񂾂�ˁA�R���X�g���N�^�����
*/
ResourceCSV::ResourceCSV() {

}

/**
* @fn		ResourceCSV::~ResourceCSV
* @brief	�f�X�g���N�^���O�͂����ŏ����˂΂Ȃ��E�E�E�킩���
*/
ResourceCSV::~ResourceCSV() {

}

/**
* @fn		ResiurceCSV::Load
* @brief	CSV�t�@�C���̓Ǎ�
* @param	(string)	�ǂݍ��ރt�@�C���̖��O
* @return	(bool)		���Ȃ��ǂݍ��߂����ǂ���
*/
bool ResourceCSV::Load(std::string fileName) {
	//�t�@�C���̓Ǎ�
	FILE* fp;
	fp = fopen(fileName.c_str(),"rt");
	if (!fp) {
		//�ǂݍ��߂Ȃ�����
		return false;
	}

	//���g�̋z�o��
	size_t fileSize;
	fseek(fp, 0, SEEK_END);		//	�t�@�C���I�[�܂Ń|�C���^���ړ�
	fileSize = ftell(fp);		//	�t�@�C���|�C���^���n�_����ǂꂾ������Ă��邩 = �T�C�Y
	fseek(fp, 0, SEEK_SET);		//	�t�@�C���̐擪�Ɉړ�
	std::string text;
	text.resize(fileSize);		//	vector�݂����Ɉ�����???
	if (fileSize > 0)
		fread(&text.front(), fileSize, 1, fp);	//	text��text�̐擪����fileSize���̓ǂݍ��݂��P�񂷂�

	//�t�@�C�������
	fclose(fp);

	//--- csv�̉��
//	1�s����؂�
	std::vector<std::string> lines;
	int stsrtIndex = 0;				//	��؂�̊J�n�ʒu
	int endIndex = 0;				//	��؂�̏I���ʒu
	while (endIndex != std::string::npos)	// string::npos = find�Ō�����Ȃ��������̒l
	{
		//	'\n'(���s�̕����L�����o�Ă���ʒu�̒T��
		endIndex = (int)text.find("\n", stsrtIndex);	//	������Ȃ�������string::npos�̒l���i�[

		//	�������Ă���i�[���ēǂݍ��݂܂���
		if (endIndex != std::string::npos)
		{
			//	�T���J�n�ʒu���猩�����������܂ł̕�����؂�o��
			lines.push_back(text.substr(stsrtIndex, endIndex - stsrtIndex));
		}

		//	���̒T���J�n�ʒu�̍X�V
		stsrtIndex = endIndex + 1;
	}

	//	1�Z�����f�[�^���i�[
	std::vector<std::string>::iterator linesItr = lines.begin(); // string��vector
	while (linesItr != lines.end())
	{
		Column column;	// ��

		stsrtIndex = 0;
		endIndex = 0;

		while (endIndex != std::string::npos)
		{
			//	','�̈ʒu��T��
			endIndex = (int)linesItr->find(",", stsrtIndex);	// ���̍s��,��T��

			//	��قǎ��o�����s��','�ŋ�؂��Ď��o��
			if (endIndex != std::string::npos)
			{
				//	�T���J�n�ʒu���猩�����������܂ł̕�����؂�o��
				column.push_back(linesItr->substr(stsrtIndex, endIndex - stsrtIndex));
			}
			else
			{
				column.push_back(linesItr->substr(stsrtIndex));
			}

			//	���̒T���J�n�ʒu�̍X�V
			stsrtIndex = endIndex + 1;
		}

		//	�Z�����Ƃɋ�؂����s��S�̂̃f�[�^(grid)�֒ǉ�
		m_Grid.push_back(column);

		//	���̍s
		linesItr++;
	}

	return true;

}

/**
* @fn		ResourceCSV::Save
* @brief	CSV�t�@�C���ɏ�������
* @param	(string)	�Ǎ���̖��O
* @return	(bool)	�����ɏ������߂����ǂ���
*/
bool ResourceCSV::Save(std::string fileName) {
	//	������
	FILE *pFile;
	pFile = fopen(fileName.c_str(), "wt");
	if (!pFile)
	{
		//	�������ݐ悪
		return false;
	}

	int cnt_x = 0;
	int cnt_y = 0;

	for (auto&& row : m_Grid)
	{
		cnt_y++;
		for (auto&& cell : row)
		{
			cnt_x++;

			// �Z���̒��g�i�[
			//fwrite(cell.c_str(), sizeof(char) * cell.size(), 1, pFile);
			fprintf(pFile, cell.c_str());
			//	csv�̓Z����","�ŋ�؂�
			if ((int)row.size() != cnt_x)	// �Ō�͂���Ȃ�
			{
				//fwrite(",", sizeof(char), 1, pFile);
				fprintf(pFile, ",");
			}
		}

		//	csv��"\n"�ŉ��s����
		if ((int)row.size() != cnt_y)	// �Ō�͂���Ȃ�
		{
			//fwrite("\n", sizeof(char) * 2, 1, pFile);
			fprintf(pFile, "\n");
		}

		// �J�E���g������
		cnt_x = 0;
	}

	fclose(pFile);

	return true;
}

/**
* @fn		ResourceCSV::GetStr
* @brief	�Z���̏c�����Ⴄ
* @param	(int)	���̐�
* @param	(int)	�c�̐�
* @return	�w��̏ꏊ�̕�������Ԃ�(���ɒN�����܂����)
*/
std::string ResourceCSV::GetStr(int x,int y) {
	return m_Grid[y][x];
}

/**
* @fn		ResourceCSV::GetInt
* @brief	�Z���̕�����𐔒l�ɕς���
* @param	(int)	���̐�
* @param	(int)	�c�̐�
* @return	(int)	�Ԃ����l
*/
int ResourceCSV::GetInt(int x, int y) {
	//�������int�ɕϊ�����
	return atoi(m_Grid[y][x].c_str);
}

/**
* @fn		ResourceCSV::GetFloat
* @brief	�Z���̕�����float�Ŏ擾
* @param	(int)	���̐�
* @param	(int)	�c�̐�
* @return	(float)	�Ԃ����l(it���������_)
*/
float ResourceCSV::GetFloat(int x,int y) {
	//�������float�ɕς���
	return (float)atof(m_Grid[y][x].c_str);
}

/**
* @fn		ResourceCSV::GetRowSize
* @brief	�s�̐����擾��
* @return	(int)	���s���������Ԃ�
*/
int ResourceCSV::GetRowSize() {
	return  (int)m_Grid.size();
}

/**
* @fn		ResourceCSV::GetColumnSize
* @brief	��̐����擾�����
* @param	(int)	���s�ڂ��̐��l
* @return	(int)	�ǂ̂��炢����������Ԃ�
*/
int ResourceCSV::GetColumnSize(int row) {
	//��O����
	if ((int)m_Grid.size() <= row) {
		return 0;
	}

	return (int)m_Grid[row].size();
}

/**
* @fn		ResourceCSV::SetStr
* @brief	�Z���ɕ���������
* @param	(int)		���̐�
* @param	(int)		�c�̐�
* @param	(string)	���ꂽ��������
*/
void ResourceCSV::SetStr(int x,int y,std::string str) {
	m_Grid[y][x] = str;
}

/**
* @fn		ResourceCSV::SetInt
* @brief	�Z���̒��ɐ��l������
* @param	(int)	���̐�
* @param	(int)	�c�̐�
* @param	(int)	���ꂽ�����l
*/
void ResourceCSV::SetInt(int x,int y,int num) {
	// ����Ȃ��s�͒ǉ�
	if (y >= (int)m_Grid.size())
	{
		m_Grid.resize(y + 1);
	}

	// ����Ȃ���͒ǉ�
	if (x >= (int)m_Grid[y].size())
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_Grid[y].push_back(ss.str());
	}
	else
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_Grid[y][x] = ss.str();
	}
}

/**
* @fn		ResourceCSV::SetFloat
* @brief	�Z���Ɏ���������
* @param	(int)	���̐�
* @param	(int)	�c�̐�
* @param	(float)	���ꂽ������
*/
void ResourceCSV::SetFloat(int x,int y,float num) {
	// ����Ȃ��s�͒ǉ�
	if (y >= (int)m_Grid.size())
	{
		m_Grid.resize(y + 1);
	}

	// ����Ȃ���͒ǉ�
	if (x >= (int)m_Grid[y].size())
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_Grid[y].push_back(ss.str());
	}
	else
	{
		// �����𕶎��Ƃ��Ċi�[����
		std::ostringstream ss;
		ss << num;
		m_Grid[y][x] = ss.str();
	}
}

/**
* @fn		ResourceCSV::SetRowSize
* @brief	�s�̐���ݒ�
* @param	(int)	�ǂ̈ʂ̐��ɐݒ�
*/
void ResourceCSV::SetRowSize(int size) {
	m_Grid.resize(size);
}

/**
* @fn		ResourceCSV::SetColumnSize
* @brief	��̐���ݒ�
* @param	(int)	�s�̎w��
* @param	(int)	�ǂ̈ʂ̑傫����ݒ�
*/
void ResourceCSV::SetColumnSize(int row,int size) {
	//��O����
	if ((int)m_Grid.size() <= row ){
		return;
	}
	m_Grid[row].resize(size);
}