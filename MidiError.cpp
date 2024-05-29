//
////////////////////////////////////////////////////////////////////////
// MidiError.cpp: implementation of the CMidiError class.
//
////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MidiError.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	MESSAGE_LIMIT	4 * 1024 * 1024

IMPLEMENT_DYNAMIC(CMidiError, CObject)

//
////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
////////////////////////////////////////////////////////////////////////
CMidiError::CMidiError()
{
	m_MidiError		= false;
	m_FullErrorText		= NULL;
	m_LastErrorText		= NULL;
	m_FullErrorLen		= 0;
	m_LastErrorLen		= 0;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiError::~CMidiError()
{
	Free();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiError::Free()
{
	if ( m_FullErrorText != NULL )
	{
		free ( m_FullErrorText );
		m_FullErrorText		= NULL;
		m_FullErrorLen		= 0;
	}

	if ( m_LastErrorText != NULL )
	{
		free ( m_LastErrorText );
		m_LastErrorText		= NULL;
		m_LastErrorLen		= 0;
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
bool CMidiError::MidiError()
{
	return ( m_MidiError );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
bool CMidiError::MidiSuccess()
{
	if ( m_MidiError )
	{
		return ( false );
	}
	else
	{
		return ( true );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiError::SetMidiErrorText ( const char *pText )
{
	//	Last Error Text
	if ( m_LastErrorLen == 0 || m_LastErrorText == NULL )
	{
		m_LastErrorLen = strlen ( pText ) + 1;
		m_LastErrorText	= ( char * ) malloc ( m_LastErrorLen );
	}
	else if ( m_LastErrorLen < strlen ( pText ) + 1 )
	{
		if ( m_LastErrorText != NULL )
		{
			free ( m_LastErrorText );
			m_LastErrorText		= NULL;
			m_LastErrorLen		= 0;
		}

		m_LastErrorLen = strlen ( pText ) + 1;
		m_LastErrorText	= ( char * ) malloc ( m_LastErrorLen );
	}

	ZeroMemory ( m_LastErrorText, m_LastErrorLen );
	strcpy_s ( m_LastErrorText, m_LastErrorLen, pText );

	//	Message Limit
	if ( m_FullErrorLen > MESSAGE_LIMIT )
	{
		if ( m_LastErrorText != NULL )
		{
			free ( m_LastErrorText );
			m_LastErrorText		= NULL;
			m_LastErrorLen		= 0;
		}
	}

	//	Then the Full Text
	char		*pOldFullText	= NULL;
	if ( m_FullErrorText != NULL && m_FullErrorLen != 0 )
	{
		pOldFullText = m_FullErrorText;
	}

	//
	if ( pOldFullText == NULL || m_FullErrorLen == 0 || m_FullErrorText == NULL )
	{
		m_FullErrorLen = strlen ( pText ) + 3;		// CR LF NULL
		m_FullErrorText	= ( char * ) malloc ( m_FullErrorLen );
	}
	else if ( m_FullErrorLen < strlen ( pOldFullText ) + strlen ( pText ) + 3 )
	{
		m_FullErrorLen = 0;
		m_FullErrorLen	+= strlen ( pOldFullText );
		m_FullErrorLen	+=  strlen ( pText ) + 3;	// CR LF NULL
		m_FullErrorText	= ( char * ) malloc ( m_FullErrorLen );
	}

	ZeroMemory ( m_FullErrorText, m_FullErrorLen );
	if ( pOldFullText != NULL )
	{
		strcat_s ( m_FullErrorText, m_FullErrorLen, pOldFullText );
	}
	strcat_s ( m_FullErrorText, m_FullErrorLen, "\r\n" );
	strcat_s ( m_FullErrorText, m_FullErrorLen, pText );

	if ( pOldFullText != NULL )
	{
		free ( pOldFullText );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiError::GetFullErrorText() const
{
	if ( m_FullErrorText == NULL )
	{
		return ( "" );
	}

	return ( m_FullErrorText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiError::GetLastErrorText() const
{
	if ( m_LastErrorText == NULL )
	{
		return ( "" );
	}

	return ( m_LastErrorText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiError::SetMidiError()
{
	m_MidiError = true;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiError::SetMidiSuccess()
{
	m_MidiError = false;
}

