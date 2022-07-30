/**�C���N���[�h��**/
#include "AnimMeshComponent.h"
#include "TransformComponent.h"
#include "Light.h"
#include "Camera.h"
#include "ObjectManager.h"

/**
* @fn		CAnimMesh::CAnimMesh
* @brief	�R���X�g���N�^�E�E�E�����Ă����̂��E�E�E
*/
CAnimMesh::CAnimMesh() 
	:m_nAnimCount(10)
	,m_nFrame(10)
	,m_bLoop(false){

}

/**
* @fn		CAnimMesh::~CAnimMesh
* @brief	�ʂɃf�X�g���N�^���Ă��\���̂��낤�H
*/
CAnimMesh::~CAnimMesh() {

}

/**
* @fn		CAnimMesh::CreateAnimVertex
* @brief	���_���̍쐬
*/
void CAnimMesh::CreateAnimVertex() {
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

	if (FAILED(hr)) MessageBox(GetMainWnd(), _T("CreateEffectVertex"), NULL, MB_OK);
}

/**
* @fn		CAnimMesh::Start
* @brief	����������
*/
void CAnimMesh::Start() {
	m_nAnimCount = m_nFrame;
	m_pTransform = Parent->GetComponent<CTransform>();

	//���_���̍쐬
	CreateAnimVertex();
}

/** 
* @fn		CAnimMesh::Update
* @brief	�X�V����
*/
void CAnimMesh::Update() {
	//�J�E���^�̍X�V
	--m_nAnimCount;
	if (m_nAnimCount <= 0) {
		//�A�j���[�V�����؂�ւ�
		++m_nAnim;

		//�Ō�̃A�j���[�V�����̏ꍇ
		if (m_nAnim >= m_nSplitX * m_nSplitY) {
			if (m_bLoop) {
				//�A�j���[�V�����̃��Z�b�g
				m_nAnim = 0;
			}
			else {
				//�I�u�W�F�N�g�̍폜
				Parent->Delete();
			}
		}
		m_nAnimCount = m_nFrame;
	}
}

/** 
* @fn		CAnimMesh::Draw
* @brief	�`�悷�邽�߂̃t���O
*/
void CAnimMesh::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//	��������
	SetBlendState(BS_ALPHABLEND);	//	���u�����f�B���O�L��

	//	�e�N�X�`�� �}�g���N�X�X�V
	XMMATRIX mtxTex = XMMatrixScaling(1.0f / m_nSplitX, 1.0f / m_nSplitY, 1.0f);
	mtxTex = XMMatrixMultiply(mtxTex,
		XMMatrixTranslation(
			m_nAnim % m_nSplitX / (float)m_nSplitX,
			m_nAnim / m_nSplitX / (float)m_nSplitY, 0.0f));
	XMStoreFloat4x4(&m_Mesh.mtxTexture, mtxTex);

	//	�r���[�s��̉�]�����̓]�u�s���ݒ�
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();		//	�r���[�}�g���b�N�X�̎擾
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

	//	��]�̔��f
	XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		XMConvertToRadians(m_pTransform->Rotate.y),
		XMConvertToRadians(m_pTransform->Rotate.z));
	XMMATRIX MeshMatrix = DirectX::XMLoadFloat4x4(&(m_Mesh.mtxWorld));	//	XMFLOAT4x4��XMMATRIX
	MeshMatrix = XMMatrixMultiply(MeshMatrix, mtxRot);					//	MATRIX���m�Ōv�Z
	DirectX::XMStoreFloat4x4(&m_Mesh.mtxWorld, MeshMatrix);				//	XMMATRIX��XMFLOAT4x4

	//	�ʒu�𔽉f
	m_Mesh.mtxWorld._41 = m_pTransform->Pos.x;
	m_Mesh.mtxWorld._42 = m_pTransform->Pos.y;
	m_Mesh.mtxWorld._43 = m_pTransform->Pos.z;
	m_Mesh.mtxWorld._44 = 1.0f;

	//	�`��
	DrawMesh(pDC, &m_Mesh);

	SetBlendState(BS_ALPHABLEND);	//	���u�����f�B���OON
	CLight::Get()->SetEnable();		//	���C�gON
}

/**�Z�b�^�[**/
/**
* @fn		CAnimMesh::SetSwapFrame
* @brief	�A�j���[�V�����J�E���^�̐ݒ�
* @param	(int)	�ǂ̈ʂ̃J�E���g����n�߂邩
*/
void CAnimMesh::SetSwapFrame(int nCount) {
	m_nFrame = nCount;
}

/**
* @fn		CAnimMesh::SetLoop
* @brief	���[�v���邩�ǂ����̐ݒ�
* @param	(bool)	���[�v����̂������Ȃ��̂����ǂ����Ȃ񂾂�
*/
void CAnimMesh::SetLoop(bool loop) {
	m_bLoop = loop;
}

/**
* @fn		CAnimMesh::StartAnim
* @brief	�A�j���[�V�����`��J�n
* @param	(ID3D11ShaderResourceView*)	�e�N�X�`�����
* @param	(XMFLOAT3)					�I�u�W�F�N�g�̍��W
* @param	(XMFLOAT2)					�I�u�W�F�N�g�̕`��T�C�Y
* @param	(int)						�e�N�X�`���̕�����X
* @param	(int)						�e�N�X�`���̕�����Y
* @param	(int)						�e�N�X�`���̐ؑփt���[����
* @param	(XMFLOAT3)					�F	
*/
void CAnimMesh::StartAnim(ID3D11ShaderResourceView* pTex,
	XMFLOAT3 pos,XMFLOAT2 size,int split_x,int split_y, bool loop,
	int swap_frame,XMFLOAT3 color) {
	//�I�u�W�F�N�g�쐬
	Object* pAnim = new Object("Anim",UPDATE_EFFECT,DRAW_EFFECT);

	//�R���|�[�l���g�ǉ�
	auto trans = pAnim->AddComponent<CTransform>();
	auto draw = pAnim->AddComponent<CAnimMesh>();

	//object�ݒ�
	trans->Pos = pos;
	draw->SetSize(size.x,size.y);
	draw->SetColor(color.x,color.y,color.z);
	draw->SetLoop(loop);
	draw->SetTexture(pTex);
	draw->SetAnimSplit(split_x,split_y);
	draw->SetSwapFrame(swap_frame);

	//�}�l�[�W���[�ɒǉ�
	ObjectManager::GetInstance()->AddObject(pAnim);
}

