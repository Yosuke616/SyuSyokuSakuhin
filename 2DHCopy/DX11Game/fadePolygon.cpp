/**インクルード部**/
#include "fadePolygon.h"
#include "Shader.h"

/**構造体定義**/
//シェーダーに渡す値
struct SHADER_GLOBAL {
	XMMATRIX mWorld;	// ワールド変換行列(転置行列)
	XMMATRIX mView;		// ビュー変換行列(転置行列)
	XMMATRIX mProj;		// 射影変換行列(転置行列)
	XMMATRIX mTex;		// テクスチャ変換行列(転置行列)
	XMVECTOR vWipe;		// ワイプ処理
};

/**プロトタイプ宣言**/
static HRESULT MakeVertexFadePolygon(ID3D11Device* pDevice);
static void SetVertexFadePolygon(void);

/**グローバル変数**/
static ID3D11ShaderResourceView* g_pTexture; // テクスチャへのポインタ

static VERTEX_2D g_vertexWk[NUM_VERTEX]; // 頂点情報格納ワーク

static XMFLOAT3 g_posFadePolygon; // ポリゴンの移動量
static XMFLOAT3 g_rotFadePolygon; // ポリゴンの回転量
static XMFLOAT3 g_sizFadePolygon; // ポリゴンのサイズ
static XMFLOAT4 g_colFadePolygon; // ポリゴンの頂点カラー
static bool g_bInvalidate; // 頂点データ更新フラグ

static XMFLOAT2 g_posTexFrame; // UV座標
static XMFLOAT2 g_sizTexFrame; // テクスチャ抽出サイズ

static ID3D11Buffer* g_pConstantBuffer; // 定数バッファ
static ID3D11Buffer* g_pVertexBuffer; // 頂点バッファ
static ID3D11SamplerState* g_pSamplerState; // テクスチャ サンプラ
static ID3D11VertexShader* g_pVertexShader; // 頂点シェーダ
static ID3D11InputLayout* g_pInputLayout; // 頂点フォーマット
static ID3D11PixelShader* g_pPixelShader; // ピクセルシェーダ

static XMFLOAT4X4	g_mProj;	// 射影変換行列
static XMFLOAT4X4	g_mView;	// ビュー変換行列
static XMFLOAT4X4	g_mWorld;	// ワールド変換行列
static XMFLOAT4X4	g_mTex;		// テクスチャ変換行列
static XMFLOAT2		g_fWipe;	// ワイプ

/**
* @fn		InitFadePolygon
* @brief	初期化
* @param	(ID3D11Device)	デバイス
* @return	(HRESULT)		成功したかどうか
*/
HRESULT InitFadePolygon(ID3D11Device* pDevice)
{
	HRESULT hr = S_OK;

	// シェーダ初期化
	static const D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	hr = LoadShader(L"VertexFade", L"PixelFade",
		&g_pVertexShader, &g_pInputLayout, &g_pPixelShader, layout, _countof(layout));
	if (FAILED(hr)) {
		return hr;
	}

	// 定数バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SHADER_GLOBAL);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
	if (FAILED(hr)) return hr;

	// テクスチャ サンプラ生成
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = pDevice->CreateSamplerState(&sd, &g_pSamplerState);
	if (FAILED(hr)) {
		return hr;
	}

	// 変換行列初期化
	XMStoreFloat4x4(&g_mWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&g_mView, XMMatrixLookAtLH(XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)));
	XMStoreFloat4x4(&g_mProj,
		XMMatrixOrthographicLH(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 100.0f));
	XMStoreFloat4x4(&g_mTex, XMMatrixIdentity());
	g_mTex._44 = 0.0f;
	g_fWipe = XMFLOAT2(0.0f, 0.0f);						// ワイプ初期化

	g_posFadePolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_rotFadePolygon = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_sizFadePolygon = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_colFadePolygon = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_bInvalidate = false;
	g_posTexFrame = XMFLOAT2(0.0f, 0.0f);
	g_sizTexFrame = XMFLOAT2(1.0f, 1.0f);

	// 頂点情報の作成
	hr = MakeVertexFadePolygon(pDevice);

	return hr;
}

