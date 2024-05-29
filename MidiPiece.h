//
///////////////////////////////////////////////////////////////////////////////////
// MidiPiece.h: interface for the CMidiPiece class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIPIECE_H__4D848421_AE07_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIPIECE_H__4D848421_AE07_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiClass.h"
#include "MidiEvent.h"
#include "MidiText.h"
#include "MidiBinary.h"

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
class CMidiPiece : public CMidiClass  
{
	DECLARE_DYNAMIC(CMidiPiece)

	protected:

		bool						m_bNoteOffUsed;

		//		The time in Hour/Minutes/Seconds/Millisec
		unsigned long				m_lNoteDuration;

		//		State of a Piece
		int							m_iMidiPieceState;

		//		The time in Hour/Minutes/Seconds/Millisec
		//		Modified by ComputeTempo
		unsigned long				m_lAbsoluteHMSm;

		//		The Hour / Minutes / Seconds / Fractions
		//		Modified by ComputeTempo
		unsigned short				m_iMidiHours;
		unsigned short				m_iMidiMinutes;
		unsigned short				m_iMidiSeconds;
		unsigned short				m_iMidiMillisecs;

		//		The Tempo in a long word
		unsigned long				m_lTempo;

		//		Three Bytes
		//		The default Value is 600.000 (it means 100)
		//		Note that a value of 500000 is 0,5 second
		//		by note that has a length of 120 (1:000)
		//		9, 39, 192
		unsigned char				m_cMidiPieceTempo [ 3 ];

		//		The Delta Time
		//		Modified by SetTempo
		unsigned long				m_iMidiAbsoluteTime;
		unsigned long				m_iMidiDeltaTime;

		//
		CMidiEvent					*m_pMidiEvent;

		//		Value is 4/4 4/2
		unsigned char				m_cMidiPieceNN;
		//		Value is 4/4 4/2
		unsigned char				m_cMidiPieceDD;

		//		Modified by ComputeMesure
		unsigned short				m_iMidiMesure;
		unsigned short				m_iMidiBeat;
		unsigned short				m_iMidiTimeFrame;
		unsigned short				m_iMidiTimeDivision;

	public:
		void						SetAbsoluteTime ( unsigned long iTime );

		//	Channel and complement the code with the channel
		void						SetChannelAndCode ( unsigned char cChannel );

		unsigned short				GetTimeDivision() const;
		bool						GetNoteOffUsed() const;
		void						SetNoteOffUsed ( bool bUsed );
		unsigned long				GetNoteDuration ( ) const;
		void						SetNoteDuration ( unsigned char	cHi, unsigned char	cLo, unsigned long lDuration );
		unsigned char				GetValue1() const;
		unsigned char				GetValue2() const;
		unsigned char				GetValue3() const;
		unsigned char				GetValue4() const;
		unsigned char				GetValue5() const;
		unsigned char				GetValue6() const;

		const char					*GetLabel1() const;
		const char					*GetLabel2() const;
		const char					*GetLabel3() const;
		const char					*GetLabel4() const;
		const char					*GetLabel5() const;
		const char					*GetLabel6() const;

		int							GetState() const;
		void						SetState ( int iState );
		MIDIHDR						*GetSysexChunk();
		unsigned long				GetAbsoluteHMSm() const;
		void						SetMesure ( unsigned short lMesure );
		void						SetBeat ( unsigned short lMesure );
		void						SetTimeFrame ( unsigned short lMesure );

		DWORD						GetChunk () const;
		unsigned short				GetHours() const;
		unsigned short				GetMinutes() const;
		unsigned short				GetSeconds() const;
		unsigned short				GetMillisecs() const;

		unsigned char				*GetHMSText() const;
		void						SetTempo ( const unsigned char cTempo [ 3 ] );
		const unsigned char			*GetTempo() const;
		void						SetDeltaTime ( unsigned long iTime );
		void						AddDeltaTime ( unsigned long iTime );
		unsigned long				GetDeltaTime() const;
		const char					*GetExtraText() const;
		unsigned char				*GetValues() const;
		unsigned char				GetChannel() const;
		unsigned char				*GetMesureText () const;
		unsigned char				Power ( unsigned char cVal );

