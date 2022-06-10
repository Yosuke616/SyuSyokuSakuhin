/**ポーズ
 MenuManager.h
 メニューを管理するクラスにする
 YOSUKE SHIMIZU
 2022/6/3
 */

#ifndef _MENUMANAGER_H_
#define _MENUMANAGER_H_

 //インクルード部
#include "mainApp.h"
#include <list>

//前方宣言
class Object;

//列挙隊宣言
enum MENU_STATE
{
	STATE_NONE = 0,
	STATE_TITLE,
	STATE_OPTION,
	STATE_SELECT,
	STATE_GAMEOVER,

	STATE_MAX
};

//クラス定義
class MenuManager
{
	//=== メンバ変数
private:
	static MenuManager* m_pInstance;	// インスタンス用
	std::list<Object*>	m_MenuList;		// メニューリスト
	int m_nDelayCnt;					// 遅延時間
	int m_nCreateState;					// 作成するメニュー

public:
	// オプションフラグ
	static bool m_bOption;

	//=== メンバ関数
private:
	MenuManager();						// コンストラクタ
public:
	~MenuManager();						// デストラクタ
	void Start();						// 初期化
	void Update();						// 更新
	void Draw();						// 描画
	static MenuManager* GetInstance();	// インスタンス生成
	static void Destroy();				// インスタンス破棄

	// メニュー作成
	void Create(int state, int delayCnt = 1);

	// タイトル
	void CreateTitleMenu();

	// メニューリストの削除
	void DeleteMenu();

	// メニューオブジェクトの追加
	void AddMenu(Object* pMenuObject);

	// メニューリストの取得
	std::list<Object*>& GetMenuList();
};
#endif	// _MENUMANAGER_H_
