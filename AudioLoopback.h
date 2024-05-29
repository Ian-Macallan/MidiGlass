#pragma once

#include <mmsystem.h>

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
class MyAudioSink
{
	public:
		HRESULT CopyData(BYTE* pData, UINT32 NumFrames, BOOL* pDone, WAVEFORMATEX* pwfx, HMMIO hFile);
};
    
//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
extern const char *GetLoopBackError ( );
extern void SetLoopBackFilename ( const char *pFilename );
extern int startAudioLoopBack();
extern void setLoopBackDone();

HRESULT WriteWaveHeader(HMMIO hFile, LPCWAVEFORMATEX pwfx, MMCKINFO* pckRIFF, MMCKINFO* pckData);
HRESULT FinishWaveFile(HMMIO hFile, MMCKINFO* pckRIFF, MMCKINFO* pckData);
HRESULT RecordAudioStream(MyAudioSink* pMySink);
    
