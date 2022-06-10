//=============================================================================
//
// ゲーム カメラ クラス [GameCamera.cpp]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#include "GameCamera.h"
#include "Input.h"
#include "DebugProc.h"
#include "Model.h"

// グローバル変数
namespace {
	const float VALUE_MOVE_CAMERA = 0.5f;			// カメラ移動量
	const float VALUE_ROTATE_CAMERA = 0.5f;			// カメラ回転量
}

//初期化
void CGameCamera::Init() {
	CCamera::Init();
	SetSky(CModel::GetAssimp(MODEL_SKY));
}

// 更新
void CGameCamera::Update()
{
	if (CInput::GetKeyPress(VK_A)) {
		if (CInput::GetKeyPress(VK_W)) {
			// 左前移動
			m_vPos.x -= CosDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z += SinDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
		} else if (CInput::GetKeyPress(VK_S)) {
			// 左後移動
			m_vPos.x -= CosDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z += SinDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
		} else {
			// 左移動
			m_vPos.x -= CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
			m_vPos.z += SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	} else if (CInput::GetKeyPress(VK_D)) {
		if (CInput::GetKeyPress(VK_W)) {
			// 右前移動
			m_vPos.x += CosDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z -= SinDeg(m_vAngle.y - 45.0f) * VALUE_MOVE_CAMERA;
		} else if (CInput::GetKeyPress(VK_S)) {
			// 右後移動
			m_vPos.x += CosDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
			m_vPos.z -= SinDeg(m_vAngle.y + 45.0f) * VALUE_MOVE_CAMERA;
		} else {
			// 右移動
			m_vPos.x += CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
			m_vPos.z -= SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	} else if (CInput::GetKeyPress(VK_W)) {
		// 前移動
		m_vPos.x += SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		m_vPos.z += CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	} else if (CInput::GetKeyPress(VK_S)) {
		// 後移動
		m_vPos.x -= SinDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;
		m_vPos.z -= CosDeg(m_vAngle.y) * VALUE_MOVE_CAMERA;

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	}

	if (CInput::GetKeyPress(VK_Z)) {
		// 視点旋回「左」
		m_vAngle.y += VALUE_ROTATE_CAMERA;
		if (m_vAngle.y >= 180.0f) {
			m_vAngle.y -= 360.0f;
		}

		m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_C)) {
		// 視点旋回「右」
		m_vAngle.y -= VALUE_ROTATE_CAMERA;
		if (m_vAngle.y < -180.0f) {
			m_vAngle.y += 360.0f;
		}

		m_vPos.x = m_vTarget.x - SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vPos.z = m_vTarget.z - CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_Y)) {
		// 視点移動「上」
		m_vPos.y += VALUE_MOVE_CAMERA;
	}
	if (CInput::GetKeyPress(VK_N)) {
		// 視点移動「下」
		m_vPos.y -= VALUE_MOVE_CAMERA;
	}

	if (CInput::GetKeyPress(VK_Q)) {
		// 注視点旋回「左」
		m_vAngle.y -= VALUE_ROTATE_CAMERA;
		if (m_vAngle.y < -180.0f) {
			m_vAngle.y += 360.0f;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_E)) {
		// 注視点旋回「右」
		m_vAngle.y += VALUE_ROTATE_CAMERA;
		if (m_vAngle.y >= 180.0f) {
			m_vAngle.y -= 360.0f;
		}

		m_vTarget.x = m_vPos.x + SinDeg(m_vAngle.y) * m_fLengthInterval;
		m_vTarget.z = m_vPos.z + CosDeg(m_vAngle.y) * m_fLengthInterval;
	}
	if (CInput::GetKeyPress(VK_T)) {
		// 注視点移動「上」
		m_vTarget.y += VALUE_MOVE_CAMERA;
	}
	if (CInput::GetKeyPress(VK_B)) {
		// 注視点移動「下」
		m_vTarget.y -= VALUE_MOVE_CAMERA;
	}

	if (CInput::GetKeyTrigger(VK_SPACE)) {
		// リセット
		Init();
	}

	CDebugProc::Print("[ｶﾒﾗ ｲﾁ : (%f, %f, %f)]\n", m_vPos.x, m_vPos.y, m_vPos.z);
	CDebugProc::Print("[ﾁｭｳｼﾃﾝ : (%f, %f, %f)]\n", m_vTarget.x, m_vTarget.y, m_vTarget.z);
	CDebugProc::Print("[ｶﾒﾗ ﾑｷ : (%f)]\n", m_vAngle.y);
	CDebugProc::Print("\n");

	CDebugProc::Print("*** ｼﾃﾝ ｿｳｻ ***\n");
	CDebugProc::Print("ﾏｴ   ｲﾄﾞｳ : W\n");
	CDebugProc::Print("ｳｼﾛ  ｲﾄﾞｳ : S\n");
	CDebugProc::Print("ﾋﾀﾞﾘ ｲﾄﾞｳ : A\n");
	CDebugProc::Print("ﾐｷﾞ  ｲﾄﾞｳ : D\n");
	CDebugProc::Print("ｳｴ   ｲﾄﾞｳ : Y\n");
	CDebugProc::Print("ｼﾀ   ｲﾄﾞｳ : N\n");
	CDebugProc::Print("ﾋﾀﾞﾘ ｾﾝｶｲ : Z\n");
	CDebugProc::Print("ﾐｷﾞ  ｾﾝｶｲ : C\n");
	CDebugProc::Print("\n");

	CDebugProc::Print("*** ﾁｭｳｼﾃﾝ ｿｳｻ ***\n");
	CDebugProc::Print("ｳｴ   ｲﾄﾞｳ : T\n");
	CDebugProc::Print("ｼﾀ   ｲﾄﾞｳ : B\n");
	CDebugProc::Print("ﾋﾀﾞﾘ ｾﾝｶｲ : Q\n");
	CDebugProc::Print("ﾐｷﾞ  ｾﾝｶｲ : E\n");
	CDebugProc::Print("\n");

	CCamera::Update();
}