/**
* @fn		UninitFadePolygon
* @brief	終了処理
*/
void UninitFadePolygon(void)
{
	// テクスチャ　サンプラの開放
	SAFE_RELEASE(g_pSamplerState);
	// 頂点バッファの解放
	SAFE_RELEASE(g_pVertexBuffer);
	// 定数バッファの解放
	SAFE_RELEASE(g_pConstantBuffer);
	// ピクセルシェーダ解放
	SAFE_RELEASE(g_pPixelShader);
	// 頂点フォーマット解放
	SAFE_RELEASE(g_pInputLayout);
	// 頂点シェーダ解放
	SAFE_RELEASE(g_pVertexShader);
}

/**
* @fn		UpdateFadePolygon
* @brief	更新処理
* @param	(float)
*/
void UpdateFadePolygon(float fSpeed)
{
	// (何もしない)
	g_fWipe.x = fSpeed;			// ワイプ更新
}

/**
* @fn		DrawFadePolygon
* @brief	描画処理
* @param	(ID3D11DeviceContext)	デバイス
*/
void DrawFadePolygon(ID3D11DeviceContext* pDeviceContext)
{
	// 拡縮
	XMMATRIX mWorld = XMMatrixScaling(g_sizFadePolygon.x, g_sizFadePolygon.y, g_sizFadePolygon.z);
	// 回転
	mWorld *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(g_rotFadePolygon.x),
		XMConvertToRadians(g_rotFadePolygon.y), XMConvertToRadians(g_rotFadePolygon.z));
	// 移動
	mWorld *= XMMatrixTranslation(g_posFadePolygon.x, g_posFadePolygon.y, g_posFadePolygon.z);
	// ワールド マトリックスに設定
	XMStoreFloat4x4(&g_mWorld, mWorld);

	if (g_pTexture) {
		// 拡縮
		mWorld = XMMatrixScaling(g_sizTexFrame.x, g_sizTexFrame.y, 1.0f);
		// 移動
		mWorld *= XMMatrixTranslation(g_posTexFrame.x, g_posTexFrame.y, 0.0f);
		// テクスチャ マトリックスに設定
		XMStoreFloat4x4(&g_mTex, mWorld);
	}
	else {
		// テクスチャ無し
		g_mTex._44 = 0.0f;
	}

	// 頂点バッファ更新
	SetVertexFadePolygon();

	pDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
	pDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
	pDeviceContext->IASetInputLayout(g_pInputLayout);

	UINT stride = sizeof(VERTEX_2D);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	pDeviceContext->PSSetSamplers(0, 1, &g_pSamplerState);
	pDeviceContext->PSSetShaderResources(0, 1, &g_pTexture);

	SHADER_GLOBAL cb;
	cb.mProj = XMMatrixTranspose(XMLoadFloat4x4(&g_mProj));
	cb.mView = XMMatrixTranspose(XMLoadFloat4x4(&g_mView));
	cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&g_mWorld));
	cb.mTex = XMMatrixTranspose(XMLoadFloat4x4(&g_mTex));
	cb.vWipe = XMLoadFloat2(&g_fWipe);			// ワイプの値をシェーダ側に送る
	pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	pDeviceContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	// プリミティブ形状をセット
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴンの描画
	pDeviceContext->Draw(NUM_VERTEX, 0);
}

/**
* @fn		MakeVertexFadePolygon
* @brief	頂点の作成
* @param	(ID3D11Device*)	デバイス
* @return	(HRESULT)		成功したかどうか
*/
HRESULT MakeVertexFadePolygon(ID3D11Device* pDevice)
{
	// 頂点座標の設定
	g_vertexWk[0].vtx = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	g_vertexWk[1].vtx = XMFLOAT3(0.5f, 0.5f, 0.0f);
	g_vertexWk[2].vtx = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	g_vertexWk[3].vtx = XMFLOAT3(0.5f, -0.5f, 0.0f);

	// 拡散反射光の設定
	g_vertexWk[0].diffuse = g_colFadePolygon;
	g_vertexWk[1].diffuse = g_colFadePolygon;
	g_vertexWk[2].diffuse = g_colFadePolygon;
	g_vertexWk[3].diffuse = g_colFadePolygon;

	// テクスチャ座標の設定
	g_vertexWk[0].tex = XMFLOAT2(0.0f, 0.0f);
	g_vertexWk[1].tex = XMFLOAT2(1.0f, 0.0f);
	g_vertexWk[2].tex = XMFLOAT2(0.0f, 1.0f);
	g_vertexWk[3].tex = XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(g_vertexWk);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = &g_vertexWk[0];

	HRESULT hr = pDevice->CreateBuffer(&vbd, &initData, &g_pVertexBuffer);

	return hr;
}

