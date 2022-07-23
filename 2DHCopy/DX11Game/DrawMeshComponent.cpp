/**インクルード部**/
#include "DrawMeshComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "Light.h"
#include "ObjectManager.h"
#include "ObjInfo.h"

/**定数定義**/
#define M_DIFFUSE	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_SPECULAR	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)
#define M_POWER		(10.0f)
#define M_AMBIENT	XMFLOAT4(1.0f,1.0f,1.0f,1.0f)
#define M_EMISSIVE	XMFLOAT4(0.0f,0.0f,0.0f,1.0f)

/** 
* @fn		CDrawMesh::CDrawMesh
* @brief	コンストラクタということ
*/
CDrawMesh::CDrawMesh() 
	:m_pTransform(nullptr)
	,m_fWidth(100.0f),m_fHeight(100.0f)
	,m_Offset(0.0f,0.0f)
	,m_nAnim(0)
	,m_nSplitX(1),m_nSplitY(1)
	,m_fTexSizeX(1.0f),m_fTexSizeZ(1.0f){
	//描画の順番の設定
	this->m_eUpdateOrder = COM_UPDATE_DRAW;

	m_Mesh.primitiveType = PT_TRIANGLESTRIP;

	//ワールドマトリックス初期化
	XMStoreFloat4x4(&m_Mesh.mtxWorld,XMMatrixIdentity());

	//位置・回転初期化
	m_Mesh.pos = XMFLOAT3(0.0f,0.0f,0.0f);
	m_Mesh.rot = XMFLOAT3(0.0f,0.0f,0.0f);
	m_fNormal =  XMFLOAT3(0.0f,-1.0f,0.0f);

	//マテリアルの初期設定
	m_Material.Diffuse  = M_DIFFUSE;
	m_Material.Ambient  = M_AMBIENT;
	m_Material.Specular = M_SPECULAR;
	m_Material.Power    = M_POWER;
	m_Material.Emissive = M_EMISSIVE;
	m_Mesh.pMaterial = &m_Material;
}

/**
* @fn		CDrawMesh::~CDrawMesh
* @brief	くっ・・・デストラクタに負けた
*/
CDrawMesh::~CDrawMesh() {
	//メッシュの解放
	ReleaseMesh(&m_Mesh);
}

/**
* @fn		CDrawMesh::Start
* @brief	初期化処理
*/
void CDrawMesh::Start() {
	m_pTransform = Parent->GetComponent<CTransform>();

	//頂点情報の作成
	MakeVertex(
		m_fWidth,m_fHeight,
		m_fTexSizeX,m_fTexSizeZ,
		m_fNormal);

	CDrawMesh::Update();
}

/**
* @fn		CDrawMesh::Update
* @brief	更新処理
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
* @brief	描画処理
*/
void CDrawMesh::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//メッシュの描画
	DrawMesh(pDC,&m_Mesh);
}

/**
* @fn		CDrawMesh::MakeVertex
* @brief	頂点情報の作成
* @param	(float) 横の大きさ
* @param	(float) 縦の大きさ
* @param	(float) UVの横幅(0～1)
* @param	(float) UVの縦幅(0～1)
* @param	(XMFLOAT3) 法線ベクトル
* @return	成功情 S_OK
*/
HRESULT CDrawMesh::MakeVertex(
	float fSizeBlockX, float fSizeVlockZ,
	float fTexSizeX, float fTexSizeZ,
	XMFLOAT3 normal) {
	//プリミティブ種別設定
	m_Mesh.primitiveType = PT_LINE;

	//頂点数の設定
	m_Mesh.nNumVertex = (1 + 1) * (1 + 1);

	//インデックス数の設定(縮退ポリゴン用を考慮する)
	m_Mesh.nNumIndex = 8;

	//頂点配列作成
	VERTEX_3D* pVertexWk = new VERTEX_3D[m_Mesh.nNumVertex];

	//インデックス配列の作成
	int* pIndexWk = new int[m_Mesh.nNumIndex];

	//頂点配列の中身を埋める
	VERTEX_3D* pVtx = pVertexWk;

	//頂点座標の設定
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
			//法線の設定
			pVtx->nor = XMFLOAT3(normal.x,normal.y,normal.z);

			//反射光の設定
			pVtx->diffuse = XMFLOAT4(1.0f,1.0f,1.0f,1.0f);

			//テクスチャ座標の設定
			pVtx->tex.x = fTexSizeX * x;
			pVtx->tex.y = fTexSizeZ * z;
			++pVtx;
		}
	}

	//インデックスの配列の中身を埋める
	int* pIdx = pIndexWk;
	pIdx[0] = 0;
	pIdx[1] = 2;

	pIdx[2] = 2;
	pIdx[3] = 1;

	pIdx[4] = 1;
	pIdx[5] = 3;

	pIdx[6] = 3;
	pIdx[7] = 0;

	//頂点バッファ/インデックスバッファ生成
	HRESULT hr = MakeMeshVertex(GetDevice(),&m_Mesh,pVertexWk,pIndexWk);

	//一次配列の解放
	delete[] pVertexWk;
	delete[] pIndexWk;

	if (FAILED(hr)) {
		MessageBox(GetMainWnd(),_T("メッシュ作成失敗"),NULL,MB_OK);
	}

	return hr;
}

