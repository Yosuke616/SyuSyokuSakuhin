/**
* @file		InputManager.h
* @brief	コントローラーやキーボード操作できるようにする
* @author	志水陽祐
* @date		2022/6/30
*/

//インクルードガード
#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

//インクルード部
#include "mainApp.h"
#define   DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//定数定義
#define KEY_MAX (256)

/**列挙体宣言**/
enum MOUSE_BUTTON {
	MOUSE_L,	//左クリック
	MOUSE_R,	//右クリック
	MOSUE_C		//ホイール
};

//クラス定義
class InputManager {
private:
	//メンバ変数
	/** @brief インスタンス用*/
	static InputManager* m_pInstance;
	/** @brief 現在のキーボード入力*/
	unsigned char m_CurrentKeyState[KEY_MAX];
	/** @brief 前フレームのキーボード入力*/
	unsigned char m_PrevKeyState[KEY_MAX];
	/** @brief マウスの現在の入力情報*/
	DIMOUSESTATE m_CurrentMouseState;
	/** @brief マウスの1フレーム前の入力情報*/
	DIMOUSESTATE m_PrevMouseState;
	/** @brief マウス座標*/
	XMFLOAT2 m_MousePos;

	//メンバ関数
	/** @brief キーボード初期化*/
	HRESULT InitKeyBoard();
	/** @brief キーボード更新*/
	void UpdateKeyBoard();
	/** @brief マウスの初期化*/
	HRESULT InitMouse();
	/** @brief マウスの更新*/
	void UpdateMouse();

protected:
	//メンバ変数

	//メンバ関数
	/** @brief コンストラクタ*/
	InputManager();

public:
	//メンバ変数

	//メンバ関数
	/** @brief インスタンス生成*/
	static InputManager* Instance();
	/** @brief デストラクタ*/
	~InputManager();
	/** @brief 初期化*/
	HRESULT Init();
	/** @brief 終了*/
	void Uninit();
	/** @brief 更新*/
	void Update();
	/** @brief 描画*/
	void Draw();

	/** @brief キーボードのプレス状態を取得*/
	bool GetKeyPress(unsigned char key);
	/** @brief キーボードのトリガー状態を取得*/
	bool GetKeyTrigger(unsigned char key);
	/** @brief キーボードのリリース状態を取得*/
	bool GetKeyRelease(unsigned char key);

	/** @brief マウスのプレス状態を取得*/
	bool GetMouseButton(MOUSE_BUTTON Btn);
	/** @brief マウスのトリガー状態を取得*/
	bool GetMouseTrigger(MOUSE_BUTTON Btn);
	/** @brief マウスのリリース状態を取得*/
	bool GetMouseRelease(MOUSE_BUTTON Btn);

	/** @brief マウスの座標の取得*/
	XMFLOAT2 GetMousePos() const;
	/** @brief マウスの移動量の取得*/
	XMFLOAT2 GetMouseVelocity() const;
};

#endif