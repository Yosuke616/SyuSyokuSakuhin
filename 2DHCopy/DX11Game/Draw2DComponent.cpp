//�C���N���[�h��
#include "Draw2DComponent.h"
#include "TransformComponent.h"
#include "polygon.h"
#include "ObjectManager.h"

/**
* @fn		CDraw2D::CDraw2D
* @brief	�R���X�g���N�^���n�܂�Ƃ�������
*/
CDraw2D::CDraw2D() 
	:m_pTexture(0),m_fAngle(0.0f),
	 m_nAnimNum(0),m_nAnimSplitX(1),m_nAnimSplitY(1),
	 m_BlendState(BS_ALPHABLEND){

	m_fPolygonSize = XMFLOAT2(0.0f,0.0f);
	m_fTexFrameSize = XMFLOAT2(1.0f,1.0f);
	m_fVertevColor = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);
	m_Scroll = XMFLOAT2(0.0f,0.0f);
	m_eUpdateOrder = (COM_UPDATE_DRAW);
}

/** 
* @fn		CDraw2D::~CDraw2d
* @brief	�Ԏ����Ȃ��@�����̃f�X�g���N�^�̂悤���@
*/
CDraw2D::~CDraw2D() {

}

/**
* @fn		CDraw2D::Start
* @brief	�������̏���
*/
void CDraw2D::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();
}

/**
* @fn		CDraw2D::Update
* @brief	�X�V��������������
*/
void CDraw2D::Update() {

}

/**
* @fn		CDraw2D::SetTexture
* @brief	�e�N�X�`�����Z�b�g����
* @param	(ID3D11ShaderResourceView*)	�e�N�X�`���̃|�C���^
*/
void CDraw2D::SetTexture(ID3D11ShaderResourceView* pTex) {
	m_pTexture = pTex;
}

/**
* @fn		CDraw2D::SetAnimNum
* @brief	�A�j���[�V�����ԍ����Z�b�g����
* @param	(int)	���݂̃A�j���[�V�����̔ԍ�
*/
void CDraw2D::SetAnimNum(int anim) {
	m_nAnimNum = anim;
}

/**
* @fn		CDraw2D::SetUVsize
* @brief	�e�N�X�`���̃T�C�Y��ݒ肷��
* @param	(float)		�e�N�X�`���̉���
* @param	(float)		�e�N�X�`���̗���
*/
void CDraw2D::SetUVsize(float UVSizeX,float UVSizeY) {
	m_fTexFrameSize.x = UVSizeX;
	m_fTexFrameSize.y = UVSizeY;
}

/**
* @fn		CDraw2D::SetU
* @brief	UV��U�̐ݒ�
* @param	(float)		U�ł���
*/
void CDraw2D::SetU(float u) {
	m_fTexFrameSize.x = u;
}

/**
* @fn		CDraw2D::SetV
* @brief	UV��V�̐ݒ�
* @param	(float)		����V������
*/
void CDraw2D::SetV(float v) {
	m_fTexFrameSize.y = v;
}

/**
* @fn		CDraw2D::SetSize
* @brief	�|���S���̑傫����ݒ肷��
* @param	(float)	�|���S���̉��������߂�
* @param	(float)	�|���S���̗��������߂�
*/
void CDraw2D::SetSize(float SizeX,float SizeY) {
	m_fPolygonSize.x = SizeX;
	m_fPolygonSize.y = SizeY;
}

/**
* @fn		CDraw2D::SetColor
* @brief	�F�̐ݒ�
* @param	(float)	RGB�̐ԐF�ł���
* @param	(float)	RGB�̗΂̂Ђ�
* @param	(float) RGB�̐󋳎�
*/
void CDraw2D::SetColor(float red,float green,float blue){
	m_fVertevColor.x = red;
	m_fVertevColor.y = green;
	m_fVertevColor.z = blue;
}

