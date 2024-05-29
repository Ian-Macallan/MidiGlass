//
/////////////////////////////////////////////////////////////////////////////////////////
// MidiWave.h: interface for the CMidiWave class.
//
/////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIWAVE_H__09A065A6_3050_11D3_8149_444553540000__INCLUDED_)
#define AFX_MIDIWAVE_H__09A065A6_3050_11D3_8149_444553540000__INCLUDED_

#include "MidiFile.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMidiWave  
{
public:
	DWORD Play ( const char *pWaveFile, HWND hWnd );
	DWORD Save();
	DWORD Stop();
	DWORD Record (	const char *pMidifile, int bSkipMode, 
					int wMidiDevice, const char *pWavedir,
					HWND hWnd );
	CMidiWave();
	char * GetShort ( const char *pString );
	virtual ~CMidiWave();

protected:
	CString				m_WaveDirectory;
	UINT				m_wDeviceID;
	DWORD				m_dwReturn;
	MCI_OPEN_PARMS		m_mciOpenParms;
	MCI_RECORD_PARMS	m_mciRecordParms;
	MCI_SAVE_PARMS		m_mciSaveParms;
	MCI_PLAY_PARMS		m_mciPlayParms;
	MCI_WAVE_SET_PARMS	m_mciWaveSetParms;
	HWND				m_hWnd;
	CMidiFile			*m_pMidiFile;
};

#endif // !defined(AFX_MIDIWAVE_H__09A065A6_3050_11D3_8149_444553540000__INCLUDED_)
