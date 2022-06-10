/*
	全てのコンポーネントの元のクラス
	志水陽祐
	2022/6/9　作成
*/

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//インクルード部
#include "mainApp.h"
#include "ObjInfo.h"

#include "Object.h"

//列挙隊宣言
enum COMPONENT_UPDATE_ORDER {		//コンポーネントの更新順
	COM_UPDATE_1 = 0,				//これで大丈夫だよ
	COM_UPDATE_2,					
	COM_UPDATE_TRANSRATE,			//トランスフォームコンポーネント
	COM_UPDATE_COLLIDER,			//衝突処理
	COM_UPDATE_OFF_SCREEN,			//画面外処理
	COM_UPDATE_DRAW,				//描画用コンポーネント

	MAX_COMPONENT
};

//クラス定義
class Component {
private:

protected:

public:

};

#endif