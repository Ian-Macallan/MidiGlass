//
////////////////////////////////////////////////////////////////////////
// MidiEvent.cpp: implementation of the CMidiEvent class.
//
////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MidiEvent.h"
#include "MidiGlassApp.h"

extern	CMidiWorksApp theApp;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMidiEvent, CMidiClass)

static BOOL	IgnoreErrors = FALSE;
static char szMessage [ MAX_PATH ];

//
////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
////////////////////////////////////////////////////////////////////////
CMidiEvent::CMidiEvent()
{
	m_cMidiEventCode			= 0;
	m_cMidiEventSubCode			= 0xff;

	m_cMidiEventChannel			= 0xff;
	m_MidiEventStartOfBuffer	= NULL;
	m_MidiEventEndOfBuffer		= NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent::~CMidiEvent()
{

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::EventMessageBox(const char *pText )
{
	//
	CMainFrame *pMainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd( ) );
	if ( pMainFrame != NULL )
	{
		CMidiDoc *pMididoc	= dynamic_cast< CMidiDoc * >( pMainFrame->GetMidiDoc() );
		if ( pMididoc != NULL )
		{
			CMidiFile *pMidiFile = pMididoc->GetDocumentMidiFile();
			if ( pMidiFile != NULL )
			{
				pMidiFile->SetMidiErrorText ( pText );
			}
		}
	}

	//
	if ( ! IgnoreErrors )
	{
		UINT nType = MB_YESNO;
		strcpy_s ( szMessage, sizeof(szMessage), pText );
		strcat_s ( szMessage, sizeof(szMessage), "\r\nYES to See Messages, NO to Ugnore" );
		int returned = AfxMessageBox ( szMessage, nType );
		if ( returned == IDNO )
		{
			IgnoreErrors = TRUE;
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetCode () const
{
	return m_cMidiEventCode;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char *CMidiEvent::GetEndOfBufferAddress () const
{
	return m_MidiEventEndOfBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetCodeAndChannel() const
{
	return ( m_cMidiEventCode | m_cMidiEventChannel );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetSubCode() const
{
	return m_cMidiEventSubCode;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiEvent::GetSubCodeMask() const
{
	switch ( m_cMidiEventSubCode )
	{
		case MIDI_EVENT_CODE_META_SEQ_1 :		//	Sequence
		{
			return MASK_SEQUENCE;
		}

		case MIDI_EVENT_CODE_META_TEXT :		//	Text
		{
			return MASK_TEXT;
		}

		case MIDI_EVENT_CODE_META_COPY :		//	Copyright
		{
			return MASK_COPYRIGHT;
		}

		case MIDI_EVENT_CODE_META_TRKNAME :		//	Track Name
		{
			return MASK_TRACKNAME;
		}

		case MIDI_EVENT_CODE_META_INSNAME :		//	Instrument Name
		{
			return MASK_INSTRNAME;
		}

		case MIDI_EVENT_CODE_META_LYRICS :		//	Lyrics
		{
			return MASK_LYRICS;
		}

		case MIDI_EVENT_CODE_META_MARKER :		//	Markers
		{
			return MASK_MARKERS;
		}

		case MIDI_EVENT_CODE_META_CUEPOINT :		//	Cue Point
		{
			return MASK_CUEPOINT;
		}

		case MIDI_EVENT_CODE_META_SEQR :		//	Sequencer
		{
			return MASK_SEQUENCER;
		}

	}

	//		Other cases
	return MASK_OTHER;

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetChannel() const
{
	return m_cMidiEventChannel & CHANNEL_MASK;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetEventValue1() const
{
	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetEventValue2() const
{
	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetEventValue3() const
{
	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetEventValue4() const
{
	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetEventValue5() const
{
	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiEvent::GetEventValue6() const
{
	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char *CMidiEvent::GetTextAddress( unsigned short cSubCode ) const
{
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiEvent::GetExtraText() const
{
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned int CMidiEvent::GetEventIndex() const
{
	return m_iEventIndex;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiEvent::GetChunk()
{
	//		If not implemented it will return a NULL value
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
MIDIHDR *CMidiEvent::GetSysexChunk() 
{
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetChannelAndCode(unsigned char cChannel)
{
	// Set Channel And Code
	cChannel			= cChannel & CHANNEL_MASK;
	m_cMidiEventChannel = cChannel;
	m_cMidiEventCode	= m_cMidiEventCode & OPERATION_MASK;
	m_cMidiEventCode	= m_cMidiEventCode | m_cMidiEventChannel;

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void  CMidiEvent::SetText ( const unsigned char *pBuffer )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventValue1 ( unsigned char val1 )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventValue2 ( unsigned char val1 )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventValue3 ( unsigned char val1 )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventValue4 ( unsigned char val1 )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventValue5 ( unsigned char val1 )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventValue6 ( unsigned char val1 )
{
	// Do nothing
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetCodeAndChannel ( unsigned char code )
{
	m_cMidiEventCode	= code;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetSubCode( unsigned char  subCode )
{
	m_cMidiEventSubCode	= subCode;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiEvent::SetEventIndex ( unsigned int event )
{
	m_iEventIndex	 = event;
}