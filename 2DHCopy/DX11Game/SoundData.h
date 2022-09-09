/**
* @file		SoundData.h
* @brief	音楽の情報を管理するヘッダーファイル
* @author	志水陽祐
* @date		2022/9/8
*/

/**インクルードガード**/
#ifndef _SOUND_DATA_H_
#define _SOUND_DATA_H_

/**列挙体宣言**/
/** @brief BGM*/
enum eBGM {
	TITLE_BGM = 0,

	MAX_BGM
};

/** @brief SE*/
enum eSE {
	SELECT_SE = 0,
	ATTACK_VOICE,

	MAX_SE
};

#endif