//=============================================================================
//
// プレイヤー クラス実装 [Player.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "Player.h"
#include "Input.h"
#include "DebugProc.h"

// グローバル変数
namespace {
	const float VALUE_MOVE_PLAYER = 1.0f;
	const float VALUE_ROTATE_PLAYER = 1.0f;
}

// コンストラクタ
CPlayer::CPlayer(CScene* pScene) : CModel(pScene)
{
}

// デストラクタ
CPlayer::~CPlayer()
{
}

// 初期化
HRESULT CPlayer::Init()
{
	HRESULT hr = CModel::Init();
	SetModel(MODEL_PLAYER);
	return hr;
}

// 終了処理
void CPlayer::Fin()
{
	CModel::Fin();
}

// 更新
void CPlayer::Update()
{
	// カメラの向き取得
	XMFLOAT3 rotCamera = CCamera::Get()->GetAngle();

	if (CInput::GetKeyPress(VK_LEFT)) {
		if (CInput::GetKeyPress(VK_UP)) {
			// 左前移動
			m_vPos.x -= SinDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y + 135.0f) * VALUE_MOVE_PLAYER;
		} else if (CInput::GetKeyPress(VK_DOWN)) {
			// 左後移動
			m_vPos.x -= SinDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y + 45.0f) * VALUE_MOVE_PLAYER;
		} else {
			// 左移動
			m_vPos.x -= SinDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y + 90.0f) * VALUE_MOVE_PLAYER;
		}
	} else if (CInput::GetKeyPress(VK_RIGHT)) {
		if (CInput::GetKeyPress(VK_UP)) {
			// 右前移動
			m_vPos.x -= SinDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y - 135.0f) * VALUE_MOVE_PLAYER;
		} else if (CInput::GetKeyPress(VK_DOWN)) {
			// 右後移動
			m_vPos.x -= SinDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y - 45.0f) * VALUE_MOVE_PLAYER;
		} else {
			// 右移動
			m_vPos.x -= SinDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
			m_vPos.z -= CosDeg(rotCamera.y - 90.0f) * VALUE_MOVE_PLAYER;
		}
	} else if (CInput::GetKeyPress(VK_UP)) {
		// 前移動
		m_vPos.x -= SinDeg(rotCamera.y + 180.0f) * VALUE_MOVE_PLAYER;
		m_vPos.z -= CosDeg(rotCamera.y + 180.0f) * VALUE_MOVE_PLAYER;
	} else if (CInput::GetKeyPress(VK_DOWN)) {
		// 後移動
		m_vPos.x -= SinDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
		m_vPos.z -= CosDeg(rotCamera.y) * VALUE_MOVE_PLAYER;
	}

	if (CInput::GetKeyPress(VK_LSHIFT)) {
		// 左回転
		m_vAngle.y -= VALUE_ROTATE_PLAYER;
		if (m_vAngle.y < -180.0f) {
			m_vAngle.y += 360.0f;
		}
	}
	if (CInput::GetKeyPress(VK_RSHIFT)) {
		// 右回転
		m_vAngle.y += VALUE_ROTATE_PLAYER;
		if (m_vAngle.y >= 180.0f) {
			m_vAngle.y -= 360.0f;
		}
	}

	if (CInput::GetKeyPress(VK_RETURN)) {
		// リセット
		m_vPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vAngle = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	CDebugProc::Print("[ｸﾙﾏ ｲﾁ : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[ｸﾙﾏ ﾑｷ : (%f)]\n", m_vAngle.y);
	CDebugProc::Print("\n");

	CDebugProc::Print("*** ｸﾙﾏ ｿｳｻ ***\n");
	CDebugProc::Print("ﾏｴ   ｲﾄﾞｳ : \x1e\n");//↑
	CDebugProc::Print("ｳｼﾛ  ｲﾄﾞｳ : \x1f\n");//↓
	CDebugProc::Print("ﾋﾀﾞﾘ ｲﾄﾞｳ : \x1d\n");//←
	CDebugProc::Print("ﾐｷﾞ  ｲﾄﾞｳ : \x1c\n");//→
	CDebugProc::Print("ﾋﾀﾞﾘ ｾﾝｶｲ : LSHIFT\n");
	CDebugProc::Print("ﾐｷﾞ  ｾﾝｶｲ : RSHIFT\n");
	CDebugProc::Print("\n");

	CModel::Update();
}
