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
class CAnimMesh;

/**列挙体宣言**/
enum PLAYER_STATE {
	IDLE_PLAYER = 0,
	RUN_PLAYER,			//横移動
	DUSH_PLAYER,		//エアダッシュ見たいのを作りたい（作ってない）
	JUMP_PLAYER,		//ジャンプしている
	FALL_PLAYER,		//落下している
	HIT_PLAYER,			//敵にヒットしたとき
	MISS_PLAYER,		//いわゆるゲームオーバー
	
	STOP_PLAYER,		//プレイヤーの足を止めたいとき

	MAX_PLAYER
};

//クラス定義
class CPlayer :public Component {
private:
	//メンバ変数
	/** @brief 座標、拡縮、回転*/
	CTransform* m_pPlayer;
	/** @brief 描画情報*/
	CAnimMesh*    m_pDraw2D;
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
	/** @brief ジャンプをしたかどうかのフラグ*/
	bool m_bJump;
	/** @brief パワーアップしているかどうかのフラグ*/
	bool m_bPawer_UP;
	/** @brief 無敵時間*/
	int m_nStar_Time;
	/** @brief 麦を向いているか左を向いているか(true)は右*/
	bool m_bROL;
	/** @brief どっちから当たったかを判別する(デフォルトは0)*/
	int m_nHitVec;

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
	/** @brief プレイヤーがどの方向から敵に当たったかを判別*/
	int CollEnemy(Object* pObject);
};


#endif