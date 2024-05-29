// MidiList.h: interface for the CMidiList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDILIST_H__BB233942_AE20_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDILIST_H__BB233942_AE20_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiPiece.h"

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
class CMidiList : public CMidiPiece  
{
	protected:
		CMidiList	*m_pPrevious;
		CMidiList	*m_pNext;
		int			m_iTrack;

	public:
		void			RemoveFromList( bool bAdjust );
		void			InsertAfter(CMidiList *pMidiNewList);
		void			InsertBefore(CMidiList *pMidiNewList);
		void			ChangeCurrent(CMidiList *pMidiNewList);
		void			Swap(CMidiList *pOtherMidiList);

		int				GetTrack() const;
		CMidiList		*GetPrevious() const;
		unsigned int	GetEventIndex() const;

		void			SetTrack( int iTrack );
		void			SetNext ( CMidiList *pNext );
		void			SetPrevious ( CMidiList *pPrevious );

		unsigned char	*GetNextAddress () const;
		CMidiList		*GetNext () const;

		CMidiList (	unsigned char			*cOldNN,
					unsigned char			*cOldDD,
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

		CMidiList (	unsigned char *cOldNN,
					unsigned char *cOldDD,
					unsigned char *cOldTempo, 
					unsigned char *pBuffer,
					unsigned char *oldCommand );

		CMidiList (	const CMidiList &midiList );
		CMidiList (	const CMidiList *midiList );

		virtual ~CMidiList();

		static	CMidiList * newEmptyMidiList();

	private :
		CMidiList();

};

#endif // !defined(AFX_MIDILIST_H__BB233942_AE20_11D2_8147_444553540000__INCLUDED_)
