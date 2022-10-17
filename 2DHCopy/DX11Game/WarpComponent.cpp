/**�C���N���[�h��**/
#include "WarpComponent.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "InputManager.h"
#include "TextureManager.h"

#include "Object.h"
#include "ObjInfo.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "PlayerComponent.h"
#include "sceneGame.h"
#include "Camera.h"
#include "imgui.h"

/**
* @fn		CWarp::CWarp
* @brief	�R���X�g���N�^�G���W��
*/
CWarp::CWarp() 
	:m_pTransform(nullptr),m_pCollider(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CWarp::CWarp
* @brief	�f�X�g���N�^�݂����Ńe���V�����オ��Ȃ�
*/
CWarp::~CWarp() {

}

/**
* @fn		CWarp::Start	
* @brief	�J�n����
*/
void CWarp::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
	m_pCollider = Parent->GetComponent<CCollider>();
	m_bWarpFlg = false;
}

/**
* @fn		CWarp::Update
* @brief	�X�V����
*/
void CWarp::Update() {
	//���[�v�t���O���I���ɂȂ����珈�����J�n����
	if (m_bWarpFlg) {
		//�X�e�[�W���Ƃɂǂ����[�v�����邩�͌��߂�

		//��肽������
		//��ʂ�^���Âɂ�����(�t�F�[�h�A�E�g) 
		//�I�u�W�F�N�g�̍X�V���~�߂�
		//�`��̍X�V���~�߂�
		//UI�̕\��������
		//�v���C���[�ƃJ�����̈ʒu���΂�(4580,-573.139771+770)
		//�����_���Ȃ���(�Œ�J�����ɂ���)
		//�w�i��ύX����
		//�X�V�ƕ`��̍ĊJ
		//UI�̕\�����ĊJ
		//�t�F�[�h�C��������

		switch (SceneGame::GetInstance()->GetStage()) {
		case STAGE_1:
			//�����̓t�F�[�h�A�E�g�ɕς�����
			//����͉��ɐ^�����ŕ���
			CreateBlak();

			//�S�ẴI�u�W�F�N�g�̍X�V���X�g�b�v����
			for (auto&& obj :ObjectManager::GetInstance()->GetUpdateList()) {
				if (obj->GetName() == WARP_NAME) {
					continue;
				}
				obj->StopUpdate();
			}
			//UI�`����X�g�b�v����
			for (auto&& obj:ObjectManager::GetInstance()->GetDrawList()) {
				if (obj->GetDrawOrder() == DRAW_UI|| obj->GetDrawOrder() == DRAW_MODEL) {
					obj->StopDraw();
				}
			}

			ChangeCamera();

			//��x�ݒ��ς�����t���O��߂��ē���Ȃ��悤�ɂ���
			//m_bWarpFlg;
			break;
		case STAGE_1_RE:break;
		}
	}
}

/**
* @fn		CWarp::Draw
* @brief	�`�揈��
*/
void CWarp::Draw() {

}

/**
* @fn		CWarp::OnCliisionEnter
* @brief	�����蔻��̏���
* @param	(Object*)	��������������
*/
void CWarp::OnCollisionEnter(Object* pObject) {
	
}

/**
* @fn		CWarp::SetWarp
* @brief	���[�v�������ǂ������Z�b�g����֐�
* @param	(bool)	true�Ń��[�v���J�n����
*/
void CWarp::SetWarp(bool bWarp) {
	m_bWarpFlg = bWarp;
}

/**
* @fn		CWarp::CreateBlack
* @brief	���w�i���쐬�����ŏ���
*/
void CWarp::CreateBlak() {
	Object* black = new Object("black_outoooo", UPDATE_DEBUG, DRAW_DEBUG);
	//�R���|�[�l���g�̒ǉ�
	auto trans = black->AddComponent<CTransform>();
	auto draw = black->AddComponent<CDraw2D>();
	//�I�u�W�F�N�g�̐ݒ�
	draw->SetColor(0.0f, 0.0f, 0.0f);
	draw->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//���X�g�ɒǉ�
	ObjectManager::GetInstance()->AddObject(black);
}

/**
* @fn		CWarp::ChangeCamera
* @brief	�J�����̒����_��؂��āA�J�����ƃv���C���[�̍��W��ύX����
*/
void CWarp::ChangeCamera() {
	//�v���C���[�̃I�u�W�F�N�g�̃|�C���^���擾����
	Object* obj = ObjectManager::GetInstance()->GetGameObject(PLAYER_NAME);

	obj->GetComponent<CTransform>()->Pos.x = 4580;

	//�J�����̒����_���I�t�ɂ���
	//
	CCamera::Get()->SetAxisX(nullptr);

	//���[�v���Ă���悩�ǂ����̃t���O�������Ă�����������������
	//�X�e�[�W�ɂ��ƂɃ��[�v��ɂ��邩�ǂ����̃t���O����������
	//�J�����̃\�[�X�Ń��~�b�g��ω�������

}
