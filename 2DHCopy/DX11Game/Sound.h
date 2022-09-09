// CSound�N���X��` (Sound.h)
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

//-------- �}�N��
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
#define MAX_BUFFER_COUNT		3	// �X�g���[���Đ��p�o�b�t�@��
#define MAX_DUP					16	// �ő命�d�Đ���

enum SoundStatus {
	SS_STOP = 0,
	SS_PLAY,
	SS_PAUSE,

	MAX_SOUNDSTATUS
};

// BGM�N���X
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

// SE�N���X
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



// �T�E���h �}�l�[�W�� �N���X
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

	static void Init(void);					//	������
	static void Update(void);				//	�X�V
	static void Fin(void);					//	�I������

	static void Play(eBGM bgm);					//	�Đ�(BGM)
	static void Play(eSE se);					//	�Đ�(SE)

	static bool IsPlaying(eBGM bgm);			//	BGM���Đ����Ă��܂����H
	static bool IsPlaying(eSE se);				//	SE���Đ����Ă��܂����H

	static void Stop(eBGM bgm);					//	��~(BGM)
	static void Stop(eSE se);					//	��~(SE)

	static void Pause();						//	�ꎞ��~
	static void Resume();						//	�Đ��ĊJ

	static void SetVolume(float fVol);				//	�}�X�^�[�{�����[���ݒ�
	static void SetVolume(eBGM bgm, float fVol);	//	���ʐݒ�(�P�̂�BGM)
	static void SetVolume(eSE se, float fVol);		//	���ʐݒ�(�P�̂�SE)

	static float GetVolume();						//	�}�X�^�[�{�����[���̒l
	static float GetVolume(eBGM bgm);				//	BGM�̃{�����[���̒l
	static float GetVolume(eSE se);					//	SE�̃{�����[���̒l

	// �ǉ�
	static void SetVolume_BGM(float fVol);	//	BGM�{�����[���ݒ�
	static void SetVolume_SE(float fVol);	//	SE�{�����[���ݒ�
	static float GetVolume_BGM();			//	BGM�{�����[���̒l
	static float GetVolume_SE();			//	SE�{�����[���̒l
};