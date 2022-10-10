/**
* @file		ItemComponent.h
* @brief	スコアアップ、パワーアップ、キーアイテムの管理
* @detail	アイテムマネージャー的な要素もある
* @author	志水陽祐
* @date		2022/10/5
*/

/**インクルードガード**/
#ifndef _ITEM_COMPONENT_H_
#define _ITEM_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**前方宣言**/
class CTransform;
class Object;
class CCollider;
class CDraw3D;

/**列挙体宣言**/
enum ITEM_TYPE {
	ITEM_KOBAN,
	ITEM_MAGA,
	ITEM_OHUDA,

	MAX_ITEM
};

/**クラス定義**/
/**
* @brief	アイテムの動きを制御するコンポーネント
* @detail	マネージャー的な使い方でセーブとかしたい
*/
class CItem : public Component {
private:
	//メンバ変数
	/** @brief 座標とか決めます*/
	CTransform* m_pTransform;
	/** @brief モデルとかの設定*/
	CDraw3D* m_pDraw;
	/** @brief 当たり判定の情報*/
	CCollider* m_pCollider;
	/** @brief 自分のアイテムの種類*/
	ITEM_TYPE m_eItem;
	/** @brief 上下に移動させるための変数*/
	float m_fUpDown;
	/** @brief 上下に移動させるフラグ
			   trueで上　falseで下*/
	bool m_bUpDown;
	/** @brief パワーアップアイテムが取得されたかのフラグ*/
	bool m_bPlayer;
	/** @brief 指定の場所に付いたかついていないかを判定するフラグ
			   falseでつていない　trueでついている*/
	bool m_bSetUp;
	/** @brief 一度だけ逆向きに設定する*/
	bool m_bFirstBlad;
	/** @brief 回転させるための変数*/
	float m_fRotTime;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CItem();
	/** @brief デストラクタ*/
	~CItem();
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;
	/** @brief 当たり判定の処理*/
	void OnCollisionEnter(Object* pObject)override;
	/** @brief アイテムの種類の設定*/
	void SetItem(ITEM_TYPE type);
	/** @brief アイテムの種類の取得*/
	ITEM_TYPE GetItem();
	/** @brief プレイヤーに取得されたかを設定する*/
	void SetPlayer(bool player);

};

#endif