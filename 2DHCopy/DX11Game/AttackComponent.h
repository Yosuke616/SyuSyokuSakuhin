/**
* @file		AttackComponent.h
* @brief	プレイヤーや敵の攻撃を処理する関数
* @author	志水陽祐
* @date		2022/8/10
*/

/**インクルードガード**/
#ifndef _ATTACK_COMPONENT_H_
#define _ATTACK_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;
class Object;
class CCollider;
class CDrawMesh;

/**列挙体宣言**/
/** 誰が攻撃しているかを判別する列挙体*/
enum WHO_ATTACK {
	NONE_ATTACK = 0,
	PLAYER_ATTACK,

	MAX_WHO_ATTACK
};

/**クラス定義**/
/**
* @brief	攻撃の動きを制御する
* @detail	攻撃を行った者のよって挙動を変える
*/
/**必要なさそうな変数は消していく**/
class CAttack :public Component {
private:
	//メンバ変数
	/** @brief 座標とかの情報*/
	CTransform* m_pTrans;
	/** @brief  左右のどちらから攻撃されたかを判別する変数*/
	/** @detail trueは右向き*/
	bool m_bRoL;
	/** @brief どのオブジェクトによる攻撃か*/
	WHO_ATTACK m_eAttack;
	/** @brief 攻撃用の当たり判定の存在していい時間*/
	int m_nAttackCnt;
	/** @brief プレイヤーのオブジェクトを得る*/
	Object* m_pObj;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CAttack();
	/** @brief デストラクタ*/
	~CAttack()override;
	/** @brief 初期化*/
	void Start()override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 左右どちらからの攻撃かを取得する*/
	bool GetAttackRoL();
	/** @brief どの種類のオブジェクトによる攻撃かを設定する*/
	void SetAttackType(WHO_ATTACK who);
	/** @brief 左右どっちを向いているかセットする関数*/
	void SetRoL(bool RoL);
	/** @brief オブジェクトの情報を取得する*/
	void SetObject(Object* pObj);

};

#endif