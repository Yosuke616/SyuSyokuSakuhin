/**
* @file		ScoreComponent.h
* @brief	スコアをカウントし、テクスチャの割り当てを変更していく
* @detail	スコアを増やしたりするのはこのコンポーネントでいいと思うが管理はマネージャにさせるべきだな
* @author	志水陽祐			
* @data		2022/9/10
*/

/**インクルードガード**/
#ifndef _SCORE_COMPONENT_H_
#define _SCORE_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**定数定義**/
#define MAX_SCORE (5)

/**クラス定義**/
/**
* @brief	スコアを増やし数字を加えるコンポーネント
* @detail	マネージャーに値を渡したりする
*/
class CScore : public Component {
private:
	//メンバ変数
	/** @brief 自分が何桁目かを保存する変数*/
	int m_nDigits;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CScore();
	/** @brief デストラクタ*/
	~CScore() override;
	/** @brief 初期化*/
	void Start() override;
	/** @brief 更新処理*/
	void Update() override;
	/** @brief 自分が何桁目か設定する関数*/
	void SetDigits(int digit);
};

#endif