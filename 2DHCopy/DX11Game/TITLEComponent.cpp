/**�C���N���[�h��**/
#include "TITLEComponent.h"
#include "ObjectManager.h"
#include "TextureManager.h"

#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "AnimMeshComponent.h"
#include "SceneTitle.h"
#include "imgui.h"

#include "ObjInfo.h"

/**
* @fn		CTITLE::CTITLE
* @brief	�R���X�g���N�^�B�B�B���O�D�~���
*/
CTITLE::CTITLE() 
	:m_pTransform(nullptr),m_pDraw2D(nullptr),m_pDrawMesh(nullptr),
	 m_StartObj(nullptr),m_EndObj(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CTITLE::~CTITLE
* @brief	�f�X�g���N�^�A�V���W�ɏ��
*/
CTITLE::~CTITLE() {
	
}

/**
* @fn		CTITLE::START
* @brief	�J�n����
*/
void CTITLE::Start() {
	//���ꐯ�𗎂Ƃ��t���O
	m_bFallStar = false;
	//�X�^�[�g�n�_�ƏI���n�_
	m_StartPos = XMFLOAT2(0.0f,0.0f);
	m_EndPos = XMFLOAT2(0.0f,0.0f);

	if (Parent->GetName() == "TITLE_Start_Pos") {
		//�J�n�n�_�ƏI���n�_�̃I�u�W�F�N�g�̍��W���擾����
		m_StartObj = Parent;

		for (auto&& Obj : ObjectManager::GetInstance()->GetUpdateList()) {
			if (Obj->GetName() == "TITLE_End_Pos") {
				m_EndObj = Obj;
			}
		}
	}

	//���ꐯ�𗎂Ƃ��^�C�~���O�̓����_���ɂ���
	m_nStarCnt = rand()% 60 * 10;

	//��x��������������t���O
	m_bStarCre = false;

	m_pTransform = Parent->GetComponent<CTransform>();
	//�v���C���[�L�����N�^�[�݂̂������̏���
	if (Parent->GetName() == "Miko") {
		m_pDrawMesh = Parent->GetComponent<CAnimMesh>();
		//�E�Ɉړ�������
		m_pTransform->Vel.x = 2.0f;
	}
	else if (Parent->GetName() == "TITLE_Start_Pos") {
		m_pTransform->Vel.x = rand() % 5;
	}
	else if (Parent->GetName() == "TITLE_End_Pos") {
		m_pTransform->Vel.x = rand() % 5;
	}else {
		m_pDraw2D = Parent->GetComponent<CDraw2D>();
	}

}

/**
* @fn		CTITLE::UPDATE
* @brief	�X�V����
* @detail	�ޏ������͏����������
*/
void CTITLE::Update(){
	//�����������`��
	if (Parent->GetName() == "Miko") {

		//��ʒ[!�Ɍ�������t�����ɑ��点��
		if (m_pTransform->Pos.x >= 450.0f) {
			//�e�N�X�`���𔽓]������č������ɗ͂�������
			m_pDrawMesh->SetVertex(false);
			m_pTransform->Vel.x = -2.0f;
		}
		if (m_pTransform->Pos.x <= -450.0f) {
			//�e�N�X�`���𔽓]�����ĉE�����ɗ͂�������
			m_pDrawMesh->SetVertex(true);
			m_pTransform->Vel.x = 2.0f;
		}

	}	
	else if (Parent->GetName() == TITLE_PURPLE_NAME)  {
		//�F�����܂Ɏ��ɕύX����
		if (rand()%10 == 5 || rand() % 10 == 4 || rand() % 10 == 3) {
			m_pDraw2D->SetColor(1.0f,0.0f,1.0f);
		}
		else {
			m_pDraw2D->SetColor(1.0f,1.0f,1.0f);
		}
	}
	else if (Parent->GetName() == "TITLE_Start_Pos" ) {
		//���E�Ɉړ������ĉ�ʒ[�Ŕ��](200�`-200)
		if (m_pTransform->Pos.x >= 200.0f) {
			m_pTransform->Vel.x = -(rand() % 5);
		}
		if (m_pTransform->Pos.x <= -200.0f) {
			m_pTransform->Vel.x = (rand() % 5);
		}
	}
	else if (Parent->GetName() == "TITLE_End_Pos") {
		//���E�Ɉړ������ĉ�ʒ[�Ŕ��](200�`-200)
		if (m_pTransform->Pos.x >= 200.0f) {
			m_pTransform->Vel.x = -(rand() % 5);
		}
		if (m_pTransform->Pos.x <= -200.0f) {
			m_pTransform->Vel.x = (rand() % 5);
		}
	}

	//��x�������������邽�ߊJ�n�n�_�ƕR�t���Ă���
	if (Parent->GetName() == "TITLE_Start_Pos") {


		//�K���Ȏ��ԂŃX�^�[�g�n�_�ƏI�����_�̃|�W�V���������߂�
		if (!m_bFallStar) {
			//�^�C�}�[�����炵������
			m_nStarCnt--;

			//�^�C�}�[��0�����������
			if (m_nStarCnt <= 0) {
				m_bFallStar = true;
				//�X�^�[�g�n�_�ƏI���n�_�̃|�W�V������ݒ肷��
				m_StartPos.x = m_StartObj->GetComponent<CTransform>()->Pos.x;
				m_StartPos.y = m_StartObj->GetComponent<CTransform>()->Pos.y;
				m_EndPos.x = m_EndObj->GetComponent<CTransform>()->Pos.x;
				m_EndPos.y = m_EndObj->GetComponent<CTransform>()->Pos.y;
			}
		}
		else {
			if (!m_bStarCre) {
				//�I�u�W�F�N�g�����
				Object* Star = new Object("Star", UPDATE_DEBUG, DRAW_DEBUG);
				//�R���|�[�l���g�̒ǉ�
				auto TransStar = Star->AddComponent<CTransform>();
				auto DrawStar = Star->AddComponent<CAnimMesh>();
				//�I�u�W�F�N�g�̐ݒ�
				DrawStar->SetColor(0.0f, 1.0f, 1.0f);
				TransStar->SetPosition(m_StartPos.x, m_StartPos.y);
				DrawStar->SetSize(10.0f, 10.0f);
				DrawStar->SetLoop(true);
				//�I�u�W�F�N�g��ǉ�
				ObjectManager::GetInstance()->AddObject(Star);
				//�t���O���I�t�ɂ��ē�x�ƍ��Ȃ�
				m_bStarCre = true;
				//���̃I�u�W�F�N�g���|�C���^�Ŏw���Ă���
				m_StarObj = Star;
			}
			else {
				//������I�u�W�F�N�g�𓮂���
				//���ꐯ�݂����ɗ��Ƃ��ď���������Ȃǂ��S�ď���
				//�G���~�[�g�Ȑ�
				m_StarObj->GetComponent<CTransform>()->SetPosition(rand() % 100, rand() % 100);
			}

		}
	}
}

/**
* @fn
* @brief
*/
void CTITLE::Draw() {

}