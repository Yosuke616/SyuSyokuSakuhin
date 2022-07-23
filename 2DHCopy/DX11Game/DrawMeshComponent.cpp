/**�C���N���[�h��**/
#include "DrawMeshComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Light.h"
#include "ObjectManager.h"
#include "ObjInfo.h"

/**�萔��`**/
#define M_DIFFUSE	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER		(10.0f)
#define M_AMBIENT	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

/** 
* @fn		CDrawMesh::CDrawMesh
* @brief	�R���X�g���N�^�Ƃ�������
*/
CDrawMesh::CDrawMesh() 
	:m_pTransform(nullptr)
	,m_fWidth(100.0f),m_fHeight(100.0f)
	,m_Offset(0.0f,0.0f)
	,m_nAnim(0)
	,m_nSplitX(1),m_nSplitY(1)
	,m_fTexSizeX(1.0f),m_fTexSizeZ(1.0f){
	//�`��̏��Ԃ̐ݒ�
	this->m_eUpdateOrder = COM_UPDATE_DRAW;

	m_Mesh.primitiveType = PT_TRIANGLESTRIP;

	//���[���h�}�g���b�N�X������
	XMStoreFloat4x4(&m_Mesh.mtxWorld,XMMatrixIdentity());

	//�ʒu�E��]������
	m_Mesh.pos = XMFLOAT3(0.0f,0.0f,0.0f);
	m_Mesh.rot = XMFLOAT3(0.0f,0.0f,0.0f);
	m_fNormal =  XMFLOAT3(0.0f,-1.0f,0.0f);

	//�}�e���A���̏����ݒ�
	m_Material.Diffuse  = M_DIFFUSE;
	m_Material.Ambient  = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power    = M_POWER;
	m_Material.Emissive = M_EMISSIVE;
	m_Mesh.pMaterial = &m_Material;
}

/**
* @fn		CDrawMesh::~CDrawMesh
* @brief	�����E�E�E�f�X�g���N�^�ɕ�����
*/
CDrawMesh::~CDrawMesh() {
	//���b�V���̉��
	ReleaseMesh(&m_Mesh);
}

/**
* @fn		CDrawMesh::Start
* @brief	����������
*/
void CDrawMesh::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();

	//���_���̍쐬
	MakeVertex(
		m_fWidth,m_fHeight,
		m_fTexSizeX,m_fTexSizeZ,
		m_fNormal);

	CDrawMesh::Update();
}

/**
* @fn		CDrawMesh::Update
* @brief	�X�V����
*/
void CDrawMesh::Update() {
	m_Mesh.pos.x = m_pTransform->Pos.x + m_Offset.x;
	m_Mesh.pos.y = m_pTransform->Pos.y + m_Offset.y;
	m_Mesh.pos.z = m_pTransform->Pos.z;
	m_Mesh.rot = m_pTransform->Rotate;

	UpdateMesh(&m_Mesh);
}

/**
* @fn		CDrawMesh::Draw
* @brief	�`�揈��
*/
void CDrawMesh::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//���b�V���̕`��
	DrawMesh(pDC,&m_Mesh);
}

