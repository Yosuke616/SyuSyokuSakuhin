#ifndef _OBJECT_INFO_H_
#define _OBJECT_INFO_H_

//�񋓑��錾

//�I�u�W�F�N�g�̍X�V����
enum UPDATE_ORDER {
	UPDATE_BG,			//�w�i
	UPDATE_PLAYER,		//�v���C���[
	UPDATE_FIELD,		//�n�`
	UPDATE_EFFECT,		//�G�t�F�N�g
	UPDATE_MODEL,		//���f��
	UPDATE_DEBUG,		//debug
	UPDATE_UI,			//UI
	UPDATE_OPTION,		//�I�v�V����
	UPDATE_GOLETTER,	//

	MAX_UPDATE_ORDER
};

//�I�u�W�F�N�g�̕`��̏���
enum DRAW_ORDER {
	DRAW_BG,			//�w�i
	DRAW_FIELD,			//�t�B�[���h
	DRAW_MODEL,			//���f��
	DRAW_PLAYER,		//�v���C���[
	DRAW_EFFECT,		//�G�t�F�N�g

	DRAW_DEBUG,			//debug
	DRAW_UI_FRAME,		//UI�g
	DRAW_GOLETTER,
	DRAW_UI,			//UI
	DRAW_OPTION_BLACK_OUT,
	DRAW_OPTION_FRAME,	//�I�v�V�����I�u�W�F�N�g
	DRAW_OPTION,		//�I�v�V����

	MAX_DRAW_ORDER
};

//@@@@@ �f�B�G�b�N�X����� ----------------------------------------
#define PLAYER_NAME					("DXchan")
#define DXCHAN_SIZE_X				(100.0f)
#define DXCHAN_SIZE_Y				(100.0f)
#define DXCHAN_SIZE_RUN_Y			(95.0f)
#define DXCHAN_COLL_SIZE_X			(25.0f)
#define DXCHAN_COLL_SIZE_Y			(80.0f)
#define DXCHAN_COLL_SIZE_Z			(100.0f)
#define DXCHAN_COLL_SIZE_RUN_X		(55.0f)
#define DXCHAN_COLL_OFFSET_RUN_X	(-8.0f)
#define DXCHAN_COLL_OFFSET_RUN_Y	(-2.0f)

//�U���p���
#define ATTACK_NAME					("Attack")
#define OFFSET_DX_X					(35.0f)
#define OFFSET_DX_Y					(25.0f)
#define ATTACK_COLL_X				(50.0f)
#define ATTACK_COLL_Y				(30.0f)

//@@@@@ �u���b�N�̊�{��� ----------------------------------------
#define BLOCK_NAME					("Block")
#define MAPCHIP_WIDTH				(40.0f)	
#define MAPCHIP_HEIGHT				(40.0f)	
#define BLOCK_SIZE_X				(50.0f)
#define BLOCK_SIZE_Y				(50.0f)
#define BLOCK_SIZE_Z				(50.0f)
#define BLOCK_COLL_SIZE_X			(43.5f)
#define BLOCK_COLL_SIZE_Y			(43.5f)
#define BLOCK_COLL_SIZE_Z			(45.0f)

//@@@@@ �G�̊�{��� ----------------------------------------------
//�G���Ƃɐݒ肷��l�͕ς���Ă���
#define ENEMY_NAME					("Enemy")
//�����G�@�����N���{�[
#define E_WALK_SIZE_X				(20.0f)
#define E_WALK_SIZE_Y				(20.0f)
#define E_WALK_SIZE_Z				(20.0f)
#define E_WALK_COLL_X				(38.5f)
#define E_WALK_COLL_Y				(38.5f)
#define E_WALK_COLL_Z				(50.0f)

//@@@@@ �S�[���̊�{��� -------------------------------------------
#define GOAL_NAME					("Goal")

//@@@@@ UI�̊�{��� -----------------------------------------------
//�X�R�A
#define UI_SCORE_NAME				("UI_SCORE")
#define UI_SCORE_SIZE_X				(200.0f)
#define UI_SCORE_SIZE_Y				(150.0f)
#define UI_SCORE_POS_X				(-500.0f)
#define UI_SCORE_POS_Y				(325.0f)

