// MidiInputChunk.cpp: implementation of the CMidiInputChunk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiInputChunk.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMidiInputChunk::~CMidiInputChunk()
{
	if ( m_SysexChunk.lpData != NULL )
	{
		delete [] m_SysexChunk.lpData;
		m_SysexChunk.lpData = NULL;
	}
}

CMidiInputChunk * CMidiInputChunk::GetNext() const
{
	return m_pNext;
}

CMidiInputChunk * CMidiInputChunk::GetPrevious() const
{
	return m_pPrevious;
}

CMidiInputChunk::CMidiInputChunk()
{
	Init ( );
}

CMidiInputChunk::CMidiInputChunk(unsigned int iType, 
	CMidiInputChunk *pPrevious, CMidiInputChunk *pNext, 
	DWORD iTime, MIDIHDR *pSysexChunk )
{
	Init ( );

	m_pNext				= pNext;
	m_pPrevious			= pPrevious;

	m_iTime				= iTime;

	m_iType				= iType;

	//		The Midi Header
	SetMidiHdrChunk ( pSysexChunk);
}

CMidiInputChunk::CMidiInputChunk(unsigned int iType, 
	CMidiInputChunk *pPrevious, CMidiInputChunk *pNext, 
	DWORD iTime, DWORD dwDwordChunk )
{
	Init ( );

	m_pNext				= pNext;
	m_pPrevious			= pPrevious;

	m_iTime				= iTime;

	m_DwordChunk		= dwDwordChunk;

	m_iType				= GetIndex ( m_DwordChunk );

}

void CMidiInputChunk::SetNext( CMidiInputChunk *pNext )
{
	m_pNext				= pNext;
}

void CMidiInputChunk::SetPrevious( CMidiInputChunk *pPrevious )
{
	m_pPrevious			= pPrevious;
}

unsigned CMidiInputChunk::GetValues (
	DWORD *iTime,
	DWORD *pDwordChunk, MIDIHDR *pSysexChunk ) const
{
	*pDwordChunk	= m_DwordChunk;

	*pSysexChunk	= m_SysexChunk;

	*iTime			= m_iTime;

	return m_iType;
}

void CMidiInputChunk::SetDwordChunk(DWORD *dwChunk)
{
	m_DwordChunk		= *dwChunk;

	m_iType				= GetIndex ( m_DwordChunk );
}

void CMidiInputChunk::SetMidiHdrChunk(MIDIHDR *pChunk)
{
	if ( pChunk )
	{
		if ( pChunk->lpData && pChunk->dwBytesRecorded > 0 )
		{
			m_SysexBufferLength	= pChunk->dwBytesRecorded;
			m_SysexBuffer		= new char [ m_SysexBufferLength ];
			m_SysexChunk.lpData			= m_SysexBuffer;
			m_SysexChunk.dwBufferLength	= m_SysexBufferLength;
		}
	}
}

DWORD CMidiInputChunk::GetDwordChunk() const
{
	return m_DwordChunk;
}

void CMidiInputChunk::Init()
{
	m_pNext				= NULL;
	m_pPrevious			= NULL;

	m_iType				= MIDI_EVENT_UNKNOWN;
	m_iTime				= 0;

	m_DwordChunk		= 0;
	memset ( &m_SysexChunk, 0, sizeof ( m_SysexChunk ) );

	m_SysexBuffer		= NULL;
	m_SysexBufferLength	= 0;
}

char * CMidiInputChunk::GetSysexAddress()
{
	return m_SysexBuffer;
}

DWORD CMidiInputChunk::GetSysexLength()
{
	return m_SysexBufferLength;
}

int CMidiInputChunk::GetChunkType()
{
	return m_iType;
}

DWORD CMidiInputChunk::GetChunkTime()
{
	return m_iTime;
}

unsigned char CMidiInputChunk::GetChunkByte1()
{
	return ( unsigned char ) ( m_DwordChunk & 0xff );
}

unsigned char CMidiInputChunk::GetChunkByte2()
{
	return ( unsigned char ) ( ( m_DwordChunk >> 8 ) & 0xff );
}

unsigned char CMidiInputChunk::GetChunkByte3()
{
	return ( unsigned char ) ( ( m_DwordChunk >> 16 ) & 0xff );
}

unsigned char CMidiInputChunk::GetChunkByte4()
{
	return ( unsigned char ) ( ( m_DwordChunk >> 24 ) & 0xff );
}
