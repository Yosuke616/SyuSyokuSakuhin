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
};

#endif