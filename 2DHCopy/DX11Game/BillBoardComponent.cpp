/**�C���N���[�h��**/
#include "BillboardComponent.h"
#include "TransformComponent.h"
#include "Camera.h"
#include "Light.h"

/**
* @fn		CBillboard::CBillboard
* @brief	�R���X�g���N�^�͍ӂ��Ȃ�
*/
CBillboard::CBillboard() {
	m_eUpdateOrder = COM_UPDATE_DRAW;
	m_fNormal = XMFLOAT3(0.0f, 0.0f, -1.0f);
}

/**
* @fn		CBillboard::~CBillboard
* @brief	���O�ƃf�X�g���N�^����̂́A�����Ǝv���Ă���
*/
CBillboard::~CBillboard() {

}

/**
* @fn		CBillboard::Start
* @brief	����������
*/
void CBillboard::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();

	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());

	//	���_�̍쐬
	CreateBillboardVertex();
}

/**
* @fn		CBillboard::Update
* @brief	�X�V����
*/
void CBillboard::Update() {

}

/**
* @fn		CBillboard::Draw
* @brief	�`�揈��
* @detail	�J�����Ƃ̍��W���v�Z���Č����𐮂���
*/
void CBillboard::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();
	SetBlendState(BS_NONE);
	CLight::Get()->SetDisable();
	//SetZWrite(false);				//	�������`���Z�o�b�t�@���X�V���Ȃ�(Z�`�F�b�N�͍s��)

	//	�e�N�X�`�� �}�g���N�X�X�V
	XMMATRIX mtxTex = XMMatrixScaling(1.0f / m_nSplitX, 1.0f / m_nSplitY, 1.0f);
	mtxTex = XMMatrixMultiply(mtxTex,
		XMMatrixTranslation(
			m_nAnim % m_nSplitX / (float)m_nSplitX,
			m_nAnim / m_nSplitX / (float)m_nSplitY, 0.0f));
	XMStoreFloat4x4(&m_Mesh.mtxTexture, mtxTex);

	//	�r���[�s��̉�]�����̓]�u�s���ݒ�
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();
	m_Mesh.mtxWorld._11 = mView._11 * m_pTransform->Scale.x * m_fWidth;
	m_Mesh.mtxWorld._12 = mView._21 * m_pTransform->Scale.x * m_fWidth;
	m_Mesh.mtxWorld._13 = mView._31 * m_pTransform->Scale.x * m_fWidth;
	m_Mesh.mtxWorld._14 = 0.0f;
	m_Mesh.mtxWorld._21 = mView._12 * m_pTransform->Scale.y * m_fHeight;
	m_Mesh.mtxWorld._22 = mView._22 * m_pTransform->Scale.y * m_fHeight;
	m_Mesh.mtxWorld._23 = mView._32 * m_pTransform->Scale.y * m_fHeight;
	m_Mesh.mtxWorld._24 = 0.0f;
	m_Mesh.mtxWorld._31 = mView._13;
	m_Mesh.mtxWorld._32 = mView._23;
	m_Mesh.mtxWorld._33 = mView._33;
	m_Mesh.mtxWorld._34 = 0.0f;

	//	�ʒu�𔽉f
	m_Mesh.mtxWorld._41 = m_pTransform->Pos.x;
	m_Mesh.mtxWorld._42 = m_pTransform->Pos.y;
	m_Mesh.mtxWorld._43 = m_pTransform->Pos.z;
	m_Mesh.mtxWorld._44 = 1.0f;

	DrawMesh(pDC, &m_Mesh);

	SetBlendState(BS_NONE);
	CLight::Get()->SetEnable();
}

/**
* @fn		CBillboard::OnCollisionEnter
* @brief	�Փ˔���
* @param	(Object*)	�����������̏��̓����Ă���|�C���^ 
*/
void CBillboard::OnCollisionEnter(Object* obj) {

}

/**
* @fn		CBillBoard::CreateBillboardVertex
* @brief	�r���{�[�h�����邽�߂̒��_���쐬����
*/
void CBillboard::CreateBillboardVertex() {
	// ���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());

	// �ꎞ�I�Ȓ��_�z��𐶐�
	m_Mesh.nNumVertex = 4;
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];
	VERTEX_3D* pVtx = pVertexWk;
	pVtx[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	pVtx[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	pVtx[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	pVtx[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);
	pVtx[0].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = XMFLOAT3(0.0f, 0.0f, -1.0f);
	pVtx[0].tex = XMFLOAT2(0.0f, 0.0f);
	pVtx[1].tex = XMFLOAT2(1.0f, 0.0f);
	pVtx[2].tex = XMFLOAT2(0.0f, 1.0f);
	pVtx[3].tex = XMFLOAT2(1.0f, 1.0f);
	m_Mesh.nNumIndex = 4;
	int* pIndexWk = new int[m_Mesh.nNumIndex];
	pIndexWk[0] = 0;
	pIndexWk[1] = 1;
	pIndexWk[2] = 2;
	pIndexWk[3] = 3;
	HRESULT hr = MakeMeshVertex(GetDevice(), &m_Mesh, pVertexWk, pIndexWk);
	delete[] pIndexWk;
	delete[] pVertexWk;

	if (FAILED(hr)) MessageBox(GetMainWnd(), _T("CreateBillboardVertex"), NULL, MB_OK);
}