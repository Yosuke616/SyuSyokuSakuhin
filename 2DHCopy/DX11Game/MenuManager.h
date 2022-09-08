/**
* @file		MenuManager.h
* @brief	メニューを操作したりするマネージャー
* @author	志水陽祐
* @date		2022/7/31
*/

/**インクルードガード**/
#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

/**インクルード部**/
#include "mainApp.h"
#include <list>

/**前方宣言**/
class Object;

/**列挙体宣言**/
enum MENU_STATE {
	STATE_NONE = 0,
	TITLE_STATE,
	SELECT_STATE,
	PAUSE_STATE,
	OPTION_STATE,
	GAMECLEAR_STATE,
	MISS_STATE,
	GAMEOVER_STATE,

	MAX_MENU
};

/**クラス定義**/
/**
* @brief	メニューの状態を管理するクラス
* @detail	シーンごとに用意するメニューを変えたりする
*/
class MenuManager {
private:
	//メンバ変数
	/** @brief インスタンス*/
	static MenuManager* m_pInstance;
	/** @brief メニューリスト*/
	std::list<Object*> m_MenuList;
	/** @brief メニューリストの中身をいじるためのイテレーター*/
	std::list<Object*>::iterator m_itr_Menu;
	/** @brief 遅延時間*/
	int m_nDelayCnt;
	/** @brief 作成するメニュー*/
	int m_nCreateMenu;
	/** @brief ステージを表すオブジェクトを管理するためのリスト*/
	std::list<Object*> m_StageObjList;
	/** @brief ステージオブジェクトを触るためのイテレーター*/
	std::list<Object*>::iterator m_itr_Stage;
	/** @brief  ステージのオブジェクトが移動しきったら再び操作できるようにするフラグ*/
	/** @detail trueで操作できない*/
	bool m_bRestart;
	/** @brief どれだけ移動したかを保存する変数*/
	float m_fMoveObj;
	/** @brief 透明から半透明にするための変数*/
	float m_fBlackAlpha;
	/** @brief ある程度時間がたったら初めからやり直す*/
	int m_nChangeCnt;

	//メンバ関数
	/** @brief コンストラクタ*/
	MenuManager();
	/** @brief 選ばれているボタンの色を変えたり、
			   ボタンを押されたらシーン遷移したりする関数*/
	void SelectButton();
	/** @brief 決定ボタンが押された場合の処理*/
	void PushButton();
	/** @brief ステージセレクトを制御するための関数
			   選んだステージに入ったりデータをロードしたりする*/
	void StageSelect();
	/** @brief 選択されたステージを読み込む関数*/
	void StageIN();
	/** @brief ミスしたときの処理*/
	void MissMenu();
	/** @brief ポーズメニューの処理*/

protected:
	//メンバ変数

	//メンバ関数

public:
	//メンバ変数
	/** @brief オプションフラグ*/
	static bool m_bOption;

	//メンバ関数
	/** @brief デストラクタ*/
	~MenuManager();
	/** @brief 初期化*/
	void Start();
	/** @brief 更新処理*/
	void Update();
	/** @brief 描画処理*/
	void Draw();
	/** @brief インスタンス生成*/
	static MenuManager* GetInsutance();
	/** @brief インスタンス破棄*/
	static void Destroy();
	/** @brief メニュー作成*/
	void Create(int state,int delayCnt = 1);
	/** @brief メニューリストの削除*/
	void DeleteMenu();
	/** @brief メニューオブジェクトの追加*/
	void AddMenu(Object* pMenuObject);
	/** @brief メニューリストの取得*/
	std::list<Object*>& GetMenuList();
	/** @brief 作るメニューの番号を取得する*/
	int GetCreateMenu();

	/**作りたいメニューをここから下に追加していく**/
	/** @brief タイトルメニュー*/
	void CreateTitleMenu();
	/** @brief ポーズメニュー*/
	void CreatePauseMenu();
	/** @brief ステージセレクトメニュー*/
	void CreateSelectMenu();
	/** @brief ミスメニュー*/
	void CreateMissMenu();
	/** @brief ゲームオーバーメニュー*/
	void CreateGameOverMenu();
};

#endif