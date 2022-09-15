/**
* @file		TimeComponent.h
* @brief	時間をどんどん減らしていく
* @author	志水陽祐
* @data		2022/9/13
*/

/**インクルードガード**/
#ifndef _TIME_COMPONENT_H_
#define _TIME_COMPONENT_H_

/**インクルード部**/
#include "Component.h"

/**クラス定義**/
class CTime : public Component {
private:
	//メンバ変数
	/** @brief 何桁目かを保存する*/
	int m_nDigits;

	/** @brief 60秒に一回更新するための変数*/
	int m_nSixCnt;

	/** @brief 数字を減らすかどうかのフラグを持っている関数*/
	bool m_bSetTime;

	/** @brief かかっている時間*/
	int m_nTimeCnt;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	CTime();
	/** @brief デストラクタ*/
	~CTime()override;
	/** @brief 初期化*/
	void Start()override;
	/** @brief 更新*/
	void Update() override;
	/** @brief 自分が何桁目かを設定する関数*/
	void SetDigits(int digit);
	/** @brief タイムを変えるための変数をセットする*/
	void SetTime(bool main);
};

#endif