/**インクルード部**/
#include "AnimMeshComponent.h"
#include "TransformComponent.h"
#include "Light.h"
#include "Camera.h"
#include "ObjectManager.h"

/**
* @fn		CAnimMesh::CAnimMesh
* @brief	コンストラクタ・・・生きていたのか・・・
*/
CAnimMesh::CAnimMesh() 
	:m_nAnimCount(10)
	,m_nFrame(10)
	,m_bLoop(false){

}

/**
* @fn		CAnimMesh::~CAnimMesh
* @brief	別にデストラクタしても構わんのだろう？
*/
CAnimMesh::~CAnimMesh() {

}

/**
* @fn		CAnimMesh::CreateAnimVertex
* @brief	頂点情報の作成
*/
void CAnimMesh::CreateAnimVertex() {
	// ワールドマトリックス初期化
	XMStoreFloat4x4(&m_Mesh.mtxWorld, XMMatrixIdentity());

	// 一時的な頂点配列を生成
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
* @brief	初期化処理
*/
void CAnimMesh::Start() {
	m_nAnimCount = m_nFrame;
	m_pTransform = Parent->GetComponent<CTransform>();

	//頂点情報の作成
	CreateAnimVertex();
}

/** 
* @fn		CAnimMesh::Update
* @brief	更新処理
*/
void CAnimMesh::Update() {
	//カウンタの更新
	--m_nAnimCount;
	if (m_nAnimCount <= 0) {
		//アニメーション切り替え
		++m_nAnim;

		//最後のアニメーションの場合
		if (m_nAnim >= m_nSplitX * m_nSplitY) {
			if (m_bLoop) {
				//アニメーションのリセット
				m_nAnim = 0;
			}
			else {
				//オブジェクトの削除
				Parent->Delete();
			}
		}
		m_nAnimCount = m_nFrame;
	}
}

/** 
* @fn		CAnimMesh::Draw
* @brief	描画するためのフラグ
*/
void CAnimMesh::Draw() {
	ID3D11DeviceContext* pDC = GetDeviceContext();

	CLight::Get()->SetDisable();	//	光源無効
	SetBlendState(BS_ALPHABLEND);	//	αブレンディング有効

	//	テクスチャ マトリクス更新
	XMMATRIX mtxTex = XMMatrixScaling(1.0f / m_nSplitX, 1.0f / m_nSplitY, 1.0f);
	mtxTex = XMMatrixMultiply(mtxTex,
		XMMatrixTranslation(
			m_nAnim % m_nSplitX / (float)m_nSplitX,
			m_nAnim / m_nSplitX / (float)m_nSplitY, 0.0f));
	XMStoreFloat4x4(&m_Mesh.mtxTexture, mtxTex);

	//	ビュー行列の回転成分の転置行列を設定
	XMFLOAT4X4& mView = CCamera::Get()->GetViewMatrix();		//	ビューマトリックスの取得
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

	//	回転の反映
	XMMATRIX mtxRot = XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		XMConvertToRadians(m_pTransform->Rotate.y),
		XMConvertToRadians(m_pTransform->Rotate.z));
	XMMATRIX MeshMatrix = DirectX::XMLoadFloat4x4(&(m_Mesh.mtxWorld));	//	XMFLOAT4x4→XMMATRIX
	MeshMatrix = XMMatrixMultiply(MeshMatrix, mtxRot);					//	MATRIX同士で計算
	DirectX::XMStoreFloat4x4(&m_Mesh.mtxWorld, MeshMatrix);				//	XMMATRIX→XMFLOAT4x4

	//	位置を反映
	m_Mesh.mtxWorld._41 = m_pTransform->Pos.x;
	m_Mesh.mtxWorld._42 = m_pTransform->Pos.y;
	m_Mesh.mtxWorld._43 = m_pTransform->Pos.z;
	m_Mesh.mtxWorld._44 = 1.0f;

	//	描画
	DrawMesh(pDC, &m_Mesh);

	SetBlendState(BS_ALPHABLEND);	//	αブレンディングON
	CLight::Get()->SetEnable();		//	ライトON
}

/**セッター**/
/**
* @fn		CAnimMesh::SetSwapFrame
* @brief	アニメーションカウンタの設定
* @param	(int)	どの位のカウントから始めるか
*/
void CAnimMesh::SetSwapFrame(int nCount) {
	m_nFrame = nCount;
}

/**
* @fn		CAnimMesh::SetLoop
* @brief	ループするかどうかの設定
* @param	(bool)	ループするのかいしないのかいどっちなんだい
*/
void CAnimMesh::SetLoop(bool loop) {
	m_bLoop = loop;
}

/**
* @fn		CAnimMesh::StartAnim
* @brief	アニメーション描画開始
* @param	(ID3D11ShaderResourceView*)	テクスチャ情報
* @param	(XMFLOAT3)					オブジェクトの座標
* @param	(XMFLOAT2)					オブジェクトの描画サイズ
* @param	(int)						テクスチャの分割数X
* @param	(int)						テクスチャの分割数Y
* @param	(int)						テクスチャの切替フレーム数
* @param	(XMFLOAT3)					色	
*/
void CAnimMesh::StartAnim(ID3D11ShaderResourceView* pTex,
	XMFLOAT3 pos,XMFLOAT2 size,int split_x,int split_y, bool loop,
	int swap_frame,XMFLOAT3 color) {
	//オブジェクト作成
	Object* pAnim = new Object("Anim",UPDATE_EFFECT,DRAW_EFFECT);

	//コンポーネント追加
	auto trans = pAnim->AddComponent<CTransform>();
	auto draw = pAnim->AddComponent<CAnimMesh>();

	//object設定
	trans->Pos = pos;
	draw->SetSize(size.x,size.y);
	draw->SetColor(color.x,color.y,color.z);
	draw->SetLoop(loop);
	draw->SetTexture(pTex);
	draw->SetAnimSplit(split_x,split_y);
	draw->SetSwapFrame(swap_frame);

	//マネージャーに追加
	ObjectManager::GetInstance()->AddObject(pAnim);
}

