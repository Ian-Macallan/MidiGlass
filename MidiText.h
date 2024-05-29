// MidiText.h: interface for the CMidiText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDITEXT_H__3B8FC602_AD51_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDITEXT_H__3B8FC602_AD51_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "mmsystem.h"

#include "MidiEvent.h"

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiText : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiText)

	protected :

		//	Subcode must be 0x01

		//	Text Length.
		unsigned long		m_iMidiTextLength;

		//	Text Value.
		unsigned char		*m_MidiTextBuffer;

	public:
		CMidiText( unsigned char cCommand, unsigned char *pBuffer );
		CMidiText( const unsigned char *pBuffer );
		virtual ~CMidiText();

		virtual void Free();
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual const unsigned char *GetEventText () const;
		virtual unsigned char *GetTextAddress( unsigned short cSubCode = 0xffff ) const;
		virtual void SetText ( const unsigned char *pBuffer );

		virtual const char *GetEventLabel1 () const
		{
			return "Text";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiUnknownText : public CMidiText 
{
	DECLARE_DYNAMIC(CMidiUnknownText)

	public:
		CMidiUnknownText( unsigned char cCommand, unsigned char *pBuffer );
		CMidiUnknownText( const unsigned char *pBuffer );
		virtual ~CMidiUnknownText();

		virtual void Trace ( FILE *hFile );
		virtual const unsigned char *GetEventText () const;

		virtual const char *GetEventLabel1 () const
		{
			return "Unknown";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiCopyRight : public CMidiText  
{
	DECLARE_DYNAMIC(CMidiCopyRight)

	public:
		CMidiCopyRight( unsigned char cCommand, unsigned char *pBuffer );
		CMidiCopyRight( const unsigned char *pBuffer );
		virtual ~CMidiCopyRight();

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Copyright";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiTrackName : public CMidiText  
{

	DECLARE_DYNAMIC(CMidiTrackName)

	public:
		CMidiTrackName( unsigned char cCommand, unsigned char *pBuffer );
		CMidiTrackName( const unsigned char *pBuffer );
		virtual ~CMidiTrackName();

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Track Name";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiInstrumentName : public CMidiText  
{
	DECLARE_DYNAMIC(CMidiInstrumentName)

	public:
		CMidiInstrumentName( unsigned char cCommand, unsigned char *pBuffer );
		CMidiInstrumentName( const unsigned char *pBuffer );
		virtual ~CMidiInstrumentName();

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Instrument Name";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiLyrics : public CMidiText  
{

	DECLARE_DYNAMIC(CMidiLyrics)

public:
		CMidiLyrics( unsigned char cCommand, unsigned char *pBuffer );
		CMidiLyrics( const unsigned char *pBuffer );
		virtual ~CMidiLyrics();

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Lyrics";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiMarkers : public CMidiText  
{
	DECLARE_DYNAMIC(CMidiMarkers)

	public:
		CMidiMarkers( unsigned char cCommand, unsigned char *pBuffer );
		CMidiMarkers( const unsigned char *pBuffer );
		virtual ~CMidiMarkers();

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Markers";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiCuePoint : public CMidiText  
{
	DECLARE_DYNAMIC(CMidiCuePoint)

	public:
		CMidiCuePoint( unsigned char cCommand, unsigned char *pBuffer );
		CMidiCuePoint( const unsigned char *pBuffer );
		virtual ~CMidiCuePoint();

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "CuePoint";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Cue Point";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiSystemExclusive : public CMidiEvent  
{
		DECLARE_DYNAMIC(CMidiSystemExclusive)

	protected :
		MIDIHDR m_MidiSysexChunk;

		//	Text Length.
		unsigned long		m_iMidiSysexLength;

		//	Text Value.
		unsigned char		*m_MidiSysexBuffer;

		//	The full sysex value
		unsigned char		*m_MidiSysexFull;

	public :
		const unsigned char *GetSysexBuffer();
		int GetSysexBufferLength();

	public:
		CMidiSystemExclusive( unsigned char cCommand, unsigned char *pBuffer );
		CMidiSystemExclusive ( const unsigned char *pText, unsigned iLen );
		virtual ~CMidiSystemExclusive();

		MIDIHDR * GetSysexChunk();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual const char * GetExtraText() const;
		//	Take an hexadeimal string and store it
		//	Must be on the form hx hx hx hx
		virtual void SetText ( const unsigned char *pBuffer );

		virtual const char *GetEventLabel1 () const
		{
			return "System Exclusive";
		}

};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiStartOfTrack : public CMidiEvent  
{
		DECLARE_DYNAMIC(CMidiStartOfTrack)

	protected :

		//	Code must be 0xFF

		//	Subcode must be 0x21

		//	Subcode must be 0x01
		unsigned char		m_cMidiEventSubCode2;

		unsigned char		m_cMidiPort;

	public:
		CMidiStartOfTrack( unsigned char cCommand, unsigned char *pBuffer );
		CMidiStartOfTrack( unsigned char value1 );
		virtual ~CMidiStartOfTrack();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

		virtual unsigned char GetEventValue1 () const;
		virtual void SetEventValue1 ( unsigned char val1 );

		virtual const char *GetEventLabel1 () const
		{
			return "Midi Port";
		}

		virtual bool useChannel() const
		{
			return false;
		}

		virtual bool usePort() const
		{
			return true;
		}

		virtual int valueCount() const
		{
			return 1;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiStartOfTrack20 : public CMidiStartOfTrack
{

		DECLARE_DYNAMIC(CMidiStartOfTrack20)

	public:
		CMidiStartOfTrack20( unsigned char cCommand, unsigned char *pBuffer );
		CMidiStartOfTrack20( unsigned char value1 );
		virtual ~CMidiStartOfTrack20();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual unsigned char GetEventValue1 () const;
		virtual void SetEventValue1 ( unsigned char val1 );

		virtual const char *GetEventLabel1 () const
		{
			return "Midi Port";
		}

		virtual bool useChannel() const
		{
			return false;
		}

		virtual bool usePort() const
		{
			return true;
		}

		virtual int valueCount() const
		{
			return 1;
		};

};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiEndOfTrack : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiEndOfTrack)

	protected :

		//	Code must be 0xFF

		//	Subcode must be 0x2F

		//	Subcode must be 0x00
		unsigned char		m_cMidiEventSubCode2;

	public:
		CMidiEndOfTrack( unsigned char cCommand, unsigned char *pBuffer );
		CMidiEndOfTrack( );
		virtual ~CMidiEndOfTrack();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiSetTempo : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiSetTempo)

	protected :

		//	Code must be 0xFF

		//	Subcode must be 0x51

		//	Subcode must be 0x03
		unsigned char		m_cMidiEventSubCode2;

		unsigned char		m_cMidiTempo [ 3 ];

	public:
		CMidiSetTempo( unsigned char cCommand, unsigned char *pBuffer );
		CMidiSetTempo( unsigned char cVal1, unsigned char cVal2, unsigned char cVal3 );
		virtual ~CMidiSetTempo();

		const unsigned char * GetTempo() const;
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;
		virtual unsigned char GetEventValue3() const;
		virtual void SetEventValue1( unsigned char val );
		virtual void SetEventValue2( unsigned char val );
		virtual void SetEventValue3( unsigned char val );

		virtual const char *GetEventLabel1 () const
		{
			return "Tempo Value 1";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Tempo Value 2";
		}

		virtual const char *GetEventLabel3 () const
		{
			return "Tempo Value 3";
		}

		virtual int valueCount() const
		{
			return 3;
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiSetSMPTE : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiSetSMPTE)

	protected :

		//	Code must be 0xFF

		//	Subcode must be 0x54
	
		//	Subcode must be 0x05
		unsigned char		m_cMidiEventSubCode2;

		unsigned char		m_cMidiHours;
		unsigned char		m_cMidiMinutes;
		unsigned char		m_cMidiSecondes;

		unsigned char		m_cMidiFractional;
		unsigned char		m_cMidiFrame;

	public:
		CMidiSetSMPTE( unsigned char cCommand, unsigned char *pBuffer );
		CMidiSetSMPTE( unsigned char cVal1, unsigned char cVal2, 
			unsigned char cVal3, unsigned char cVal4, 
			unsigned char cVal5 );
		virtual ~CMidiSetSMPTE();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;
		virtual unsigned char GetEventValue3() const;
		virtual unsigned char GetEventValue4() const;
		virtual unsigned char GetEventValue5() const;

		virtual void SetEventValue1(unsigned char  val);
		virtual void SetEventValue2(unsigned char  val);
		virtual void SetEventValue3(unsigned char  val);
		virtual void SetEventValue4(unsigned char  val);
		virtual void SetEventValue5(unsigned char  val);

		virtual const char *GetEventLabel1 () const
		{
			return "Hours";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Minutes";
		}

		virtual const char *GetEventLabel3 () const
		{
			return "Seconds";
		}

		virtual const char *GetEventLabel4 () const
		{
			return "Fractional";
		}

		virtual const char *GetEventLabel5 () const
		{
			return "Frame";
		}

		virtual int valueCount() const
		{
			return 5;
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiTimeSignature : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiTimeSignature)

	protected :

		//	Code must be 0xFF

		//	Subcode must be 0x58

		//	Subcode must be 0x04
		unsigned char		m_cMidiEventSubCode2;

		unsigned char		m_cMidiNN;
		unsigned char		m_cMidiDD;
		unsigned char		m_cMidiCC;
		unsigned char		m_cMidiBB;

	public:
		CMidiTimeSignature( unsigned char cCommand, unsigned char *pBuffer );
		CMidiTimeSignature( unsigned char cVal1, unsigned char cVal2, 
			unsigned char cVal3, unsigned char cVal4 );
		virtual ~CMidiTimeSignature();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		unsigned char CMidiTimeSignature::GetDD () const;
		unsigned char CMidiTimeSignature::GetNN () const;
		virtual void Save ( FILE *hFile );

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;
		virtual unsigned char GetEventValue3() const;
		virtual unsigned char GetEventValue4() const;

		virtual void SetEventValue1(unsigned char  val);
		virtual void SetEventValue2(unsigned char  val);
		virtual void SetEventValue3(unsigned char  val);
		virtual void SetEventValue4(unsigned char  val);

		virtual const char *GetEventLabel1 () const
		{
			return "NN";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "DD";
		}

		virtual const char *GetEventLabel3 () const
		{
			return "CC";
		}

		virtual const char *GetEventLabel4 () const
		{
			return "BB";
		}

		virtual int valueCount() const
		{
			return 4;
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiKeySignature : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiKeySignature)

	protected :

		//	Code must be 0xFF

		//	Subcode must be 0x59

		//	Subcode must be 0x02
		unsigned char		m_cMidiEventSubCode2;

		unsigned char		m_cMidiSf;
		unsigned char		m_cMidiMi;

	public:
		CMidiKeySignature( unsigned char cCommand, unsigned char *pBuffer );
		CMidiKeySignature( unsigned char cVal1, unsigned char cVal2 );
		virtual ~CMidiKeySignature();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;

		virtual void SetEventValue1(unsigned char  val);
		virtual void SetEventValue2(unsigned char  val);

		virtual const char *GetEventLabel1 () const
		{
			return "SF";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Mi";
		}

		virtual int valueCount() const
		{
			return 2;
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiNoteOff : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiNoteOff)

	protected :

		//	Code must be 0x80
		unsigned char		m_cMidiPitch;
		unsigned char		m_cMidiVelocity;

	public:
		CMidiNoteOff( unsigned char cCommand, unsigned char *pBuffer );
		CMidiNoteOff( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 );

		virtual ~CMidiNoteOff();

		virtual DWORD GetChunk ();
		//	Channel and complement the code with the channel
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual const char * GetExtraText() const;

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;

		virtual void SetEventValue1 ( unsigned char val1 );
		virtual void SetEventValue2 ( unsigned char val2 );

		virtual const char *GetEventLabel1 () const
		{
			return "Pitch";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Velocity";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 2;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiNoteOn : public CMidiEvent  
{
		DECLARE_DYNAMIC(CMidiNoteOn)

	protected :

		//	Code must be 0x90
		unsigned char		m_cMidiPitch;
		unsigned char		m_cMidiVelocity;

	public:
		CMidiNoteOn(unsigned char cCommand, unsigned char *pBuffer );
		CMidiNoteOn( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 );
		virtual ~CMidiNoteOn();

		//	Channel and complement the code with the channel
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual DWORD GetChunk ();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual const char * GetExtraText() const;

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;

		virtual void SetEventValue1 ( unsigned char val1 );
		virtual void SetEventValue2 ( unsigned char val2 );

		virtual const char *GetEventLabel1 () const
		{
			return "Pitch";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Velocity";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 2;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiKeyPressure : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiKeyPressure)

	protected :

		//	Code must be 0xA0
		unsigned char		m_cMidiPitch;
		unsigned char		m_cMidiPressure;

	public:
		CMidiKeyPressure( unsigned char cCommand, unsigned char *pBuffer );
		CMidiKeyPressure( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 );
		virtual ~CMidiKeyPressure();

		virtual DWORD GetChunk ();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;

		virtual void SetEventValue1 ( unsigned char val1 );
		virtual void SetEventValue2 ( unsigned char val2 );

		virtual const char *GetEventLabel1 () const
		{
			return "Pitch";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Pressure";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 2;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiParameter : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiParameter)

	protected :

		//	Code must be 0xB0
		unsigned char		m_cMidiParameter;
		unsigned char		m_cMidiSetting;

	public:
		CMidiParameter( unsigned char cCommand, unsigned char *pBuffer );
		CMidiParameter( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 );
		virtual ~CMidiParameter();

		//	Channel and complement the code with the channel
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual DWORD GetChunk ();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual const char * GetExtraText() const;

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;

		virtual void SetEventValue1 ( unsigned char val1 );
		virtual void SetEventValue2 ( unsigned char val2 );

		virtual const char *GetEventLabel1 () const
		{
			return "Parameter";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Setting";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 2;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiProgram : public CMidiEvent  
{
		DECLARE_DYNAMIC(CMidiProgram)

	protected :

		//	Code must be 0xC0
		unsigned char		m_cMidiProgram;

	public:
		CMidiProgram( unsigned char cCommand, unsigned char *pBuffer );
		CMidiProgram( unsigned int iChannel, unsigned char cVal1 );
		virtual ~CMidiProgram();

		//	Channel and complement the code with the channel
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual DWORD GetChunk ();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );
		virtual const char * GetExtraText() const;

		virtual unsigned char GetEventValue1() const;
		virtual void SetEventValue1 ( unsigned char val1 );

		virtual const char *GetEventLabel1 () const
		{
			return "Program";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 1;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiChannelPressure : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiChannelPressure)

	protected :

		//	Code must be 0xD0
		unsigned char		m_cMidiPressure;

	public:
		CMidiChannelPressure( unsigned char cCommand, unsigned char *pBuffer );
		CMidiChannelPressure( unsigned int iChannel, unsigned char cVal1 );
		virtual ~CMidiChannelPressure();

		//	Channel and complement the code with the channel
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual DWORD GetChunk ();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

		virtual unsigned char GetEventValue1() const;
		virtual void SetEventValue1 ( unsigned char val1 );

		virtual const char *GetEventLabel1 () const
		{
			return "Channel Pressure";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 1;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiWheel : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiWheel)

	protected :

		//	Code must be 0xE0
		unsigned char		m_cMidiValue1;
		unsigned char		m_cMidiValue2;

	public:
		CMidiWheel( unsigned char cCommand, unsigned char *pBuffer );
		CMidiWheel( unsigned int iChannel, unsigned char cVal1, 
			unsigned char cVal2 );
		virtual ~CMidiWheel();

		//	Channel and complement the code with the channel
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual DWORD GetChunk ();
		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

		virtual unsigned char GetEventValue1() const;
		virtual unsigned char GetEventValue2() const;

		virtual void SetEventValue1 ( unsigned char val1 );
		virtual void SetEventValue2 ( unsigned char val2 );

		virtual const char *GetEventLabel1 () const
		{
			return "Wheel Value 1";
		}

		virtual const char *GetEventLabel2 () const
		{
			return "Wheel Value 2";
		}

		virtual bool useChannel() const
		{
			return true;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 2;
		};
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiUnknown : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiUnknown)

	protected :

		unsigned char		*m_pMidiString;

	public:
		CMidiUnknown( unsigned char cCommand, unsigned char *pBuffer );
		CMidiUnknown( );
		virtual ~CMidiUnknown();

		virtual void Free();
		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );
		virtual void Save ( FILE *hFile );

};

#endif // !defined(AFX_MIDITEXT_H__3B8FC602_AD51_11D2_8147_444553540000__INCLUDED_)
