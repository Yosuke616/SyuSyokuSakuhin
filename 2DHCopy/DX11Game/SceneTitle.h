/*
	�u���z�S
	2022/6/8
	�^�C�g���V�[���̍쐬

*/

//�C���N���[�h�K�[�h
#ifndef _SCENE_TILTE_H_
#define _SCENE_TITLE_H_

//�C���N���[�h��
#include "Scene.h"

//�N���X��`
class SceneTitle :public Scene {
private:
	//�����o�ϐ�

	//�����o�֐�
protected:
	//�����o�ϐ�

	//�����o�֐�
public:
	//�����o�ϐ�

	//�����o�֐�
	SceneTitle();						//	�R���X�g���N�^
	~SceneTitle() override;				//	�f�X�g���N�^
	void Init() override;				//	������
	void Uninit() override;				//	�I������
	void Update() override;				//	�X�V
	void Draw() override;				//	�`��
};








#endif