/**セッター**/
/**
* @fn		CDrawMesh::SetSize
* @brief	メッシュサイズの設定
* @param	(float)	横幅
* @param	(float) 立幅
*/
void CDrawMesh::SetSize(float width,float height) {
	m_fWidth = width;
	m_fHeight = height;
}

/**
* @fn		CDrawMesh::SetTexture
* @brief	テクスチャの設定
* @param	(ID3D11ShaderResourceView*)	テクスチャの情報の入ったポインタ
*/
void CDrawMesh::SetTexture(ID3D11ShaderResourceView* pTexture) {
	if (pTexture) {
		//参照カウンタ+1
		pTexture->AddRef();
	}
	m_Mesh.pTexture = pTexture;
	if (m_Mesh.pTexture) {
		XMStoreFloat4x4(&m_Mesh.mtxTexture,XMMatrixIdentity());
	}
}

/**
* @fn		CDrawMesh::SetDiffuse
* @brief	反射光の設定
* @param	(XMFLOAT4)	反射光の強さの情報
*/
void CDrawMesh::SetDiffuse(XMFLOAT4 Diffuse) {
	m_Material.Diffuse = Diffuse;
}

/**
* @fn		CDrawMesh::SetAmbient
* @brief	環境光の設定
* @param	(XMFLOAT4)	環境光の情報
*/
void CDrawMesh::SetAmbient(XMFLOAT4 Ambient) {
	m_Material.Ambient = Ambient;
}

/**
* @fn		CDrawMesh::SetSpecular
* @brief	鏡面反射の設定
* @param	(XMFLOAT4)	鏡面反射はどの位するかの情報
*/
void CDrawMesh::SetSpecular(XMFLOAT4 Specular) {
	m_Material.Specular = Specular;
}

/**
* @fn		CDrawMesh::SetEmissive
* @brief	自己発光の設定
* @param	(XMFLOAT4)	自分がどれくらい光っているかの情報
*/
void CDrawMesh::SetEmissive(XMFLOAT4 Emissive) {
	m_Material.Emissive = Emissive;
}

/**
* @fn		CDrawMesh::SetPower
* @brief	光沢の設定
* @param	(float)	どの位光沢を出すかの数値
*/
void CDrawMesh::SetPower(float power) {
	m_Material.Power = power;
}

/**
* @fn		CDrawMesh::SetNormal
* @brief	法線ベクトルの設定
* @param	(float)	x方向の法線の数値
* @param	(float)	y方向の法線の数値
* @param	(float)	z方向の法線の数値
*/
void CDrawMesh::SetNormal(float fx,float fy,float fz) {
	m_fNormal.x = fx;
	m_fNormal.y = fy;
	m_fNormal.z = fz;
}

/**
* @fn		CDrawMesh::SetUVsize
* @brief	テクスチャ座標の設定
* @param	(XMFLOAT2)	どのあたりに座標があるかの設定
*/
void CDrawMesh::SetUVsize(XMFLOAT2 uv) {
	m_fTexSizeX = uv.x;
	m_fTexSizeZ = uv.y;
}

/** 
* @fn		CDrawMesh::SetAnimFrame
* @brief	アニメーション番号の設定
* @param	(int)	アニメーション番号の数値
*/
void CDrawMesh::SetAnimFrame(int anim) {
	m_nAnim = anim;
}

/**
* @fn		CDrawMesh::SetAnimSplit
* @brief	分割数の設定
* @param	(int)	横方向の分割数
* @param	(int)	縦方向の分割数
*/
void CDrawMesh::SetAnimSplit(int nx,int ny) {
	m_nSplitX = nx;
	m_nSplitY = ny;
}

/**
* @fn		CDrawMesh::SetOffSet
* @brief	オフセットの設定
* @param	(XMFLOAT2)	どの位意図的にずらすか
*/
void CDrawMesh::SetOffSet(XMFLOAT2 offset) {
	m_Offset = offset;
}

/**
* @fn		CDrawMesh::SetColor
* @brief	色を変える
* @param	(float)	rgbのrの数値を受け取る
* @param	(float)	rgbのgの数値を受け取る
* @param	(float)	rgbのbの数値を受け取る
*/
void CDrawMesh::SetColor(float r,float g,float b) {
	m_Mesh.pMaterial->Diffuse = XMFLOAT4(r,g,b,1.0f);
}

/**ゲッター**/
/**
* @fn		CDrawMesh::GetMesh
* @brief	メッシュ情報の取得
* @return	(MESH)	メッシュ情報
*/
MESH* CDrawMesh::GetMesh() {
	return &m_Mesh;
}