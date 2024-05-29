// MidiFile.h: interface for the CMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIFILE_H__BB233944_AE20_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIFILE_H__BB233944_AE20_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>

#include "MidiDefs.h"

#include "MidiClass.h"
#include "MidiTrack.h"
#include "MidiChannel.h"
#include "MidiPlayerChunk.h"
#include "MidiInputChunk.h"
#include <mmsystem.h>

class CMidiFile : public CMidiClass  
{
	protected:
		CString					m_Web_Address;
		CString					m_E_Mail;

		//		Value is 4/4 4/2
		unsigned char			m_cMidiPieceNN;
		//		Value is 4/4 4/2
		unsigned char			m_cMidiPieceDD;

		//		Three Bytes
		//		The default Value is 600.000 (it means 100)
		//		Note that a value of 500000 is 0,5 second
		//		by note that has a length of 120 (1:000)
		//		9, 39, 192
		unsigned char			m_cMidiPieceTempo [ 3 ];

		unsigned short			m_iMaximumMesure;
		bool					m_bMciPlaying;

		//	Immediate Chunk is for applying modification during playback
		//	It use a bool to say there is an immediate chunk to treat
		//	It can be programs / controller
		CMidiPlayerChunk		m_ImmediateChunk;
		bool					m_bImmediateChunk;

		int						m_iMidiDevice;
		bool					m_bDoSoundOff;
		bool					m_bSkipToFirstNote;
		bool					m_bSkippingDone;

		boolean					m_bEditChange;

		unsigned				m_iSysEx;
		int						m_bAddStartOfTrack;
		int						m_bAddEndOfTrack;
		int						m_bBankSelectLow;
		int						m_bBankSelectHigh;
		int						m_bFilter;
		int						m_bCorrectStartOfTrack;

		unsigned long			m_iLLSystemTime;
		bool					m_bLLPlayAndLoop;
		unsigned long			m_iLLSpeedMul;
		unsigned long			m_iLLSpeedDiv;

		//	Start Of Media
		CMidiPlayerChunk		* m_pLLStartChunk;

		//	Current Position in the media
		CMidiPlayerChunk		* m_pLLCurrentChunk;

		//	Absolute position of the current chunk
		unsigned long			m_iLLCurrentHMSm;

		//	Resuming position
		unsigned long			m_iLLStartingHMSm;

		//
		unsigned				m_bLLState;
		unsigned				m_bLLInputState;
		MMRESULT				m_wLLTimerID;
		bool					m_bLLTimerOk;
		UINT					m_wLLTimerRes;
		bool					m_bLLDeviceOk;
		UINT					m_wLLDeviceID;
		MIDIOUTCAPS				m_LLMidiOutCaps;
		MMRESULT				m_wLLResult;
		HMIDIOUT				m_hLLMidiOut;
		UINT					m_wLLReturn;
		HWND					m_hLLNotifyWnd;

		bool					m_bLLDeviceInpOk;
		UINT					m_wLLDeviceInpID;
		HMIDIIN					m_hLLMidiInp;
		MIDIINCAPS				m_LLMidiInpCaps;
		int						m_iMidiInpDevice;
		MIDIHDR					m_LLMidiHdr;

		CMidiInputChunk			* m_pLLStartInputChunk;
		CMidiInputChunk			* m_pLLCurrentInputChunk;
		int						m_iNumberOfInputChunk;

		bool					m_bPlayAndLoop;

		bool					m_bMidiDeviceOk;
		UINT					m_wMidiDeviceID;
		DWORD					m_dwMidiReturn;

		MCI_OPEN_PARMS			m_MidiOpenParms;
		MCI_PLAY_PARMS			m_MidiPlayParms;
		MCI_STATUS_PARMS		m_MidiStatusParms;

		unsigned long			m_iMaximumHMSm;

		CMidiList				* m_pTextListContext;
		unsigned int			m_iTextTrackContext;
		CMidiChannel			m_MidiChannels [ MAX_CHANNELS ];
		CMidiTrack				m_MidiFileTrack [ MAX_TRACKS ];