/**
* @fn		CDrawMesh::MakeVertex
* @brief	���_���̍쐬
* @param	(float) ���̑傫��
* @param	(float) �c�̑傫��
* @param	(float) UV�̉���(0�`1)
* @param	(float) UV�̏c��(0�`1)
* @param	(XMFLOAT3) �@���x�N�g��
* @return	������ S_OK
*/
HRESULT CDrawMesh::MakeVertex(
	float fSizeBlockX, float fSizeVlockZ,
	float fTexSizeX, float fTexSizeZ,
	XMFLOAT3 normal) {
	//�v���~�e�B�u��ʐݒ�
	m_Mesh.primitiveType = PT_LINE;

	//���_���̐ݒ�
	m_Mesh.nNumVertex = (1 + 1) * (1 + 1);

	//�C���f�b�N�X���̐ݒ�(�k�ރ|���S���p���l������)
	m_Mesh.nNumIndex = 8;

	//���_�z��쐬
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];

	//�C���f�b�N�X�z��̍쐬
	int* pIndexWk = new int[m_Mesh.nNumIndex];

	//���_�z��̒��g�𖄂߂�
	VERTEX_3D* pVtx = pVertexWk;

	//���_���W�̐ݒ�
	pVtx[0].vtx.x = m_fWidth * -0.5f;
	pVtx[1].vtx.x = m_fWidth * 0.5f;
	pVtx[2].vtx.x = m_fWidth * 0.5f;
	pVtx[3].vtx.x = m_fWidth * -0.5f;
	pVtx[0].vtx.y = m_fHeight * 0.5f;
	pVtx[1].vtx.y = m_fHeight * -0.5f;
	pVtx[2].vtx.y = m_fHeight * 0.5f;
	pVtx[3].vtx.y = m_fHeight * -0.5f;
	pVtx[0].vtx.z = 0.0f;
	pVtx[1].vtx.z = 0.0f;
	pVtx[2].vtx.z = 0.0f;
	pVtx[3].vtx.z = 0.0f;

	for (int z = 0;z < 2; ++z) {
		for (int x = 0;x < 2;++x) {
			//�@���̐ݒ�
			pVtx->nor = XMFLOAT3(normal.x,normal.y,normal.z);

			//���ˌ��̐ݒ�
			pVtx->diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//�C���f�b�N�X�̔z��̒��g�𖄂߂�
	int* pIdx = pIndexWk;
	pIdx[0] = 0;
	pIdx[1] = 2;

	pIdx[2] = 2;
	pIdx[3] = 1;

	pIdx[4] = 1;
	pIdx[5] = 3;

	pIdx[6] = 3;
	pIdx[7] = 0;

	//���_�o�b�t�@/�C���f�b�N�X�o�b�t�@����
	HRESULT hr = MakeMeshVertex(GetDevice(),&m_Mesh,pVertexWk,pIndexWk);

	//�ꎟ�z��̉��
	delete[] pVertexWk;
	delete[] pIndexWk;

	if (FAILED(hr)) {
		MessageBox(GetMainWnd(),_T("���b�V���쐬���s"),NULL,MB_OK);
	}

	return hr;
}

/**�Z�b�^�[**/
/**
* @fn		CDrawMesh::SetSize
* @brief	���b�V���T�C�Y�̐ݒ�
* @param	(float)	����
* @param	(float) ����
*/
void CDrawMesh::SetSize(float width,float height) {
	m_fWidth = width;
	m_fHeight = height;
}

/**
* @fn		CDrawMesh::SetTexture
* @brief	�e�N�X�`���̐ݒ�
* @param	(ID3D11ShaderResourceView*)	�e�N�X�`���̏��̓������|�C���^
*/
void CDrawMesh::SetTexture(ID3D11ShaderResourceView* pTexture) {
	if (pTexture) {
		//�Q�ƃJ�E���^+1
		pTexture->AddRef();
	}
	m_Mesh.pTexture = pTexture;
	if (m_Mesh.pTexture) {
		XMStoreFloat4x4(&m_Mesh.mtxTexture,XMMatrixIdentity());
	}
}

/**
* @fn		CDrawMesh::SetDiffuse
* @brief	���ˌ��̐ݒ�
* @param	(XMFLOAT4)	���ˌ��̋����̏��
*/
void CDrawMesh::SetDiffuse(XMFLOAT4 Diffuse) {
	m_Material.Diffuse = Diffuse;
}

/**
* @fn		CDrawMesh::SetAmbient
* @brief	�����̐ݒ�
* @param	(XMFLOAT4)	�����̏��
*/
void CDrawMesh::SetAmbient(XMFLOAT4 Ambient) {
	m_Material.Ambient = Ambient;
}

/**
* @fn		CDrawMesh::SetSpecular
* @brief	���ʔ��˂̐ݒ�
* @param	(XMFLOAT4)	���ʔ��˂͂ǂ̈ʂ��邩�̏��
*/
void CDrawMesh::SetSpecular(XMFLOAT4 Specular) {
	m_Material.Specular = Specular;
}

/**
* @fn		CDrawMesh::SetEmissive
* @brief	���Ȕ����̐ݒ�
* @param	(XMFLOAT4)	�������ǂꂭ�炢�����Ă��邩�̏��
*/
void CDrawMesh::SetEmissive(XMFLOAT4 Emissive) {
	m_Material.Emissive = Emissive;
}

/**
* @fn		CDrawMesh::SetPower
* @brief	����̐ݒ�
* @param	(float)	�ǂ̈ʌ�����o�����̐��l
*/
void CDrawMesh::SetPower(float power) {
	m_Material.Power = power;
}

/**
* @fn		CDrawMesh::SetNormal
* @brief	�@���x�N�g���̐ݒ�
* @param	(float)	x�����̖@���̐��l
* @param	(float)	y�����̖@���̐��l
* @param	(float)	z�����̖@���̐��l
*/
void CDrawMesh::SetNormal(float fx,float fy,float fz) {
	m_fNormal.x = fx;
	m_fNormal.y = fy;
	m_fNormal.z = fz;
}

/**
* @fn		CDrawMesh::SetUVsize
* @brief	�e�N�X�`�����W�̐ݒ�
* @param	(XMFLOAT2)	�ǂ̂�����ɍ��W�����邩�̐ݒ�
*/
void CDrawMesh::SetUVsize(XMFLOAT2 uv) {
	m_fTexSizeX = uv.x;
	m_fTexSizeZ = uv.y;
}

/** 
* @fn		CDrawMesh::SetAnimFrame
* @brief	�A�j���[�V�����ԍ��̐ݒ�
* @param	(int)	�A�j���[�V�����ԍ��̐��l
*/
void CDrawMesh::SetAnimFrame(int anim) {
	m_nAnim = anim;
}

/**
* @fn		CDrawMesh::SetAnimSplit
* @brief	�������̐ݒ�
* @param	(int)	�������̕�����
* @param	(int)	�c�����̕�����
*/
void CDrawMesh::SetAnimSplit(int nx,int ny) {
	m_nSplitX = nx;
	m_nSplitY = ny;
}

/**
* @fn		CDrawMesh::SetOffSet
* @brief	�I�t�Z�b�g�̐ݒ�
* @param	(XMFLOAT2)	�ǂ̈ʈӐ}�I�ɂ��炷��
*/
void CDrawMesh::SetOffSet(XMFLOAT2 offset) {
	m_Offset = offset;
}

/**
* @fn		CDrawMesh::SetColor
* @brief	�F��ς���
* @param	(float)	rgb��r�̐��l���󂯎��
* @param	(float)	rgb��g�̐��l���󂯎��
* @param	(float)	rgb��b�̐��l���󂯎��
*/
void CDrawMesh::SetColor(float r,float g,float b) {
	m_Mesh.pMaterial->Diffuse = XMFLOAT4(r,g,b,1.0f);
}

/**�Q�b�^�[**/
/**
* @fn		CDrawMesh::GetMesh
* @brief	���b�V�����̎擾
* @return	(MESH)	���b�V�����
*/
MESH* CDrawMesh::GetMesh() {
	return &m_Mesh;
}