//�n�C�X�R�A
#define UI_MAX_SCORE_NAME			("UI_MAX_SCORE")
#define UI_MAX_SCORE_SIZE_X			(150.0f)
#define UI_MAX_SCORE_SIZE_Y			(125.0f)
#define UI_MAX_SCORE_POS_X			(-500.0f)
#define UI_MAX_SCORE_POS_Y			(275.0f)

//����
#define UI_NUMBER_NAME				("UI_NAMBER")
#define UI_NUMBER_SIZE_X_SCORE		(200.0f)
#define UI_NUMBER_SIZE_Y_SCORE		(150.0f)
#define UI_NUMBER_POS_X_SCORE		(-325.0f)
#define UI_NUMBER_POS_Y_SCORE		(325.0f)
//�n�C�X�R�A
#define UI_NUMBER_SIZE_X_MAX		(150.0f)
#define UI_NUMBER_SIZE_Y_MAX		(125.0f)
#define UI_NUMBER_POS_X_MAX			(-325.0f)
#define UI_NUMBER_POS_Y_MAX			(275.0f)
//�^�C��
#define UI_NUMBER_SIZE_X_TIME		(200.0f)
#define UI_NUMBER_SIZE_Y_TIME		(150.0f)
#define UI_NUNBER_POS_X_TIME		(560.0f)
#define UI_NUNBER_POS_Y_TIME		(325.0f)

//�|�[�Y
#define UI_PAUSE_NAME				("UI_PAUSE")
#define UI_PAUSE_SIZE_X				(200.0f)
#define UI_PAUSE_SIZE_Y				(150.0f)
#define UI_PAUSE_POS_X				(385.0f)
#define UI_PAUSE_POS_Y				(275.0f)

//����
#define UI_TIMA_NAME				("UI_TIME")
#define UI_TIME_SIZE_X				(200.0f)
#define UI_TIME_SIZE_Y				(150.0f)
#define UI_TIME_POS_X				(400.0f)
#define UI_TIME_POS_Y				(325.0f)		

//�v�b�V��
#define UI_PUSH_NAME				("UI_PUSH")
#define UI_PUSH_SIZE_X				(300.0f)
#define UI_PUSH_SIZE_Y				(200.0f)
#define UI_PUSH_POS_X				(0.0f)
#define UI_PUSH_POS_Y				(200.0f)

//�^�C�g��
#define UI_TITLE_NAME				("UI_TITLE")
#define UI_TITLE_SIZE_X				(300.0f)
#define UI_TITLE_SIZE_Y				(200.0f)
#define UI_TITLE_POS_X				(-450.0f)
#define UI_TITLE_POS_Y				(-300.0f)

//�ĊJ
#define UI_RESUME_NAME				("UI_RESUME")
#define UI_RESUME_SIZE_X			(300.0f)
#define UI_RESUME_SIZE_Y			(200.0f)
#define UI_RESUME_POS_X				(0.0f)
#define UI_RESUME_POS_Y				(75.0f)

//�I�v�V����
#define UI_OPTION_NAME				("UI_OPTION")

//���߂���
#define UI_BEGIN_NAME				("UI_BEGIN")
#define UI_BEGIN_SIZE_X				(300.0f)
#define UI_BEGIN_SIZE_Y				(200.0f)
#define UI_BEGIN_POS_X				(400.0f)
#define UI_BEGIN_POS_Y				(-100.0f)

//��������
#define UI_CONTINUE_NAME			("UI_CONTINUE")
#define UI_CONTINUE_SIZE_X			(300.0f)
#define UI_CONTINUE_SIZE_Y			(200.0f)
#define UI_CONTINUE_POS_X			(400.0f)
#define UI_CONTINUE_POS_Y			(-175.0f)


//�X�e�[�W�Z���N�g
#define UI_STAGE_SELECT_NAME		("UI_STAGE_SELECT")

//�I���
#define UI_END_NAME					("UI_END")
#define UI_END_SIZE_X				(300.0f)
#define UI_END_SIZE_Y				(200.0f)
#define UI_END_POS_X				(400.0f)
#define UI_END_POS_Y				(-250.0f)

//BGM
#define UI_BGM_NAME					("UI_BGM")

//SE
#define UI_SE_NAME					("UI_SE")

//���邳
#define UI_BRIGHTNESS_NAME			("UI_BRIGHTNESS")

#endif