		unsigned char			*m_pMidiFileHeader;
		unsigned long			m_iMidiFileLength;
		unsigned long			m_iMidiFileSize;
		short int				m_iMidiFileFormat;
		short int				m_iMidiFileMaxTracks;
		short int				m_iMidiFileUsedTracks;
		unsigned char			m_iMidiFileDivisionLo;
		unsigned char			m_iMidiFileDivisionHi;

		char					*m_pMidiFilename;
		FILE					*m_hMidiFile;

		char					*m_pSoundFont;
		int 					m_iSoundFont;

		unsigned char			*m_pMidiFileBuffer;
		unsigned char			*m_pMidiFilePosition;

		char					*m_pMidiTracename;
		FILE					*m_hTraceFile;

		CCriticalSection		m_CriticalSectionPlay;
		CCriticalSection		m_CriticalSectionRecord;

	public:
		CMidiFile();
		CMidiFile ( char *pBuffer, int iLength );
		CMidiFile( const char *pFilename);
		virtual ~CMidiFile();

		void SaveInputChunk(const char *pFilename);
		int GetNumberOfInputChunk();
		void DeleteInputChunkList();
		void SetInternetAddresses();
		CString GetEMail();
		void SetWebAddress ( const char *pAddress );
		CString GetWebAddress();
		UINT LLPlayRoutine ( bool bStart, bool bTimer, bool bSysex );
		BOOL UnLock();
		BOOL Lock();
		BOOL UnLockInput();
		BOOL LockInput();
		void MoveEventFromTrackToTrack(unsigned int iType, int iFrom, int iTo);
		char * GetMidiPathName ( ) const;

		void CorrectNoteDuration();
		void SetTrackChannel ( int iTrack, unsigned char cChannel );
		CMidiChannel * GetChannelAddress ( int iChannel );
		void SetMaximumMesure();
		unsigned short GetMaximumMesure ( ) const;
		void ComputeNoteDuration ();

		const char *GetInstrument ( int iTrack );
		void SetInstrumentNameOnTrack(int iTrack, const char *szText);
		void SetTrackNameOnTrack( int iTrack, const char *szText );
		void SetChannelVolume ( int iChannel, unsigned char cVolume );
		void SetVolumeOnTrack ( int iTrack, unsigned char cVolume );
		bool GetChannelVolume ( int iChannel, unsigned char *cVolume );
		//	Search Channel in the track : there must be only one channel
		unsigned char GetTrackChannel ( int iTrack ) const;
		void SetProgramOnTrack ( unsigned iTrack, unsigned char iProgram );
		void SetChannelProgram ( int iChannel, unsigned char iProgram );

		//	Immediate Chunk is for applying modification during playback
		//	It use a bool to say there is an immediate chunk to treat
		//	It can be programs / controller
		void SetImmediateChunk ( DWORD dwChunk );

		const char * GetChannelProgram ( int iChannel );

		unsigned char GetChannelProgramNum ( int iChannel );
		void SetChannelMuteState ( int iX, bool bMute );
		bool GetChannelMuteState ( int iX ) const;
		void SetMuteState ( int iX, bool bMute );
		bool GetMuteState ( int iX ) const;
		void MoveEventToTrack ( unsigned iType, int iTrack );

		void CountEvents();
		void UpdateTrackNumber();

		void SetMidiDevice ( int iDevice );
		void SetSoundFont ( const char *pSoundFont, int iSoundFont );
		void SetMidiInpDevice ( int iDevice );


		void RemoveEventCodeSubCode ( unsigned int iType, unsigned char cSubType );
		void RemoveEventCode ( unsigned int iType, 
			bool bSkipTrack0 = false, bool bTrack0Only = false,
			bool bSkipFirstEvent = false, bool bFirstEventOnly = false );
		void RemoveSystemExclusive();
		void CorrectAll (	unsigned iSysEx, int iAddStartOfTrack, int iAddEndOfTrack,
							int	iBankSelectLow, int iBankSelectHigh, int iFilter,
							int iCorrectStartOfTrack );

		void AddGSReset();
		void AddGMReset();
		void AddXGReset();

		void FilterEventsTracks();


