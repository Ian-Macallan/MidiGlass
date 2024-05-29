// MidiPlayerChunk.h: interface for the CMidiPlayerChunk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIINPUTCHUNK_H__6453CDC1_C75F_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIINPUTCHUNK_H__6453CDC1_C75F_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmsystem.h>
#include "MidiClass.h"

class CMidiInputChunk : public CMidiClass  
{
public:
	unsigned char GetChunkByte1();
	unsigned char GetChunkByte2();
	unsigned char GetChunkByte3();
	unsigned char GetChunkByte4();
	DWORD GetChunkTime();
	int GetChunkType();
	DWORD GetSysexLength();
	char * GetSysexAddress();
	void Init();
	DWORD GetDwordChunk() const;
	void SetMidiHdrChunk ( MIDIHDR *pChunk );
	void SetDwordChunk ( DWORD *dwChunk );
	CMidiInputChunk();
	CMidiInputChunk ( unsigned iType, 
		CMidiInputChunk *pPrevious, CMidiInputChunk *pNext, 
		DWORD iTime, MIDIHDR *pSysexChunk );
	CMidiInputChunk ( unsigned iType, 
		CMidiInputChunk *pPrevious, CMidiInputChunk *pNext, 
		DWORD iTime, DWORD dwDwordChunk );
	unsigned GetValues ( DWORD *iTime,
		DWORD *pDwordChunk, MIDIHDR *pSysexChunk ) const;
	CMidiInputChunk * GetNext() const;
	CMidiInputChunk * GetPrevious() const;
	void SetNext( CMidiInputChunk *pNext );
	void SetPrevious( CMidiInputChunk *pPrevious );
	virtual ~CMidiInputChunk();

protected:
	CMidiInputChunk		*m_pNext;
	CMidiInputChunk		*m_pPrevious;

	unsigned			m_iType;

	DWORD				m_iTime;

	char				*m_SysexBuffer;
	DWORD				m_SysexBufferLength;
	MIDIHDR				m_SysexChunk;

	DWORD				m_DwordChunk;
};

#endif // !defined(AFX_MIDIINPUTCHUNK_H__6453CDC1_C75F_11D2_8147_444553540000__INCLUDED_)
