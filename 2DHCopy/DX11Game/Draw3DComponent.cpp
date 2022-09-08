//インクルード部
#include "Draw3dComponent.h"
#include "TransformComponent.h"
#include "InputManager.h"
#include "ObjInfo.h"
#include "sceneGame.h"
#include "Animation.h"

/**
* @fn		CDraw3D::CDraw3D
* @brief	またコンストラクタ先輩が状態
*/
CDraw3D::CDraw3D() 
	:m_pTransform(nullptr),m_pMaterial(nullptr),m_pAnim(nullptr){
	this->m_eUpdateOrder = COM_UPDATE_DRAW;

	//mat.Ka = DirectX::XMFLOAT4(0, 0, 0, 0);		// アンビエント+テクスチャ有無
	//mat.Kd = DirectX::XMFLOAT4(1, 1, 1, 1);		// ディフューズ
	//mat.Ks = DirectX::XMFLOAT4(0, 0, 0, 0);		// スペキュラ+スペキュラ強度
	//mat.Ke = DirectX::XMFLOAT4(0, 0, 0, 0);		// エミッシブ
	//mat.pTexture = nullptr;
	//mat.pTexEmmisive = nullptr;
	//mat.pTexSpecular = nullptr;
	//mat.pTexTransparent = nullptr;
}

/**
* @fn		CDraw3D::~CDraw3D
* @brief	デデデストラクタ大王
*/
CDraw3D::~CDraw3D() {
	if (m_pMaterial) {
		delete m_pMaterial;
		m_pMaterial = m_pMaterial;
	}

	if (m_pAnim) {
		delete m_pAnim;
		m_pAnim = nullptr;
	}
}

/**
* @fn		CDraw3D::Start()
* @brief	作戦開始
*/
void CDraw3D::Start() {
	//オブジェクトの座標を取得
	m_pTransform = Parent->GetComponent<CTransform>();

	/*if (m_model)
	{
		auto material = m_model->GetMate();
		m_Material = *material;
	}*/
}

/**
* @fn		CDraw3D::Update
* @brief	更新処理
*/
void CDraw3D::Update() {

	if (m_pAnim) {
		m_pAnim->Update();
	}

	XMMATRIX mtxWorld, mtxRot, mtxTranslate, mtxScale;

	//ワールドマトリックスの初期化
	mtxWorld = XMMatrixIdentity();

	//拡縮を反転
	mtxScale = XMMatrixScaling(
		m_pTransform->Scale.x,
		m_pTransform->Scale.y,
		m_pTransform->Scale.z
	);
	mtxWorld = XMMatrixMultiply(mtxWorld, mtxScale);

	//X回転を反映
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(m_pTransform->Rotate.x),
		0.0f,
		0.0f
	);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	//Y回転を反映
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.y),
		0.0f
	);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	//Z回転を反映
	mtxRot = DirectX::XMMatrixRotationRollPitchYaw(
		0.0f,
		0.0f,
		XMConvertToRadians(m_pTransform->Rotate.z)
	);
	mtxWorld = DirectX::XMMatrixMultiply(mtxWorld, mtxRot);

	//移動を反映
	mtxTranslate = XMMatrixTranslation(
		m_pTransform->Pos.x,
		m_pTransform->Pos.y,
		m_pTransform->Pos.z
	);
	mtxWorld = XMMatrixMultiply(mtxWorld,mtxTranslate);

	//ワールドマトリックス設定
	XMStoreFloat4x4(&m_mtxWorld,mtxWorld);
}

/**
* @fn		CDraw3D::Draw
* @brief	描画処理
*/
void CDraw3D::Draw() {
	//デバイスコンテクストの取得
	ID3D11DeviceContext* pDC = GetDeviceContext();

	//不透明部分を描画
	//ライトオフ
	CLight::Get()->SetDisable();
	//アルファブレンド有効
	SetBlendState(BS_NONE);
	//Zバッファ有効
	SetZBuffer(true);
	m_model->Draw(pDC,m_mtxWorld,eOpacityOnly);

	//半透明部分を描画
	//αブレンド有効
	SetBlendState(BS_ALPHABLEND);
	//Zバッファ更新する
	SetZWrite(false);
	m_model->Draw(pDC,m_mtxWorld,eTransparentOnly);

	//ライトオン
	CLight::Get()->SetEnable();
	//Zバッファ更新する
	SetZWrite(true);
	SetBlendState(BS_NONE);
}