		//	Low Level Play
		void LLSendMCINotification ( WPARAM wParam, LPARAM lParam );
		void LLKillEventTimer();
		void LLSetSkippingMode ( bool bSkip );
		MIDIHDR *LLGetHeaderAddress ( );
		DWORD LLResetChunk ( int iChannel );
		DWORD LLSoundOffChunk ( int iChannel );
		UINT LLSoundOff();
		UINT LLReset();
		void LLSetErrorText ( unsigned iError );
		void LLSetErrorInpText ( unsigned iError );
		UINT LLPlayAndLoop ( HWND hWnd = NULL );
		UINT LLOneShotTimer ( long iDelta );
		UINT LLSetSpeed (unsigned iSpeed );
		UINT LLBeginTimer();
		UINT LLEndTimer();
		UINT LLClose();
		UINT LLOpen ( boolean bResume );
		UINT LLResume ();
		UINT LLPause();
		UINT LLPlayChunk( bool bNoTimer, bool bNoNotes );
		bool LLIsPlaying() const;
		bool LLIsRecording() const;
		bool LLIsPaused() const;
		bool LLIsStopped() const;
		UINT LLStop();
		UINT LLPlay ( HWND hWnd = NULL );
		UINT LLPlayTracks ( bool bStart, bool bTimer, bool bSysex );
		UINT LLSlide (unsigned long iSeconds );
		unsigned long LLGetTime() const;
		void LLFillChunkList();

		UINT LLInpOpen ();
		UINT LLInpReset();
		UINT LLInpClose();
		UINT LLInpRecord ( HWND hWnd = NULL );
		UINT LLInpStart ( );
		UINT LLInpStop();
		void LLInpRecordChunk ( MIDIHDR *pSysex, DWORD_PTR dwParam2 );
		void LLInpRecordChunk ( DWORD_PTR dwChunk, DWORD_PTR dwParam2 );


		//
		void SetMaximumHMSm();
		unsigned GetMaximumHMSm() const;
		void DeleteChunkList();

		void Update ();
		void ComputeTempo();
		unsigned char * GetHMSText();
		void SetMidiTempo();

		unsigned ComputeMaxTracks();
		unsigned ComputeUsedTracks();

		bool MciGetPlayValue() const;
		DWORD MciPlay ( HWND hWndNotify );
		DWORD MciPlayAndLoop ( HWND hWndNotify );
		DWORD MciStop ();
		DWORD MciPlayOnce ( HWND hWndNotify );
		bool MciIsPlaying ( ) const;

		void SetMidiFileDivisionHi ( unsigned char cHi );
		void SetMidiFileDivisionLo ( unsigned char cLo );

		void AddEndOfTrack();

		//	Add Start Of Track Starting with track 1
		void AddStartOfTrack();

		void CorrectStartOfTrack();

		CMidiTrack * GetMidiTrackAddress ( unsigned iTrack );
		CMidiList * GetMidiListContext() const;
		const char * GetExtraText();
		unsigned char * GetTextAddress();
		unsigned char * GetValues();
		unsigned char GetChannel();
		unsigned char * GetMesureText();
		CMidiList * AdvanceTrackEvent();
		const unsigned char *GetTrackEvent ( unsigned int iTrack );

		boolean getEditChange();
		boolean setEditChange( boolean bEditChange );

		unsigned long GetMidiTrackSize ( unsigned int iTrack ) const;
		void ResetContext();
		unsigned char * GetText( unsigned short cSubCode = 0xffff );
		unsigned char GetMidiFileDivisionHi() const;
		unsigned char GetMidiFileDivisionLo() const;

		short int GetMidiFileTracksCount() const;

		short int GetMidiFileFormat() const;
		void ReadMidiFile ( const char *pFilename );
		void Save ( const char *pFilename );
		void ComputeMesure();
		void SetMidiSignature();
		void Trace ( const char *pTracename );
		void Free();
		void Init();
		void SetHeaderInformation();


};

#endif // !defined(AFX_MIDIFILE_H__BB233944_AE20_11D2_8147_444553540000__INCLUDED_)
