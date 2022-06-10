//=============================================================================
//
// プレイヤー クラス定義 [Player.h]
// Author : HIROHIKO HAMAYA
//
//=============================================================================
#pragma once
#include "Model.h"
class CPlayer : public CModel
{
public:
	CPlayer(CScene* pScene);
	virtual ~CPlayer();

	virtual HRESULT Init();
	virtual void Fin();
	virtual void Update();
};
