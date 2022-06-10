#ifndef _SCENE_H_
#define _SCENE_H_

/*
	�쐬��:YOSUKE SHIMIZU 
	�S�ẴV�[���̌��ɂȂ�e�N���X
	�I���̎n�܂�

	�X�V
	�u���@�쐬�J�n�@2022/6/3
*/

//===== �C���N���[�h ======

//===== �N���X��` ======
class Scene
{
protected:
	//=== �����o�ϐ� ===


public:
	//=== �����o�֐� ===
	Scene() {}							//	�R���X�g���N�^
	virtual ~Scene() {}					//	(���z�֐�)�f�X�g���N�^
	virtual void Init() = 0;			//	(�������z�֐�)������
	virtual void Uninit() = 0;			//	(�������z�֐�)�I��
	virtual void Update() = 0;			//	(�������z�֐�)�X�V
	virtual void Draw() = 0;			//	(�������z�֐�)�`��
};

#endif