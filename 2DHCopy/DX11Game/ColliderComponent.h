/**
* @fn		ColliderComponent.h
* @brief	このコンポーネントを持つオブジェクトどうしが当たっているか判断する
* @author	志水陽祐
* @date		2022/6/22
*
* @detail	必要な設定項目
*			当たり判定の大きさ
*
*/
//インクルードガード
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//インクルード
#include "mainApp.h"
#include "Component.h"

#include <vector>

//前方宣言
class ObjectManager;
class Object;

//クラス定義
class CCollider :public Component {
private:
	//メンバ変数
	/** @brief 座標*/

	/** @brief 当たり判定の大きさ*/	
	XMFLOAT3 m_CollisionSize;
	/** @brief モデル中心のズレ*/
	XMFLOAT2 m_CenterOffset;
	/** @brief 半径*/
	float m_fRadisu;
	/** @brief 当たり判定のリスト*/
	static std::list<Object*> m_ColliderList;
	/** @brief 衝突したオブジェクトたち*/
	std::list<Object*> m_IntersectObjects;

	//メンバ関数
	//当たり判定と愉快な仲間たち
	/** @brief 矩形どうしの当たり判定*/
	bool CheckCollisionRectToRect(XMFLOAT2 centerPos1,XMFLOAT2 centerPos2,
		XMFLOAT2 size1,XMFLOAT2 size2,XMFLOAT2 Velocity1,XMFLOAT2 Velocity2);
	/** @brief 円と円の当たり判定*/
	bool CheckCollisionCircleToCircle(XMFLOAT2, XMFLOAT2, float, float);
	/** @brief 球と球の当たり判定*/
	bool CollisionSphere(XMFLOAT3 Apos,XMFLOAT3 Bpos, float Ar, float Br);
	/** @brief 直方体と直方体の当たり判定*/
	bool CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Asize, XMFLOAT3 Bpos, XMFLOAT3 Bsize);
	/** @brief xy平面の当たり判定*/
	bool CollisionRectToRectXY();

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CCollider();
	/** @brief デストラクタ*/
	~CCollider() override;
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新*/
	void Update() override;

	/** @brief 当たり判定リストに追加*/
	static void AddColliderList(Object* pObject);
	/** @brief 当たり判定を持っているオブジェクトのリストを取得*/
	static std::list<Object*>& GetColliderList();
	/** @brief 当たり判定を持っているオブジェクトたちの更新*/
	static void UpdateColliders();

	//ゲッター
	/** @brief オブジェクトの中心のズレ*/
	XMFLOAT2 GetOffSet();
	/** @brief 当たり判定の大きさの取得*/
	XMFLOAT3 GetColliderSize();
	/** @brief 当たり判定の中心の座標の取得*/
	XMFLOAT2 GetCenterPos();

	//セッター
	/** @brief 当たり判定の大きさの設定*/
	void SetCollisionSize(float fx,float fy,float fz = 0.0f);
	/** @brief 当たり判定の中心とオブジェクトの中心とのズレ*/
	void SetOffset(float fx,float fy);
};

#endif