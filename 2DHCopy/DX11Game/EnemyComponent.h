/**
* @file		EnemyComponent.h
* @brief	敵の動きを制御するコンポーネント
* @author	志水陽祐
* @date		2022/8/4
*/

/**インクルードガード**/
#ifndef _ENEMY_COMPONENT_H_
#define _ENEMY_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;
class Object;
class CCollider;
class CDraw3D;

/**列挙体宣言**/
//敵の種類
enum ENEMY_TYPE {
	ENEMY_WALK = 0,

	MAX_ENEMY_TYPE
};

//敵の状態
enum ENEMY_STATE {
	ENEMY_DEFAULT = 0,
	ENEMY_DELETE,

	MAX_ENEMY_STATE
};

/**クラス定義**/
/**
* @brief	敵の動きを制御する
* @detail	攻撃したり引き返したり
*/
class CEnemy :public Component {
private:
	//メンバ変数
	/** @brief 座標、拡縮、回転*/
	CTransform* m_pTransform;
	/** @brief 描画情報*/
	CDraw3D* m_pDraw;
	/** @brief 当たり判定*/
	CCollider* m_pCollider;
	/** @brief 左右のどちらを向いているか(trueだったら左へ行く)*/
	bool m_bRightorLeft;
	/** @brief 過去座標の保存*/
	XMFLOAT2 m_OldPos;
	/** @brief 敵の種類を管理する変数*/
	ENEMY_TYPE m_eEnemy_Type;
	/** @brief 敵の状態を管理する変数*/
	ENEMY_STATE m_eEnemy_State;
	/** @brief 一度だけコンポーネントの機能を停止する為の変数*/
	bool m_bStopCom;
	/** @brief プレイヤーの攻撃が右から当たったか左から当たったかを判別する*/
	bool m_bRoL;
	/** @brief  途中で速度を切り上げるようにする*/
	/** @detail trueにしてフラグをオンにする*/
	bool m_bStopVel;
	/** @brief 点滅用の変数*/
	int m_nBlink;
	/** @brief さよならオブジェクトする変数*/
	int m_nDeleteFlg;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CEnemy();
	/** @brief デストラクタ*/
	~CEnemy()override;
	/** @brief 初期化*/
	void Start()override;
	/** @brief 更新処理*/
	void Update()override;
	/** @brief 描画処理*/
	void Draw()override;
	/** @brief 当たり判定の処理*/
	void OnCollisionEnter(Object* pObject)override;
	/** @brief 敵の種類の設定*/
	void SetEnemyType(ENEMY_TYPE type);
};

#endif