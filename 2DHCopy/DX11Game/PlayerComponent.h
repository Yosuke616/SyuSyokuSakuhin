/**
* @file		PlayerComponent.h
* @brief	プレイヤーの操作をするコンポーネント
* @author	志水陽祐
* @date		2022/7/14
*/

//インクルードガード
#ifndef _PLAYER_COMPONENT_H_
#define _PLAYER_COMPONENT_H_

//インクルード部
#include "Component.h"

//前方宣言
class InputManager;
class CTransform;
class Object;
class CCollider;
class CDraw2D;

/**列挙体宣言**/
enum PLAYER_STATE {
	IDLE_PLAYER = 0,
	RUN_PLAYER,
	JUMP_PLAYER,
	FALL_PLAYER,

	MAX_PLAYER
};

//クラス定義
class CPlayer :public Component {
private:
	//メンバ変数
	/** @brief 座標、拡縮、回転*/
	CTransform* m_pPlayer;
	/** @brief 描画情報*/
	CDraw2D*    m_pDraw2D;
	/** @brief 当たり判定*/
	CCollider* m_pCollider;
	/** @brief インプットマネージャーの取得*/
	InputManager* m_pInput;
	/** @brief 過去座標の保存*/
	XMFLOAT2 m_OldPos;
	/** @brief 地上にいるかどうかのフラグ*/
	bool m_bGround;
	/** @brief プレイヤーの状態を判別する列挙体*/
	PLAYER_STATE m_ePlayer;
	/** @brief ジャンプを下かどうかのフラグ*/
	bool m_bJump;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数
	
public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CPlayer();
	/** @brief デストラクタ*/
	~CPlayer() override;
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新*/
	void Update() override;
	/** @brief 描画*/
	void Draw() override;
	/** @brief 衝突処理*/
	void OnCollisionEnter(Object* pObject) override;
	/** @brief プレイヤーの状態を変更するため*/
	void SetPlayerState(PLAYER_STATE PlayerSta);
};


#endif