/**
* @fn		CDraw2D::SetAlpha
* @brief	�����x��ݒ肷��
* @param	(float)	�����x�������ĉ��������悤
*/
void CDraw2D::SetAlpha(float alpha) {
	m_fVertevColor.w = alpha;
}

/**
* @fn		CDraw2D::SetAngle
* @brief	�p�x�̐ݒ�
* @param	(float)	�ǂ̊p�x�ɂ��邩�̐��l
*/
void CDraw2D::SetAngle(float angle) {
	m_fAngle = angle;
}

/**
* @fn		CDraw2D::SetAnimSplit
* @brief	�A�j���[�V�����̕�������ݒ�
* @param	(int)	���̕������̐��l
* @param	(int)	�c�̕������̐��l
*/
void CDraw2D::SetAnimSplit(int x,int y) {
	m_nAnimSplitX = x;
	m_nAnimSplitY = y;
	m_fTexFrameSize.x = 1.0f / x;
	m_fTexFrameSize.y = 1.0f / y;
}

/**
* @fn		CDraw2D::SetScroll
* @brief	�X�N���[���ʂ̐ݒ�
* @param	(XMFLOAT2)	�ǂ̈ʓ������΂������̐ݒ�
*/
void CDraw2D::SetScroll(XMFLOAT2 scroll) {
	m_Scroll.x = scroll.x;
	m_Scroll.y = scroll.y;
}

/**
* @fn		CDraw2D::SetBlend
* @brief	�u�����h�X�e�[�g�̐ݒ�
* @param	(EBlendState)	�u�����h�X�e�[�g�̏��
*/
void CDraw2D::SetBlend(EBlendState state) {
	m_BlendState = state;
}

/**
* @fn		CDraw2D::GetAnimSplitX
* @brief	�A�j���[�V�����������̉��������擾����
* @return	(int) �ǂ̈ʕ�����Ă��邩��Ԃ�
*/
int CDraw2D::GetAnimSplitX() {
	return m_nAnimSplitX;
}

/**
* @fn		CDraw2D::Draw
* @brief	�`��֐�	
* @detail	
*/
void CDraw2D::Draw() {
	//�f�o�C�X�R���e�L�X�g�̎擾
	ID3D11DeviceContext* pDC = GetDeviceContext();
	//Z�o�b�t�@���X�V���Ȃ�
	SetZBuffer(false);
	//�A���t�@�u�����h�L��
	SetBlendState(m_BlendState);

	//�e�N�X�`���̃Z�b�g
	SetPolygonTexture(m_pTexture);
	//�|���S���̍��W���Z�b�g
	SetPolygonPos(m_pTransform->Pos.x,m_pTransform->Pos.y);
	//�|���S���̑傫�����Z�b�g
	SetPolygonSize(m_fPolygonSize.x,m_fPolygonSize.y);
	//�e�N�X�`�����蓖��
	SetPolygonFrameSize(m_fTexFrameSize.x,m_fTexFrameSize.y);
	//�A�j���[�V�����ԍ��ݒ�
	SetPolygonUV(
		(m_nAnimNum % m_nAnimSplitX) / (float)m_nAnimSplitX + m_Scroll.x,
		(m_nAnimNum / m_nAnimSplitX) / (float)m_nAnimSplitY + m_Scroll.y);
	//�|���S���̐F��ݒ�
	SetPolygonColor(m_fVertevColor.x,m_fVertevColor.y,m_fVertevColor.z);
	//�|���S���̓����x��ݒ�
	SetPolygonAlpha(m_fVertevColor.w);
	//�|���S���̌X���̐ݒ�
	SetPolygonAngle(m_fAngle);

	//�`�悵�Ă����������ł����܂����
	DrawPolygon(pDC);

	//�ݒ�����ɖ߂�
	SetPolygonColor(1,1,1);
	SetPolygonAlpha(1.0f);
	SetPolygonAngle(0.0f);
	SetPolygonFrameSize(1.0f,1.0f);
	SetPolygonUV(0.0f,0.0f);
}
