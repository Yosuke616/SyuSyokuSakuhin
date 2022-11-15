/**
* @file		SceneStage_1_Re.h
* @brief	ステージ1(Reloaded)の作成
* @author	志水陽祐
* @date		2022/7/30
*/

/**インクルードガード**/
#ifndef _STAGE_1_RE_H_
#define _STAGE_1_RE_H_

/**インクルード部**/
#include "Scene.h"
#include "mainApp.h"
#include "Object.h"

/**クラス定義**/
/**
* @brief	ステージ1
* @detail	裏表ラバーズ
*/
class SceneStage_1_Re : public Scene {
private:
	//メンバ変数
	/** @brief インスタンス*/
	static SceneStage_1_Re* m_pInstance;
	/** @brief 特殊当たり判定を保存する変数*/
	std::list<Object*> m_EventList;
	/** @brief お札を取得したかどうかのフラグ*/
	bool m_bOhuda;
	/** @brief ボスを倒しているかどうかのフラグ*/
	bool m_bBossBreak;
	/** @brief 一度だけブロックを壊すフラグ*/
	bool m_bBreak;
	/** @brief ボスのHPの管理*/
	int m_nBossHP;
	/** @brief ゴールフラグに触れたかの判定*/
	bool m_bGoalFlg;
	/** @brief イベント当たり判定に入ったかどうかのフラグ*/
	bool m_bEventTimeOut;
	/** @brief クリアまでの流れ*/
	bool m_bEventGoal;
	/** @brief 一度だけオブジェクトをチェンジするフラグ*/
	bool m_bFirstChange;
	/** @brief クリアに至るまでの秒数*/
	int m_nTime;

	//メンバ関数
	/** @brief タイムアウト用の物を作る*/
	void CreateTimeOut();
	/** @brief タイムアウトを動かす*/
	void EventTimeOut();
protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンス生成*/
	static SceneStage_1_Re* GetInstance();
	/** @brief インスタンスの破棄*/
	static void Destroy();
	/** @brief コンストラクタ*/
	SceneStage_1_Re();
	/** @brief デストラクタ*/
	~SceneStage_1_Re();
	/** @brief 初期化*/
	void Init() override;
	/** @brief 終了処理*/
	void Uninit() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 描画処理*/
	void Draw() override;

	/** @brief オブジェクトの描画を変えるための変数*/
	void ChangeObject();
	/** @brief 特殊当たり判定を保存するリスト*/
	void SetBaseInfo(std::list<Object*>);
	/** @brief ゴール用の当たり判定*/
	void GoalColl();
	/** @brief イベント用リストに中身が入っていたら消す*/
	void DeleteList();
	/** @brief イベントとの当たり判定の計算*/
	bool CollPlayer(Object* obj);
	/** @brief お札を取得したかの情報を送る*/
	bool GetOhuda();
	/** @brief お札を取得したかを設定する*/
	void SetOhuda(bool bOhuda);

	/** @brief ボスのHPを設定するための関数*/
	void SetBossHP();
	/** @brief ボスのHPを取得するための関数*/
	int GetBossHP();
};

#endif