/**
* @fn		SetVertexFadePolygon
* @brief	頂点座標の設定
*/
void SetVertexFadePolygon(void)
{
	if (g_bInvalidate) {
		// 頂点バッファの中身を埋める
		ID3D11DeviceContext* pDeviceContext = GetDeviceContext();
		HRESULT hr = S_OK;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		D3D11_MAPPED_SUBRESOURCE msr;
		hr = pDeviceContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		if (SUCCEEDED(hr)) {
			// 拡散反射光の設定
			g_vertexWk[0].diffuse = g_colFadePolygon;
			g_vertexWk[1].diffuse = g_colFadePolygon;
			g_vertexWk[2].diffuse = g_colFadePolygon;
			g_vertexWk[3].diffuse = g_colFadePolygon;
			// 頂点データを上書き
			memcpy_s(msr.pData, sizeof(g_vertexWk), g_vertexWk, sizeof(g_vertexWk));
			// 頂点データをアンロックする
			pDeviceContext->Unmap(g_pVertexBuffer, 0);
			// フラグをクリア
			g_bInvalidate = false;
		}
	}
}

/**
* @fn		SetFadePolygonTexture
* @brief	テクスチャの設定
* @param	(ID3D11ShaderResourceView*)	テクスチャ情報
*/
void SetFadePolygonTexture(ID3D11ShaderResourceView* pTexture)
{
	g_pTexture = pTexture;
	g_mTex._44 = (g_pTexture) ? 1.0f : 0.0f;
}

/**
* @fn		SetFadePolygonPos
* @brief	座標の設定
* @param	(float)	横の座標
* @param	(float)	縦の座標	
*/
void SetFadePolygonPos(float fX, float fY)
{
	g_posFadePolygon.x = fX;
	g_posFadePolygon.y = fY;
}

/**
* @fn		SetFadePolygonSize
* @brief	表示サイズの設定
* @param	(float)	横の大きさ
* @param	(float)	縦の大きさ
*/
void SetFadePolygonSize(float fScaleX, float fScaleY)
{
	g_sizFadePolygon.x = fScaleX;
	g_sizFadePolygon.y = fScaleY;
}

/**
* @fn		SetFadePolygonAngle
* @brief	表示角度の設定
* @param	(float)	角度の設定	
*/
void SetFadePolygonAngle(float fAngle)
{
	g_rotFadePolygon.z = fAngle;
}

/**
* @fn		SetFadePolygonUV
* @brief	左上テクスチャ座標の設定
* @param	(float)	テクスチャの座標設定
* @param	(float)	テクスチャの座標設定
*/
void SetFadePolygonUV(float fU, float fV)
{
	g_posTexFrame.x = fU;
	g_posTexFrame.y = fV;
}

/**
* @fn		SetFadePolygonFrameSize
* @brief	テクスチャフレームサイズの設定
* @param	(float)	横の大きさ
* @param	(float)	縦の大きさ
*/
void SetFadePolygonFrameSize(float fWidth, float fHeight)
{
	g_sizTexFrame.x = fWidth;
	g_sizTexFrame.y = fHeight;
}

/**
* @fn		SetFadePolygonColor
* @brief	頂点カラーの設定
* @param	(float)	R
* @param	(float)	G
* @param	(float)	B
*/
void SetFadePolygonColor(float fRed, float fGreen, float fBlue)
{
	if (fRed != g_colFadePolygon.x || fGreen != g_colFadePolygon.y || fBlue != g_colFadePolygon.z) {
		g_colFadePolygon.x = fRed;
		g_colFadePolygon.y = fGreen;
		g_colFadePolygon.z = fBlue;
		g_bInvalidate = true;
	}
}

/**
* @fn		SetFadePolygonAlpha
* @brief	不透明度の設定
* @param	(float)	透明度
*/
void SetFadePolygonAlpha(float fAlpha)
{
	if (fAlpha != g_colFadePolygon.w) {
		g_colFadePolygon.w = fAlpha;
		g_bInvalidate = true;
	}
}