//インクルード部
#include "ColliderComponent.h"
#include "ObjectManager.h"
#include "Object.h"
#include "ObjInfo.h"
#include "TransformComponent.h"
#include "Draw2DComponent.h"
#include "PlayerComponent.h"

//静的メンバ変数
/** @brief 当たり判定のリスト*/
std::list<Object*> CCollider::m_ColliderList;

/**
* @fn		CCollider::CCollider()
* @brief	ただのコンストラクタそれ以上でも以下でもない
*/
CCollider::CCollider()
	:m_pTransform(nullptr){
	m_eUpdateOrder = COM_UPDATE_COLLIDER;		//コンポーネントリストの更新順序

	m_CenterOffset = XMFLOAT2(0.0f,0.0f);
	m_CollisionSize = XMFLOAT3(0.0f,0.0f,0.0f);
}

/**
* @fn		CCollider:~CCollider
* @brief	デストラクタだったもの
*/
CCollider::~CCollider() {

}

/**
* @fn		CCollider:Start
* @brief	初期化
*/
void CCollider::Start() {
	//コンポーネントの取得
	//オブジェクトの座標の取得
	m_pTransform = Parent->GetComponent<CTransform>();
	
	//当たり判定のリストに登録
	AddColliderList(Parent);

}

/**
* @fn		Collder::Update
* @brief	更新処理
*/
void CCollider::Update() {
	//衝突判定リストへの追加ループ
	for (auto&& OtherObject:m_ColliderList) {
		//どちらかの削除フラグがtrueだった場合 continue
		if (Parent->GetDeleteFlag() == true) {
			continue;
		}
		if (OtherObject->GetDeleteFlag() == true) {
			continue;
		}

		//コンポーネントが機能停止の時 continue
		auto OtherCollider = OtherObject->GetComponent<CCollider>();
		if (this->m_bUpdateFlag == false) {
			continue;
		}
		if (OtherCollider->m_bUpdateFlag == false) {
			continue;
		}

		//当たり判定を取る対象がこのオブジェクトだった場合 continue
		if (OtherObject == Parent) {
			continue;
		}

		//当たり判定を取る対象の情報を取得
		auto Other = OtherObject->GetComponent<CTransform>();

		//2回目の衝突判定を避ける
		bool bOnceHit = false;

		for (auto OnceCollision:this->m_IntersectObjects) {
			//自分が当たったことがあるオブジェクトのリスト == 現在の当たり判定を取る作業
			if (OnceCollision == OtherObject) {
				bOnceHit = true;
				break;
			}
		}
		if (bOnceHit) {
			continue;
		}

		//x,y平面の当たり判定
		if (this->CollisionRectToRectXY(
			m_pTransform,Other,
			XMFLOAT2(m_CollisionSize.x,m_CollisionSize.y),
			XMFLOAT2(OtherCollider->m_CollisionSize.x,
				OtherCollider->m_CollisionSize.y),
			m_CenterOffset,
			OtherCollider->m_CenterOffset)) {
			//相手に当たったことを知らせる
			OtherCollider->m_IntersectObjects.push_back(Parent);
		}
	}
}

/**
* @fn		CCollider::SetCollisionSize
* @brief	当たり判定の大きさの設定
* @param	(float)		当たり判定の横幅
* @param	(float)		当たり判定の立幅
* @param	(float)		当たり判定の奥行
*/
void CCollider::SetCollisionSize(float fWidth,float fHeight,float fz) {
	m_CollisionSize.x = fWidth;
	m_CollisionSize.y = fHeight;
	m_CollisionSize.z = fz;
}

/**
* @fn		CCollider::SetOffset
* @brief	オブジェクトと当たり判定のずれを直すための関数
* @param	(float)		x方向のずれを直す
* @param	(float)		y方向のずれを直す
*/
void CCollider::SetOffset(float fx,float fy) {
	m_CenterOffset.x = fx;
	m_CenterOffset.y = fy;
}

/**
* @fn		CCollider::GetOffset
* @brief	どの位ズレているかを取得する
* @return	(XMFLOAT2)	どの位中心からずれているかの情報
*/
XMFLOAT2 CCollider::GetOffSet() {
	return m_CenterOffset;
}

/**
* @fn		CCollider::GetColliderSize
* @brief	当たり判定の大きさを取得する
* @return	(XMFLOAT3)	当たり判定の大きさの情報
*/
XMFLOAT3 CCollider::GetColliderSize() {
	return m_CollisionSize;
}

/**
* @fn		CCollider::GetCenterPos
* @brief	当たり判定も中心座標を取得する
* @return	(XMFLOAT2)	当たり判定の中心の座標の情報
*/
XMFLOAT2 CCollider::GetCenterPos() {
	return XMFLOAT2(m_pTransform->Pos.x + m_CenterOffset.x,m_pTransform->Pos.y + m_CenterOffset.y);
}

