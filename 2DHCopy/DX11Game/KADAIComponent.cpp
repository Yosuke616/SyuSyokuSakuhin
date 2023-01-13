/**�C���N���[�h��**/
#include "KADAIComponent.h"
#include "ObjectManager.h"

#include "TransformComponent.h"
#include "DrawMeshComponent.h"
#include "AnimMeshComponent.h"
#include "ColliderComponent.h"
#include "SceneTitle.h"
#include "SeeCollComponent.h"
#include "imgui.h"

/**��ʐ���`**/

/**
* @fn		CKADAI::CKADAI
* @brief	����ȃR���X�g���N�^�ɈӖ��Ȃ�ĂȂ�
* @detail	�F�X����������[��
*/
CKADAI::CKADAI()
	:m_pTrans(nullptr),m_pDraw(nullptr),m_pColl(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_1;
}

/**
* @fn		CKADAI::~CKADAI
* @brief	�f�X�g���N�^���폜����
* @detail	�I���
*/
CKADAI::~CKADAI() {
	
}

/**
* @fn		CKADAI::Start
* @brief	���������Ė�ڂł���
*/
void CKADAI::Start() {
	m_pTrans = Parent->GetComponent<CTransform>();
	m_pDraw = Parent->GetComponent<CAnimMesh>();
	m_pColl = Parent->GetComponent<CCollider>();


	//�����_���ɏ��������߂�
	m_pTrans->SetVelocity(rand()%3,rand()%3,0.0f);

}

/**
* @fn		CKADAI::Update
* @brief	�X�V����
* @detail   ���˂����ē�������[�[��
*/
void CKADAI::Update() {
	//��ʊO�ɍs�����Ƃ����甽�˂�����
	if (m_pTrans->Pos.x >= (SCREEN_WIDTH/4)) {
		m_pTrans->Vel.x *= -1;
	}
	if (m_pTrans->Pos.x <= -(SCREEN_WIDTH/4)) {
		m_pTrans->Vel.x *= -1;
	}
	if (m_pTrans->Pos.y >= (SCREEN_HEIGHT/4)) {
		m_pTrans->Vel.y *= -1;
	}
	if (m_pTrans->Pos.y <= -(SCREEN_HEIGHT/4)) {
		m_pTrans->Vel.y *= -1;
	}
}

/**
* @fn		CKADAI::Draw
* @brief	�`�悳���Ă��肢
*/
void CKADAI::Draw() {

#ifdef _DEBUG

	using namespace ImGui;

	SetNextWindowSize(ImVec2(120, 160), ImGuiCond_FirstUseEver);
	SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver, ImVec2(0, 0));
	Begin("BALL");
	Text("Pos	  : %3.0f %3.0f %3.0f", m_pTrans->Pos.x, m_pTrans->Pos.y, m_pTrans->Pos.z);
	Text("Vel	  : %.0f %.0f", m_pTrans->Vel.x, m_pTrans->Vel.y);
	End();
#endif
}

/**
* @fn		CKADAI::OnCollisionEnter
* @brief	�I�u�W�F�N�g���m�Ԃ������甽�˂������`��
* @param	(Object*)	������̃I�u�W�F�N�g�ƂԂ������甽�˂���	
*/
void CKADAI::OnCollisionEnter(Object* pObject){
	if (pObject->GetName() == "TITLE_BALL") {
		
	}
}