/**
* @fn		Draw3D::SetPrimitiveType
* @brief	モデルのプリミティブ設定
* @param	(D3D_PRIMITIVE_TOPOLOGY)	プリミティブの情報？
*/
void CDraw3D::SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY type) {
	if (m_model != nullptr) {
		m_model->SetPrimitiveType(type);
	}
}

/**
* @fn		CDraw3D::GetWorldPos
* @brief    ワールド行列の取得
* @return	(XMFLOAT4X4*)	行列の入ったポインタを返す
*/
XMFLOAT4X4* CDraw3D::GetWorldPos() {
	if (m_model != nullptr) {
		return &(m_model->GetWorldMatrix());
	}
	return nullptr;
}

/**
* @fn		CDraw3D::SetTexture
* @brief	テクスチャの設定
* @param	(ID3D11ShaderResourceView*)	テクスチャの情報を行う
*/
void CDraw3D::SetTexture(ID3D11ShaderResourceView* pTexture) {
	if (pTexture) {
		if (!m_pMaterial) {
			m_pMaterial = new TAssimpMaterial();
		}
		m_pMaterial->pTexture = pTexture;
		m_model->SetMaterial(m_pMaterial);
	}
}

/**
* @fn		CDraw3D::GetBox
* @brief	大きさの取得
* @return	(XMFLOAT3)	縦横幅の値を返す
*/
XMFLOAT3 CDraw3D::GetBox() {
	return m_model->GetBBox();
}

/**
* @fn		CDra3D::GetModel
* @brief	モデルの情報を取得
* @return	(CAssimpModel*)	モデルの情報が入ったポインタを返す
*/
CAssimpModel* CDraw3D::GetModel(){
	return m_model;
}

/**
* @fn		CDraw3D::SetModel
* @brief	モデルの情報の設定
* @param	(CAssimpModel*)	モデルの情報を用意する
*/
void CDraw3D::SetModel(CAssimpModel* model) {
	//モデルのセット
	m_model = model;

	//アニメーションがあったら追加
	if (model->HasAnimation()) {
		//アニメーションクラスの作成
		if (!m_pAnim) {
			m_pAnim = new Animation;

		}
		m_pAnim->SetModel(this);
	}
}

/**
* @fn		CDraw3D::AddAnimation
* @brief	アニメーションの追加
* @param	(int)		アニメーションの番号
* @param	(double)	開始時間
* @param	(double)	終了時間
* @param	(double)	ループの始まりの時間
* @param	(bool)		ループするかどうか
*/
void CDraw3D::AddAnimation(int AnimID,double Start,double end, bool loop,double loopStartFrame) {
	if (m_pAnim) {
		m_pAnim->AddAnimation(AnimID,Start,end,loop,loopStartFrame);
	}
}

/**
* @fn		CDraw3D::Play
* @brief	アニメーションを再生する
* @param	(int)モデルの番号
*/
void CDraw3D::Play(int AnimID) {
	if (m_pAnim) {
		m_pAnim->Play(AnimID);
	}
}

/**
* @fn		CDraw3D::SetAnimTime
* @brief	アニメーションのフレーム設定
* @double	(dounle)	どの位のフレームにするかの数値
*/
void CDraw3D::SetAnimTime(double dTime) {
	m_model->SetAnimTime(dTime);
}

/**
* @fn		CDraw3D::AlwaysAnimation
* @brief	アニメーションを再生し続けるか
* @param	(bool)	どうするんだい状態
*/	
void CDraw3D::AlwaysAnimation(bool always) {
	if (m_pAnim) {
		m_pAnim->AlwaysAnimation(always);
	}
}

/**
* @fn		CDraw3D::SetPlaySpeed
* @brief	再生速度の設定
* @param	(int)	モデルの番号	
* @param	(double)再生速度
*/
void CDraw3D::SetPlaySpeed(int AnimID,double speed) {
	if (m_pAnim) {
		m_pAnim->SetPlaySpeed(AnimID,speed);
	}
}
/**
* @fn		CDraw3D::AnimEnd
* @brief	アニメーションを終わらせるか
* @param	(int)	モデルの番号
* @return	(bool)	終わらせれたかどうか
*/
bool CDraw3D::AnimEnd(int AnimID){
	if (m_pAnim) {
		return m_pAnim->AnimEnd(AnimID);
	}
	else {
		return false;
	}
}