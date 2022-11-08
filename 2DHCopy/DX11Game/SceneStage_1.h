/**
* @file		SceneStage_1.h
* @brief	ステージ1の作成
* @author	志水陽祐
* @date		2022/7/30
*/

/**インクルードガード**/
#ifndef _STAGE_1_H_
#define _STAGE_1_H_

/**インクルード部**/
#include "Scene.h"
#include "mainApp.h"
#include "Object.h"

/**クラス定義**/
class SceneStage_1 :public Scene {
private:
	//メンバ変数
	/** @brief プレイヤーのポインタを保存する変数*/
	Object* m_pPlayer;
	/** @brief インスタンス*/
	static SceneStage_1* m_pInstance;
	/** @brief イベントに当たったとき用のフラグ*/
	bool m_bEventFlg;
	/** @brief イベント用のオブジェクトを格納する変数*/
	std::list<Object*> m_EventList;
	/** @brief お札を取っているかどうかの取得*/
	bool m_bOhuda;
	/** @brief ワープ先にいるかどうか
			   0でデフォルト 1でワープ先*/
	int m_nWarpPoint;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンス生成*/
	static SceneStage_1* GetInstance();
	/** @brief インスタンスの破棄*/
	static void Destroy();
	/** @brief コンストラクタ*/
	SceneStage_1();
	/** @brief デストラクタ*/
	~SceneStage_1()override;
	/** @brief 初期化*/
	void Init()override;
	/** @brief 終了処理*/
	void Uninit()override;
	/** @brief 更新処理*/
	void Update()override;
	/** @brief 描画*/
	void Draw() override;

	/** @brief 変数の設定だよー*/
	void SetBaseInfo(std::list<Object*>);
	/** @brief イベント用リストに中身が入っていたら消す*/
	void DeleteList();
	/** @brief イベント当たり判定とプレイヤーとの当たり判定の処理*/
	bool CollPlayer(Object* obj);
	/** @brief お札を取得したかを取得する*/
	bool GetOhuda();
	/** @brief お札を取得したかを設定する*/
	void SetOhuda(bool bOhuda);
	/** @brief どこのワープ先に移動したかを設定する*/
	void SetWarpPoint(int);
	/** @brief どこのワープ先にプレイヤーが存在するかを取得する*/
	int GetWarpPoint();
};

#endif