/**
* @fn		CCollider::CheckCollisionRectToRect
* @brief	矩形同士の当たり判定
* @param	(XMFLOAT2)	片方の当たり判定の中心座標
* @param	(XMFLOAT2)	もう片方の当たり判定の中心座標
* @param	(XMFLOAT2)	片方の当たり判定の大きさ
* @param	(XMFLOAT2)	もう片方の当たり判定の大きさ
* @param	(XMFLOAT2)	片方の当たり判定の速度
* @param	(XMFLOAT2)	もう片方の当たり判定の速度
* @return	(bool)		当たっていたかどうかの真偽を返す
*/
bool CCollider::CheckCollisionRectToRect(XMFLOAT2 centerPos1, XMFLOAT2 centerPos2,
										 XMFLOAT2 size1, XMFLOAT2 size2,
										 XMFLOAT2 Velocity1, XMFLOAT2 Velocity2) {
	XMFLOAT2 halfSize1 = XMFLOAT2(size1.x / 2.0f,size1.y / 2.0f);
	XMFLOAT2 halfSize2 = XMFLOAT2(size2.x / 2.0f,size2.y / 2.0f);

	if (centerPos2.x - halfSize2.x + Velocity2.x < centerPos1.x + halfSize1.x + Velocity1.x &&
		centerPos1.x - halfSize1.x + Velocity1.x < centerPos2.x + halfSize2.x + Velocity2.x) {

		if (centerPos2.y - halfSize2.y + Velocity2.y < centerPos1.y + halfSize1.y + Velocity1.y &&
			centerPos1.y - halfSize1.y + Velocity1.y < centerPos2.y + halfSize2.y + Velocity2.y){
			
			return true;
		}
	}
	return false;
}

/**
* @fn		CCollider::CheckCollisionCircleToCircle
* @brief	円と円の当たり判定
* @param	(XMFLOAT2)	円の中心の座標
* @param	(XMFLOAT2)	もう片方の円の中心の座標
* @param	(float)		円の半径
* @param	(float)		もう片方の円の半径
* @return	(bool)		当たったか同課の真偽の判定
*/
bool CCollider::CheckCollisionCircleToCircle(XMFLOAT2 centerPos1,XMFLOAT2 centerPos2,
											 float radius1,float radius2) {
	if ((centerPos1.x - centerPos2.x) * (centerPos1.x - centerPos2.x)
		+ (centerPos1.y - centerPos2.y) * (centerPos1.y - centerPos2.y)
		<= (radius1 + radius2) * (radius1 * radius2)) {
	
		return true;
	}
	return false;
}

/**
* @fn		CCollider::CollisionSphere
* @brief	球と球の当たり判定
* @param	(XMFLOAT3)	球の座標
* @param	(XMFLOAT3)	もう片方の球の座標
* @param	(float)		球の半径
* @param	(float)		もう片方の球の半径
* @return	(bool)		当たったかどうかの真偽の判定
*/
bool CCollider::CollisionSphere(XMFLOAT3 Apos,XMFLOAT3 Bpos,float Ar,float Br) {
	float dx = Apos.x - Bpos.x;
	float dy = Apos.y - Bpos.y;
	float dz = Apos.z - Bpos.z;
	float dr = Ar + Br;
	return dx * dx + dy * dy + dz * dz <= dr * dr;
}

/**
* @fn		CCollider::CollisionAABB
* @brief	AABB同士の当たり判定
* @param	(XMFLOAT3)	ボックスの座標
* @param	(XMFLOAT3)	もう片方のボックスの座標
* @param	(XMFLOAT3)	ボックスの大きさ
* @param	(XMFLOAT3)	もう片方のボックスの大きさ
* @return	(bool)		当たっているかどうかの真偽の判定	
*/
bool CCollider::CollisionAABB(XMFLOAT3 Apos, XMFLOAT3 Bpos, XMFLOAT3 Asize, XMFLOAT3 Bsize) {
	return(Apos.x - Asize.x <= Bpos.x + Bsize.x) &&
		  (Bpos.x - Bsize.x <= Apos.x + Asize.x) &&
		  (Apos.y - Asize.y <= Bpos.y + Bsize.y) &&
		  (Bpos.y - Bsize.y <= Apos.y + Asize.y) &&
		  (Apos.z - Asize.z <= Bpos.z + Bsize.z) &&
		  (Bpos.z - Bsize.z <= Apos.z + Asize.z);
}

