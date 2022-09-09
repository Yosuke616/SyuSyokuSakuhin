// CSoundクラス定義 (Sound.h)
//
#pragma once

#include "SoundData.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include <objbase.h>
#include <vector>
#include <mmreg.h>

#include <comip.h>
#include <comdef.h>

#include <xaudio2.h>

#include <initguid.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>

//-------- マクロ
#ifdef COMPTR
#undef COMPTR
#endif
#define COMPTR(type) _com_ptr_t<_com_IIID<type,&__uuidof(type)>>
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)			if(x){x->Release();x=nullptr;}
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if(x){delete[]x;x=nullptr;}
#endif
#ifndef SAFE_DESTROYVOICE
#define SAFE_DESTROYVOICE(x)	if(x){x->DestroyVoice();x=nullptr;}
#endif
#define MAX_BUFFER_COUNT		3	// ストリーム再生用バッファ数
#define MAX_DUP					16	// 最大多重再生数

enum SoundStatus {
	SS_STOP = 0,
	SS_PLAY,
	SS_PAUSE,

	MAX_SOUNDSTATUS
};

// BGMクラス
struct CSoundStream
{
private:
	IXAudio2SourceVoice*	m_pSourceVoice;
	COMPTR(IMFSourceReader)	m_reader;
	DWORD					m_currentStreamBuffer;
	std::vector<BYTE>		m_buffers[MAX_BUFFER_COUNT];
	bool					m_endOfStream;
	SoundStatus				m_status;

public:
	CSoundStream();
	virtual ~CSoundStream();
	HRESULT Open(IXAudio2* pXAudio2, LPCWSTR pszPath);
	void Update();
	void Close();
	void Play();
	void Stop();
	void Pause();
	void Resume();
	bool IsPlaying();
	void SetVolume(float fVol);
	float GetVolume();
};

// SEクラス
struct CSoundEffect
{
public:
	IXAudio2SourceVoice*	m_pSourceVoice[MAX_DUP];
	SoundStatus				m_status[MAX_DUP];
	std::vector<BYTE>		m_buffer;
	DWORD					m_bufferLength;

public:
	CSoundEffect();
	virtual ~CSoundEffect();
	HRESULT Open(IXAudio2* pXAudio2, LPCWSTR pszPath);
	void Close();
	void Play();
	void Stop(int n = -1);
	void Pause();
	void Resume();
	bool IsPlaying();
	void SetVolume(float fVol);
	float GetVolume();
};



// サウンド マネージャ クラス
class CSound
{
private:
	static IXAudio2*				m_pXAudio2;
	static IXAudio2MasteringVoice*	m_pMasteringVoice;
	static CSoundStream*			m_pBgm;
	static CSoundEffect*			m_pSe;
	//add
	static IXAudio2SubmixVoice*		m_pSubmixBGM;
	static IXAudio2SubmixVoice*		m_pSubmixSE;

public:
	CSound(void);
	virtual ~CSound(void);

	static void Init(void);					//	初期化
	static void Update(void);				//	更新
	static void Fin(void);					//	終了処理

	static void Play(eBGM bgm);					//	再生(BGM)
	static void Play(eSE se);					//	再生(SE)

	static bool IsPlaying(eBGM bgm);			//	BGMを再生していますか？
	static bool IsPlaying(eSE se);				//	SEを再生していますか？

	static void Stop(eBGM bgm);					//	停止(BGM)
	static void Stop(eSE se);					//	停止(SE)

	static void Pause();						//	一時停止
	static void Resume();						//	再生再開

	static void SetVolume(float fVol);				//	マスターボリューム設定
	static void SetVolume(eBGM bgm, float fVol);	//	音量設定(単体のBGM)
	static void SetVolume(eSE se, float fVol);		//	音量設定(単体のSE)

	static float GetVolume();						//	マスターボリュームの値
	static float GetVolume(eBGM bgm);				//	BGMのボリュームの値
	static float GetVolume(eSE se);					//	SEのボリュームの値

	// 追加
	static void SetVolume_BGM(float fVol);	//	BGMボリューム設定
	static void SetVolume_SE(float fVol);	//	SEボリューム設定
	static float GetVolume_BGM();			//	BGMボリュームの値
	static float GetVolume_SE();			//	SEボリュームの値
};