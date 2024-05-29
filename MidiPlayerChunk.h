// MidiPlayerChunk.h: interface for the CMidiPlayerChunk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIPLAYERCHUNK_H__6453CDC1_C75F_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIPLAYERCHUNK_H__6453CDC1_C75F_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include "MidiClass.h"

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
class CMidiPlayerChunk : public CMidiClass  
{
	public:
		unsigned char GetChannel() const;
		int GetTrack() const;
		DWORD GetDwordChunk() const;
		void SetMidiHdrChunk ( MIDIHDR *pChunk );
		void SetDwordChunk ( DWORD dwChunk );
		CMidiPlayerChunk();
		void SkipCode();

		//	For SysEx
		CMidiPlayerChunk ( unsigned iType, 
			CMidiPlayerChunk *pPrevious, CMidiPlayerChunk *pNext, 
			unsigned long iHMSm, unsigned long iDelta, MIDIHDR *pSysexChunk );
		//	For somethibg other than sysex
		CMidiPlayerChunk ( unsigned iType, 
			CMidiPlayerChunk *pPrevious, CMidiPlayerChunk *pNext, 
			unsigned long iHMSm, unsigned long iDelta, DWORD dwDwordChunk,
			unsigned char iChan, int iTrack );

		unsigned GetValues ( unsigned long *iHMSm, unsigned long *iDelta,
			DWORD *dwDwordChunk, MIDIHDR *pSysexChunk,
			unsigned char *iChan, int *iTrack, bool bShort=false ) const;

		CMidiPlayerChunk * GetNext() const;
		CMidiPlayerChunk * GetPrevious() const;

		void SetNext( CMidiPlayerChunk *pNext );
		void SetPrevious( CMidiPlayerChunk *pPrevious );

		virtual ~CMidiPlayerChunk();

	protected:
		int					m_iTrack;
		unsigned char		m_iChannel;

		//
		CMidiPlayerChunk	*m_pNext;
		CMidiPlayerChunk	*m_pPrevious;

		unsigned			m_iType;

		unsigned long		m_iHMSm;
		unsigned long		m_iDelta;

		MIDIHDR				m_SysexChunk;
		DWORD				m_DwordChunk;
		DWORD				m_DwordShortChunk;
};

#endif // !defined(AFX_MIDIPLAYERCHUNK_H__6453CDC1_C75F_11D2_8147_444553540000__INCLUDED_)
