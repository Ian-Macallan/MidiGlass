// MidiSysEx.cpp: implementation of the CMidiSysEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiSysEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMidiSysEx			clSysExGSReset [ MAX_SYSEX_LINES ];
CMidiSysEx			clSysExGMReset [ MAX_SYSEX_LINES ];
CMidiSysEx			clSysExXGReset [ MAX_SYSEX_LINES ];

IMPLEMENT_DYNAMIC(CMidiSysEx, CMidiClass)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMidiSysEx::CMidiSysEx()
{
	Init ();
}

void CMidiSysEx::Set ( unsigned iLength, const char *pSysEx )
{
	Delete ();

	m_iLength	= iLength;
	m_pSysEx	= ( char * ) malloc ( m_iLength + 1 );
	memcpy ( m_pSysEx, pSysEx, m_iLength );
}

void CMidiSysEx::Set ( const char *pSysExText)
{
	char			szAscii [ MAX_LONG_STRING ];
	char			szHexa [ MAX_LONG_STRING ];
	CMidiClass		clForConversion;

	strcpy_s ( szAscii, sizeof ( szAscii ), pSysExText );
	m_iLength = 
		clForConversion.ConvertAscii2Hexa ( szAscii, szHexa );

	m_pSysEx	= ( char * ) malloc ( m_iLength + 1 );
	memcpy ( m_pSysEx, szHexa, m_iLength );

}

CMidiSysEx::~CMidiSysEx()
{
	if ( m_pSysEx != NULL )
	{
		free ( m_pSysEx );
		m_pSysEx = NULL;
	}
}

void CMidiSysEx::Init()
{
	m_iLength	= 0;
	m_pSysEx	= NULL;
}

void CMidiSysEx::Delete()
{
	if ( m_pSysEx != NULL )
	{
		free ( m_pSysEx );
		m_pSysEx = NULL;
	}

	Init ();
}


unsigned CMidiSysEx::Get( char *pSysEx)
{
	if ( m_iLength >= 0 )
	{
		memcpy ( pSysEx, m_pSysEx, m_iLength );
	}

	return m_iLength;
}

bool CMidiSysEx::Empty()
{
	return ( m_iLength == 0 );
}
