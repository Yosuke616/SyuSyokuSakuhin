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
	:m_pTransform(nullptr),m_pDraw2D(nullptr),m_pDrawMesh(nullptr){
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
	m_pTransform = Parent->GetComponent<CTransform>();
	//�v���C���[�L�����N�^�[�݂̂������̏���
	if (Parent->GetName() == "Miko") {
		m_pDrawMesh = Parent->GetComponent<CAnimMesh>();
		//�E�Ɉړ�������
		m_pTransform->Vel.x = 2.0f;
	}
	else {
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
}

/**
* @fn
* @brief
*/
void CTITLE::Draw() {

}