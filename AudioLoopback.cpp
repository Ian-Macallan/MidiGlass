#include "stdafx.h"

#include <Windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <time.h>
#include <iostream>

#include "AudioLoopback.h"

using namespace std;
    
#pragma comment(lib, "Winmm.lib")
    
//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
static char szErrorText [ MAX_PATH * 4 ]	= "";
static char szFilename  [ MAX_PATH ]		= "";

static BOOL		bAudioLoopbackDone				= FALSE;
static HMMIO	mmioFile						= NULL;
static BOOL		bAudioLoopbackFinished			= FALSE;


// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC		10000000
#define REFTIMES_PER_MILLISEC	10000
    
#define EXIT_ON_ERROR(hres)		if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)		if ((punk) != NULL)  { (punk)->Release(); (punk) = NULL; }
    
const CLSID CLSID_MMDeviceEnumerator		= __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator			= __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient					= __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient			= __uuidof(IAudioCaptureClient);

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
const char *GetLoopBackError ( )
{
	return szErrorText;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void SetLoopBackFilename ( const char *pFilename )
{
	strcpy_s ( szFilename, sizeof(szFilename), pFilename );
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void setLoopBackDone()
{
	bAudioLoopbackDone	= TRUE;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
int startAudioLoopBack()
{
    bAudioLoopbackDone		= FALSE;
	bAudioLoopbackFinished	= FALSE;

    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    
    // Create file
    MMIOINFO mi = { 0 };
    mmioFile = mmioOpen ( 
        // some flags cause mmioOpen write to this buffer
        // but not any that we're using
        (LPSTR)szFilename,
        &mi,
        MMIO_WRITE | MMIO_CREATE
    );
    
    if (NULL == mmioFile)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioOpen(\"%s\", ...) failed. wErrorRet == %u", szFilename, GetLastError());
        return E_FAIL;
    }
    
    MyAudioSink AudioSink;
    RecordAudioStream(&AudioSink);
    
    mmioClose(mmioFile, 0);
    
    CoUninitialize();

	bAudioLoopbackFinished	= TRUE;

    return 0;
}
    
//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
HRESULT MyAudioSink::CopyData (BYTE* pData, UINT32 NumFrames, BOOL* pDone, WAVEFORMATEX* pwfx, HMMIO mmioFile )
{
    HRESULT hRes = S_OK;
    
    if ( 0 == NumFrames )
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "IAudioCaptureClient::GetBuffer said to read 0 frames\n");
        return E_UNEXPECTED;
    }
    
    LONG lBytesToWrite = NumFrames * pwfx->nBlockAlign;
	// #pragma prefast(suppress: __WARNING_INCORRECT_ANNOTATION, "IAudioCaptureClient::GetBuffer SAL annotation implies a 1-byte buffer")
    LONG lBytesWritten = mmioWrite(mmioFile, reinterpret_cast<PCHAR>(pData), lBytesToWrite);
    if ( lBytesToWrite != lBytesWritten )
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioWrite wrote %u bytes : expected %u bytes", lBytesWritten, lBytesToWrite);
        return E_UNEXPECTED;
    }
    
    static int CallCount = 0;
	sprintf_s ( szErrorText, sizeof(szErrorText), "CallCount = %d NumFrames %d", CallCount, NumFrames );
    
    return S_OK;
}
    
//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
HRESULT RecordAudioStream(MyAudioSink* pMySink)
{
    HRESULT					hRes;
    REFERENCE_TIME			hnsRequestedDuration		= REFTIMES_PER_SEC;
    REFERENCE_TIME			hnsActualDuration			= 0;
    UINT32					bufferFrameCount			= 0;
    UINT32					numFramesAvailable			= 0;
    IMMDeviceEnumerator*	pEnumerator					= NULL;
    IMMDevice*				pDevice						= NULL;
    IAudioClient*			pAudioClient				= NULL;
    IAudioCaptureClient*	pCaptureClient				= NULL;
    WAVEFORMATEX*			pwfx						= NULL;
    UINT32					packetLength				= 0;
    
    BYTE*					pData						= NULL;
    DWORD					flags						= NULL;
    
    MMCKINFO				ckRIFF						= { 0 };
    MMCKINFO				ckData						= { 0 };
    
    hRes = CoCreateInstance( CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)& pEnumerator);
    EXIT_ON_ERROR(hRes)
    
    hRes = pEnumerator->GetDefaultAudioEndpoint ( eRender, eConsole, &pDevice);
    EXIT_ON_ERROR(hRes)
    
    hRes = pDevice->Activate( IID_IAudioClient, CLSCTX_ALL, NULL, (void**)& pAudioClient);
    EXIT_ON_ERROR(hRes)
    
    hRes = pAudioClient->GetMixFormat(&pwfx);
    EXIT_ON_ERROR(hRes)
    
    hRes = pAudioClient->Initialize ( AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK, hnsRequestedDuration, 0, pwfx, NULL);
    EXIT_ON_ERROR(hRes)
    
    // Get the size of the allocated buffer.
    hRes = pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hRes)
    
    hRes = pAudioClient->GetService ( IID_IAudioCaptureClient, (void**)& pCaptureClient);
    EXIT_ON_ERROR(hRes)
    
    hRes = WriteWaveHeader((HMMIO)mmioFile, pwfx, &ckRIFF, &ckData);
    if (FAILED(hRes))
	{
        // WriteWaveHeader does its own logging
        return hRes;
    }
    
    // Calculate the actual duration of the allocated buffer.
    hnsActualDuration = (REFERENCE_TIME)((double)REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec);
    
    hRes = pAudioClient->Start();  // Start recording.
    EXIT_ON_ERROR(hRes)
    
    // Each loop fills about half of the shared buffer.
    while (bAudioLoopbackDone == FALSE)
    {
        // Sleep for half the buffer duration.
        Sleep((DWORD) hnsActualDuration / (DWORD) REFTIMES_PER_MILLISEC / 2);
    
        hRes = pCaptureClient->GetNextPacketSize(&packetLength);
        EXIT_ON_ERROR(hRes)
    
        while (packetLength != 0)
        {
            // Get the available data in the shared buffer.
            hRes = pCaptureClient->GetBuffer ( &pData, &numFramesAvailable, &flags, NULL, NULL);
            EXIT_ON_ERROR(hRes)
    
            if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
            {
                pData = NULL;  // Tell CopyData to write silence.
            }
    
            // Copy the available capture data to the audio sink.
            hRes = pMySink->CopyData ( pData, numFramesAvailable, &bAudioLoopbackDone, pwfx, (HMMIO)mmioFile);
            EXIT_ON_ERROR(hRes)
    
            hRes = pCaptureClient->ReleaseBuffer(numFramesAvailable);
            EXIT_ON_ERROR(hRes)
    
            hRes = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hRes)
        }
    }
    
