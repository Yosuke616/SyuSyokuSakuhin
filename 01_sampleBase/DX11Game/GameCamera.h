//=============================================================================
//
// ゲーム カメラ クラス [GameCamera.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Camera.h"

class CGameCamera : public CCamera
{
public:
	virtual void Init();
	virtual void Update();
};