		CMidiEvent					*GetMidiEvent();
		bool						useChannel() const;
		bool						usePort() const;
		int							valueCount() const;

		virtual void				Save ( FILE *hFile );
		virtual unsigned char		*GetTextAddress( unsigned short cSubCode = 0xffff )  const;
		virtual const unsigned char	*GetEventText ( ) const;
		virtual unsigned int		GetEventIndex() const;
		virtual unsigned char		GetSubCode() const;
		virtual unsigned char		GetCode() const;
		virtual unsigned short		GetTimeFrame() const;
		virtual unsigned short		GetBeat() const;
		virtual unsigned short		GetMesure() const;
		virtual void				SetDD ( unsigned char cDD );
		virtual void				SetNN ( unsigned char cNN );
		virtual unsigned char		GetDD() const;
		virtual unsigned char		GetNN() const;
		virtual unsigned long		GetAbsoluteTime () const;

		virtual unsigned long		ComputeAbsoluteTime ( unsigned long iTime );

		virtual void ComputeMesure ( 
			unsigned char	cHi,
			unsigned char	cLo,
			unsigned char	cNN,
			unsigned char	cDD,
			unsigned short	iMesure, 
			unsigned short	iBeat, 
			unsigned short	iTimeFrame,
			long			lDelta );

		virtual void ComputeTempo ( 
			unsigned char	cHi,
			unsigned char	cLo,
			unsigned char	cTempo [ ],
			unsigned short	cHours, 
			unsigned short	cMinutes,
			unsigned short	cSeconds,
			unsigned short	cMillisecs,
			long			lDelta );

		void Free();

		void Init (		unsigned char *cOldNN, unsigned char *cOldDD,
						unsigned char *cOldTempo );
		void Trace ( FILE *hFile );

		CMidiPiece (	unsigned char *cOldNN, unsigned char *cOldDD,
						unsigned char *cOldTempo );

		CMidiPiece (	unsigned char *cOldNN, unsigned char *cOldDD,
						unsigned char *cOldTempo, 
						unsigned char *pBuffer,
						unsigned char *oldMidiCommand );

		CMidiPiece (	unsigned char			*cOldNN, unsigned char *cOldDD,
						unsigned char			*cOldTempo,
						unsigned				iEvent, 
						unsigned long			iDelta,
						unsigned				iChannelPort = 0,
						unsigned				iLenText = 0,
						const unsigned char		*pText = NULL,
						unsigned char			cVal1 = 0x00, 
						unsigned char			cVal2 = 0x00, 
						unsigned char			cVal3 = 0x00, 
						unsigned char			cVal4 = 0x00, 
						unsigned char			cVal5 = 0x00, 
						unsigned char			cVal6 = 0x00 );

		CMidiPiece ( const CMidiPiece &midiPiece );
		CMidiPiece ( const CMidiPiece *midiPiece );

		void Dupplicate ( const CMidiPiece *midiPiece );

		void NewEmptyEvent ( int iEvent );
		static CMidiEvent *NewMidiEvent ( int iEvent );
		static CMidiEvent *NewMidiEvent ( int iEvent, const unsigned char *pText );
		static CMidiEvent *NewMidiEvent ( int iEvent, const unsigned char *pText, int len );
		static CMidiEvent *NewMidiEvent ( int iEvent, unsigned char cByte);
		static CMidiEvent *NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2 );
		static CMidiEvent *NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2,
											unsigned char cByte3 );
		static CMidiEvent *NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2, 
											unsigned char cByte3, unsigned char cByte4 );
		static CMidiEvent *NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2,
											unsigned char cByte3, unsigned char cByte4, unsigned char cByte5 );
		static	CMidiPiece * newEmptyMidiPiece();

		virtual ~CMidiPiece();

	protected :
		CMidiPiece();

};

#endif // !defined(AFX_MIDIPIECE_H__4D848421_AE07_11D2_8147_444553540000__INCLUDED_)