Exit:
	if ( pAudioClient != NULL )
	{
		hRes = pAudioClient->Stop();  // Stop recording.
	}    

	hRes = FinishWaveFile((HMMIO)mmioFile, &ckData, &ckRIFF);
    
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator)
    SAFE_RELEASE(pDevice)
    SAFE_RELEASE(pAudioClient)
    SAFE_RELEASE(pCaptureClient)
    
    return hRes;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
HRESULT WriteWaveHeader(HMMIO mmioFile, LPCWAVEFORMATEX pwfx, MMCKINFO* pckRIFF, MMCKINFO* pckData)
{
    MMRESULT result;

    // make a RIFF/WAVE chunk
    pckRIFF->ckid = MAKEFOURCC('R', 'I', 'F', 'F');
    pckRIFF->fccType = MAKEFOURCC('W', 'A', 'V', 'E');

    result = mmioCreateChunk(mmioFile, pckRIFF, MMIO_CREATERIFF);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioCreateChunk(\"RIFF/WAVE\") failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    // make a 'fmt ' chunk (within the RIFF/WAVE chunk)
    MMCKINFO chunk;
    chunk.ckid = MAKEFOURCC('f', 'm', 't', ' ');
    result = mmioCreateChunk(mmioFile, &chunk, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioCreateChunk(\"fmt \") failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    // write the WAVEFORMATEX data to it
    LONG lBytesInWfx = sizeof(WAVEFORMATEX) + pwfx->cbSize;
    LONG lBytesWritten =
        mmioWrite(
            mmioFile,
            reinterpret_cast<PCHAR>(const_cast<LPWAVEFORMATEX>(pwfx)),
            lBytesInWfx
        );
    if (lBytesWritten != lBytesInWfx)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioWrite(fmt data) wrote %u bytes; expected %u bytes", lBytesWritten, lBytesInWfx);
        return E_FAIL;
    }

    // ascend from the 'fmt ' chunk
    result = mmioAscend(mmioFile, &chunk, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioAscend(\"fmt \" failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    // make a 'fact' chunk whose data is (DWORD)0
    chunk.ckid = MAKEFOURCC('f', 'a', 'c', 't');
    result = mmioCreateChunk(mmioFile, &chunk, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioCreateChunk(\"fmt \") failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    //	write (DWORD)0 to it
    //	this is cleaned up later
    DWORD frames = 0;
    lBytesWritten = mmioWrite(mmioFile, reinterpret_cast<PCHAR>(&frames), sizeof(frames));
    if (lBytesWritten != sizeof(frames))
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioWrite(fact data) wrote %u bytes; expected %u bytes", lBytesWritten, (UINT32)sizeof(frames));
        return E_FAIL;
    }

    // ascend from the 'fact' chunk
    result = mmioAscend(mmioFile, &chunk, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioAscend(\"fact\" failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    // make a 'data' chunk and leave the data pointer there
    pckData->ckid = MAKEFOURCC('d', 'a', 't', 'a');
    result = mmioCreateChunk(mmioFile, pckData, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioCreateChunk(\"data\") failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    return S_OK;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
HRESULT FinishWaveFile(HMMIO mmioFile, MMCKINFO* pckRIFF, MMCKINFO* pckData)
{
    MMRESULT result;

    result = mmioAscend(mmioFile, pckData, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioAscend(\"data\" failed: MMRESULT = 0x%08x", result);
        return E_FAIL;
    }

    result = mmioAscend(mmioFile, pckRIFF, 0);
    if (MMSYSERR_NOERROR != result)
	{
        sprintf_s ( szErrorText, sizeof(szErrorText), "mmioAscend(\"RIFF/WAVE\" failed: MMRESULT = 0x%08x", result);
		return E_FAIL;
    }

    return S_OK;
}
