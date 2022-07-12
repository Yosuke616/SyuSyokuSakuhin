/**
* @file		Animation.h
* @brief	アニメーションを管理するクラス
* @author	志水陽祐
* @date		2022/7/8
*/

//インクルードガード
#ifndef _ANIMATION_H_
#define  _ANIMATION_H_

//インクルード部
#include "Component.h"
#include <unordered_map>

//前方宣言
class CDraw3D;

//クラス定義
class Animation {
private:
	//メンバ変数
	/** @brief アニメーション開始フレーム*/
	std::unordered_map<int, double> m_dStart;
	/** @brief アニメーション終了フレーム*/
	std::unordered_map<int, double> m_dEnd;
	/** @brief 再生速度*/
	std::unordered_map<int, double> m_dSpeed;
	/** @brief ループ再生*/
	std::unordered_map<int, bool> m_bLoop;
	/** @brief アニメーションの優先度*/
	std::unordered_map<int, int> m_nOrder;
	/** @brief Play関数が呼ばれているか*/
	std::unordered_map<int, bool> m_bEnd;
	
	/** @brief 現在のアニメーションの時間*/
	double m_dCurrentAnimTime;
	/** @brief 現在のアニメーションの状態*/
	int m_nCurrentAnimState;
	/** @brief ture ループが終われば0番目のアニメーション false アニメーションが終われば終了*/
	bool m_bIdle;
	/** @brief 再生フラグ*/
	bool m_bAnimation;
	/** @モデル*/
	CDraw3D* m_pDraw3D;

	//メンバ関数

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	Animation();
	/** @brief デストラクタ*/
	~Animation();
	/** @brief 更新処理*/
	void Update();
	/** @brief 描画処理*/
	void Draw();

	/** *brief モデルのセット*/
	void SetModel(CDraw3D* model);

	/** @brief 再生*/
	void Play(int AnimState);
	/** @brief停止*/
	void Stop();

	/** @brief アニメーションの追加*/
	void AddAnimation(int AnimID,double start,double end,double loopStartFrame,bool loop);

	/** @brief ループ再生の設定*/
	void SetLoop(int AnimID,bool loop);
	/** @brief 再生速度の設定*/
	void SetPlaySpeed(int AnimID,double speed);
	/** @brief アニメーションが終わったかのフラグ*/
	bool AnimEnd(int AnimID);
	/** @常に常時*/
	void AlwaysAnimation(bool always);
};

#endif