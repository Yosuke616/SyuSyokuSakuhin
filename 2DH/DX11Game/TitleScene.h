/*
	�쐬�ҁ@�u���z�S�@2022/6/3
	�^�C�g���V�[�������
*/

 //===== �C���N���[�h�K�[�h ======
#ifndef __SCENE_TITLE_H__
#define __SCENE_TITLE_H__

//===== �C���N���[�h =====
#include "Scene.h"

//2022/1/28/Tutorial_CSV Shimizu Yosuke ------------------------------
//#include "sceneGame.h"
#include <vector>
//---------------------------------------------------------------------



//===== �N���X��` =====
class SceneTitle : public Scene
{
public:
	SceneTitle();						//	�R���X�g���N�^
	~SceneTitle() override;				//	�f�X�g���N�^
	void Init() override;				//	������
	void Uninit() override;				//	�I������
	void Update() override;				//	�X�V
	void Draw() override;				//	�`��
};
#endif // __SCENE_TITLE_H__


