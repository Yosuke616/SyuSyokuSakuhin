//=============================================================================
//
// �Q�[�� �N���X��` [Game.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Scene.h"
#include "Player.h"
#include "GameCamera.h"

// �N���X��`
class CGame : public CScene
{
private:
	bool m_bResult;

	CGameCamera m_camera;
	CPlayer* m_pPlayer;

public:
	CGame();
	virtual ~CGame();

	virtual bool Init();
	virtual void Fin();
	virtual void Update();
	virtual void Draw();
};
