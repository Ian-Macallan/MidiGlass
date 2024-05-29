// MidiTrack.h: interface for the CMidiTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDITRACK_H__BB233943_AE20_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDITRACK_H__BB233943_AE20_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiClass.h"
#include "MidiList.h"

class CMidiTrack : public CMidiClass  
{
	protected:
		//		Value is 4/4 4/2
		unsigned char		m_cMidiPieceNN;
		//		Value is 4/4 4/2
		unsigned char		m_cMidiPieceDD;

		//		3 Bytes
		//		The default Value is 600.000 (it means 100)
		//		Note that a value of 500000 is 0,5 second
		//		by note that has a length of 120 (1:000)
		//		9, 39, 192
		unsigned char		m_cMidiPieceTempo [ 3 ];

		//
		bool				m_bMute;
		CMidiTrack			*m_pMidiTrack0;
		unsigned			m_iMidiTrackMaximumLength;
		DWORD				m_iMidiTrackNumber;

		//
		CMidiList			*m_pMidiTrackBegList;
		CMidiList			*m_pMidiTrackEndList;

		//
		unsigned char		*m_pMidiTrackHeader;
		DWORD				m_iMidiTrackLength;

		unsigned char		*m_pMidiTrackBuffer;
		unsigned char		*m_pMidiTrackPosition;

		unsigned long		m_iMidiTrackEventCount;

	public:
		void AppendWithDelta(CMidiList **pList, CMidiList **pAt);
		void CorrectNoteDuration ( unsigned char cHi, unsigned char cLo );
		void SetTrackChannel ( unsigned char cChannel );
		void ComputeNoteDuration( unsigned char cHi, unsigned char cLo );
		void AddInstrumentName ( const char *pInstrument );
		void Insert(CMidiList **pList);
		void AddTrackName ( const char *pTrackName );
		void AddVolume ( unsigned char cVolume );
		bool GetVolume ( unsigned char *cVolume ) const;
		void RemoveEventCodeSubCode(unsigned int iType, unsigned char cSubType, bool bFirstEventOnly = false );
		void RemoveEventCode ( unsigned int iCode, bool bSkipFirstEvent = false, bool bFirstEventOnly = false );
		void AddProgram ( unsigned char iProgram );
		char * GetInstrumentName() const;
		char * GetTrackName() const;
		const char * GetProgramText() const;
		//	Scan full list to see if we have only on channel
		unsigned char GetChannel() const;
		void SetMuteState ( bool bMute );
		bool GetMuteState() const;
		int GetTrackNumber() const;

		void InsertAfter (CMidiList **pList, CMidiList **pAt);
		void InsertBefore (CMidiList **pList, CMidiList **pAt);
		void Append ( CMidiList **pList, CMidiList **pAt );

		int CountEvents ();
		void UpdateTrackNumber ();

		void AlterBegin ( CMidiList *pOldMidiList, CMidiList *pNewMidiList );
		void AlterEnd ( CMidiList *pOldMidiList, CMidiList *pNewMidiList );
		void AlterBeginOrEnd ( CMidiList *pOldMidiList, CMidiList *pNewMidiList );
		void RemoveEventFromList ( CMidiList *pList, bool bAdjust );

		void AddXGReset();
		void AddGSReset();
		void AddGMReset();

		void ComputeTempo( unsigned char cHi, unsigned char cLo );
		void SetMidiTrack0 ( CMidiTrack *pTrack0, unsigned char *cNN, unsigned char *cDD, unsigned char *cTempo );

		void DeleteEventAdjustTime ( CMidiList *pCurrentList );
		void DeleteEventNoAdjust ( CMidiList *pCurrentList );
		void InsertEventBefore ( CMidiList *pCurrentList, CMidiList *pNewMidiList  );
		void InsertEventAfter ( CMidiList *pCurrentList, CMidiList *pNewMidiList  );
		void ChangeCurrentEvent ( CMidiList *pCurrentList, CMidiList *pNewMidiList  );

		void AddEndOfTrack();
		void AddStartOfTrack();

		void Delete();
		void Reset();

		unsigned GetTrackLength() const;
		void SetMaximumTrackLength ( unsigned iLength );
		CMidiList * GetNextAddress (CMidiList *pList ) const;
		CMidiList * GetFirstAddress() const;
		CMidiList * GetListAddress ( CMidiList *pList ) const;
		void Save ( FILE *hFile );
		void ComputeMesure ( unsigned char cHi, unsigned char cLo );
		CMidiList * GetListPointer() const;
		void SetMidiSignature ( );
		void SetMidiTempo ( );
		void ComputeAbsoluteTime();
		void Free();
		void Init();
		void Trace ( FILE *hFile );
		void DeleteListToTheEnd ( CMidiList *pList );
		CMidiTrack();
		unsigned char *ReadTrack ( int iTrack, unsigned char *pBuffer );
		virtual ~CMidiTrack();

};

#endif // !defined(AFX_MIDITRACK_H__BB233943_AE20_11D2_8147_444553540000__INCLUDED_)