/**
* @fn		CCollider::CollisionRectToRectXY
* @brief	xy平面との当たり判定
* @param	(CTransform*)	オブジェクトの情報があるポインタ
* @param	(CTransform*)	もう片方のオブジェクトの情報があるポインタ
* @param	(XMFLOAT2)		オブジェクトの大きさ
* @param	(XMFLOAT2)		もう片方のオブジェクトの大きさ
* @param	(XMFLOAT2)		オブジェクトの中心のズレ
* @param	(XMFLOAT2)		もう片方のオブジェクトの中心のズレ
* @return	(bool)			当たっているかどうかの真偽の判定
*/
bool CCollider::CollisionRectToRectXY(CTransform* pA, CTransform* pB,
								      XMFLOAT2 Asize, XMFLOAT2 Bsize,
									  XMFLOAT2 Aoffset, XMFLOAT2 Boffset) {
	//中心座標
	XMFLOAT2 centerPosA = XMFLOAT2(pA->Pos.x + Aoffset.x,pA->Pos.y + Aoffset.y);
	XMFLOAT2 centerPosB = XMFLOAT2(pB->Pos.x + Boffset.x,pB->Pos.y + Boffset.y);
	XMFLOAT2 halfSizeA = XMFLOAT2(Asize.x / 2.0f,Asize.y / 2.0f);
	XMFLOAT2 halfSizeB = XMFLOAT2(Bsize.x / 2.0f,Bsize.y / 2.0f);

	if (centerPosB.x - halfSizeB.x < centerPosA.x + halfSizeA.x &&
		centerPosA.x - halfSizeA.x < centerPosB.x + halfSizeB.x) {
		if (centerPosB.y - halfSizeB.y < centerPosA.y + centerPosA.y &&
			centerPosA.y - halfSizeA.y < centerPosB.y + halfSizeB.y) {
			return true;
		}
	}

	return false;
}

/**
* @fn		CCollider::AddColliderList
* @brief	当たり判定のリストに追加する
* @param	(Object*)	リストに追加するオブジェクトのポインタ
*/
void CCollider::AddColliderList(Object* pObject) {
	//更新リストに追加
	std::list<Object*>::iterator itr = m_ColliderList.begin();
	for (auto&& object:m_ColliderList) {
		if (pObject->GetUpdateOrder() > object->GetUpdateOrder()) {
			itr++;
			continue;
		}
		else {
			break;
		}
	}
	m_ColliderList.insert(itr,pObject);
}

/**
* @fn		CCollider::GetColliderList
* @brief	当たり判定のリストの取得
* @return	(list<Object*>)&		当たり判定の入ったオブジェクト型のリストのアドレスを返す
*/
std::list<Object*>& CCollider::GetColliderList() {
	return m_ColliderList;
}

/**
* @fn		CCollider::UpdateColliders
* @brief	当たり判定リストに入っているオブジェクトの更新
*/
void CCollider::UpdateColliders() {
	for (auto this_object:m_ColliderList) {
		//オブジェクトの名前がプレイヤーの名前じゃなかった場合
		if (this_object->GetName() != PLAYER_NAME) {
			//コライダーの機能を持ってくる
			auto this_collider = this_object->GetComponent<CCollider>();

			//衝突時の処理
			//衝突したオブジェクト：衝突した相手たち
			for (auto collided_object:this_collider->m_IntersectObjects) {
				//このオブジェクトの衝突処理を実行
				this_object->OnCollisionEnter(collided_object);
				//衝突した相手たちの衝突処理を実行
				collided_object->OnCollisionEnter(this_object);
			}
		}
		//プレイヤーだった場合
		else {
			auto this_collider = this_object->GetComponent<CCollider>();

			//謎であるということはなぞということである
			if (this_collider->m_IntersectObjects.size() > 1) {
				this_collider->m_IntersectObjects.sort([=](Object* A, Object* B) {
					//座標の取得
					auto thisPos = this_collider->GetCenterPos();
					auto PosA = A->GetComponent<CCollider>()->GetCenterPos();
					auto PosB = B->GetComponent<CCollider>()->GetCenterPos();
					//距離のの計算
					XMFLOAT2 DiffA(thisPos.x - PosA.x, thisPos.y - PosA.y);
					XMFLOAT2 DiffB(thisPos.x - PosB.x, thisPos.y - PosB.y);

					//ここが特に謎である状態
					XMVECTOR vDiffA = DirectX::XMLoadFloat2(&DiffA);
					XMVECTOR vDiffB = DirectX::XMLoadFloat2(&DiffB);
					XMVECTOR vLengthA;
					XMVECTOR vLengthB;
					vLengthA = DirectX::XMVector2LengthEst(vDiffA);
					vLengthB = DirectX::XMVector2LengthEst(vDiffB);

					//Diffの再利用
					DirectX::XMStoreFloat2(&DiffA,vLengthA);
					DirectX::XMStoreFloat2(&DiffB,vLengthB);

					return DiffA.x < DiffB.x;
				});
			}

			//衝突したときの処理
			//衝突したオブジェクト：衝突した相手たち
			for (auto collided_object:this_collider->m_IntersectObjects) {
				//このオブジェクトの衝突処理の実行
				this_object->OnCollisionEnter(collided_object);
				//衝突した相手の衝突処理実行
				collided_object->OnCollisionEnter(this_object);
			}
		}
		//衝突したことを知らせるのをやめる
		this_object->GetComponent<CCollider>()->m_IntersectObjects.clear();

	}
}
