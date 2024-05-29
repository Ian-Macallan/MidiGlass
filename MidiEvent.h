//
////////////////////////////////////////////////////////////////////////////////////
// MidiEvent.h: interface for the CMidiEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDIEVENT_H__3B8FC601_AD51_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIEVENT_H__3B8FC601_AD51_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiClass.h"
#include "mmsystem.h"

//	https://www.lim.di.unimi.it/IEEE/MIDI/INDEX.HTM
#define	MIDI_EVENT_CODE_NOTEOFF			0x80
#define	MIDI_EVENT_CODE_NOTEON			0x90
#define	MIDI_EVENT_CODE_KEYPRES			0xA0
#define	MIDI_EVENT_CODE_PARAM			0xB0
#define	MIDI_EVENT_CODE_PROGRAM			0xC0
#define	MIDI_EVENT_CODE_CHANPRESS		0xD0
#define	MIDI_EVENT_CODE_WHEEL			0xE0
#define	MIDI_EVENT_CODE_SYSEX			0xF0

#define	MIDI_EVENT_CODE_META			0xFF

//	FF 00 02 ssss Sequence Number
#define	MIDI_EVENT_CODE_META_SEQ_1		0x00
#define	MIDI_EVENT_CODE_META_SEQ_2		0x02

//	FF 01 len text Text Event
#define	MIDI_EVENT_CODE_META_TEXT		0x01

//	FF 02 len text Copyright Notice
#define	MIDI_EVENT_CODE_META_COPY		0x02

//	FF 03 len text Sequence/Track Name
#define	MIDI_EVENT_CODE_META_TRKNAME	0x03

//	FF 04 len text Instrument Name
#define	MIDI_EVENT_CODE_META_INSNAME	0x04

//	FF 05 len text Lyric
#define	MIDI_EVENT_CODE_META_LYRICS		0x05

//	FF 06 len text Marker
#define	MIDI_EVENT_CODE_META_MARKER		0x06

//	FF 07 len text Cue Point
#define	MIDI_EVENT_CODE_META_CUEPOINT	0x07

//	FF 7F len data Sequencer-Specific Meta-Event
#define	MIDI_EVENT_CODE_META_SEQR		0x7f

//	https://www.lim.di.unimi.it/IEEE/MIDI/META.HTM
//	FF 20 01 cc MIDI Channel Prefix
#define	MIDI_EVENT_CODE_META_ST20_1		0x20
#define	MIDI_EVENT_CODE_META_ST20_2		0x01

//	
#define	MIDI_EVENT_CODE_META_START_1	0x21
#define	MIDI_EVENT_CODE_META_START_2	0x01

//	FF 2F 00 End of Track
#define	MIDI_EVENT_CODE_META_EOT_1		0x2F
#define	MIDI_EVENT_CODE_META_EOT_2		0x00

//	FF 51 03 tttttt Set Tempo
#define	MIDI_EVENT_CODE_META_TEMPO_1	0x51
#define	MIDI_EVENT_CODE_META_TEMPO_2	0x03

//	FF 54 05 hr mn se fr ff SMPTE Offset
#define	MIDI_EVENT_CODE_META_SMPTE_1	0x54
#define	MIDI_EVENT_CODE_META_SMPTE_2	0x05

//	FF 58 04 nn dd cc bb Time Signature
#define	MIDI_EVENT_CODE_META_TIMSIG_1	0x58
#define MIDI_EVENT_CODE_META_TIMSIG_2	0x04

//	FF 59 02 sf mi Key Signature
#define	MIDI_EVENT_CODE_META_KEYSIG_1	0x59
#define	MIDI_EVENT_CODE_META_KEYSIG_2	0x02

//
#define	PERCUSSION_CHANNEL				0x09
#define	CHANNEL_MASK					0x0f
#define	OPERATION_MASK					0xf0

//
////////////////////////////////////////////////////////////////////////////////////
//	Controller
//	https://professionalcomposers.com/midi-cc-list/
//	https://anotherproducer.com/online-tools-for-musicians/midi-cc-list/
////////////////////////////////////////////////////////////////////////////////////
#define	CNTR_BANK_SELECT_MSB				0x00
#define	CNTR_MODULATION_WHEEL				0x01
#define	CNTR_BREATH_CONTROLLER				0x02

#define	CNTR_FOOT_PEDAL_MSB					0x04
#define	CNTR_PORTAMENTO_TIME_MSB			0x05
#define	CNTR_DATA_ENTRY_MSB					0x06
#define	CNTR_VOLUME_MSB						0x07
#define	CNTR_BALANCE_MSB					0x08

#define	CNTR_PAN_POSITION_MSB				0x0a
#define	CNTR_EXPRESSION_MSB					0x0b
#define	CNTR_EFFECT_CONTROL_1_MSB			0x0c
#define	CNTR_EFFECT_CONTROL_2_MSB			0x0d

#define	CNTR_GENERAL_PURPOSE_01				0x10
#define	CNTR_GENERAL_PURPOSE_02				0x11
#define	CNTR_GENERAL_PURPOSE_03				0x12
#define	CNTR_GENERAL_PURPOSE_04				0x13

#define	CNTR_BANK_SELECT_LSB				0x20
#define	CNTR_CONTROLLER_01					0x20
#define	CNTR_CONTROLLER_31					0x3f

#define	CNTR_HOLD_PEDAL_ON_OFF				0x40
#define	CNTR_PORTAMENTO_ON_OFF				0x41
#define	CNTR_SOSTENUTO_PEDAL_ON_OFF			0x42
#define	CNTR_SOFT_PEDAL_ON_OFF				0x43
#define	CNTR_LEGATO_PEDAL_ON_OFF			0x44
#define	CNTR_HOLD_2_PEDAL_ON_OFF			0x45

#define	CNTR_SOUND_VARIATION				0x46
#define	CNTR_RESONANCE_TIMBRE				0x47
#define	CNTR_SOUND_RELEASE_TIME				0x48
#define	CNTR_SOUND_ATTACK_TIME				0x49
#define	CNTR_FREQUENCY_CUTOFF_BRIGHTNESS	0x4a

#define	CNTR_SOUND_CONTROL_06				75
#define	CNTR_SOUND_CONTROL_07				76
#define	CNTR_SOUND_CONTROL_08				77
#define	CNTR_SOUND_CONTROL_09				78
#define	CNTR_SOUND_CONTROL_10				79

#define	CNTR_GENERAL_PURPOSE_BUTTON_1		80
#define	CNTR_GENERAL_PURPOSE_BUTTON_2		81
#define	CNTR_GENERAL_PURPOSE_BUTTON_3		82
#define	CNTR_GENERAL_PURPOSE_BUTTON_4		83
#define	CNTR_PORTAMENTO_AMOUNT				84

#define	CNTR_REVERB_LEVEL					91
#define	CNTR_TREMOLO_LEVEL					92
#define	CNTR_CHORUS_LEVEL					93
#define	CNTR_DETUNE_LEVEL					94
#define	CNTR_PHASER_LEVEL					95
#define	CNTR_DATA_BUTTON_INCREMENT			96
#define	CNTR_DATA_BUTTON_DECREMENT			97
#define	CNTR_NON_REGISTERED_PARAMETER_LSB	98
#define	CNTR_NON_REGISTERED_PARAMETER_MSB	99
#define	CNTR_REGISTERED_PARAMETER_LSB		100
#define	CNTR_REGISTERED_PARAMETER_MSB		101

#define	CNTR_ALL_SOUND_OFF					0x78
#define	CNTR_ALL_CONTROLLERS_OFF			0x79
#define	CNTR_LOCAL_KEYBOARD_ON_OFF_			0x7a
#define	CNTR_ALL_NOTES_OFF					0x7b
#define	CNTR_OMNI_MODE_OFF					0x7c
#define	CNTR_OMNI_MODE_ON					0x7d
#define	CNTR_MONO_OPERATION					0x7e
#define	CNTR_POLY_MODE						0x7f

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiEvent : public CMidiClass  
{
	DECLARE_DYNAMIC(CMidiEvent)

	friend class MidiPiece;

	protected:
		//	Event Number
		unsigned int		m_iEventIndex;

		//	Code Logical OR channel
		unsigned char		m_cMidiEventCode;

		unsigned char		m_cMidiEventSubCode;

		//	channel & 0x0f
		unsigned char		m_cMidiEventChannel;

		//
		DWORD				m_dwMidiChunk;

		//	For Parsing
		unsigned char		*m_MidiEventStartOfBuffer;
		unsigned char		*m_MidiEventEndOfBuffer;

	public:
		CMidiEvent();
		virtual ~CMidiEvent();

		void EventMessageBox(const char *pText );

		unsigned short GetSubCodeMask() const;

		virtual unsigned int GetEventIndex() const;
		virtual unsigned char GetChannel () const;

		virtual unsigned char GetCode () const;
		virtual unsigned char GetSubCode() const;
		virtual unsigned char GetCodeAndChannel() const;

		virtual MIDIHDR * GetSysexChunk();
		virtual DWORD GetChunk ();

		virtual unsigned char *GetEndOfBufferAddress () const;

		virtual const char * GetExtraText() const;

		virtual unsigned char GetEventValue1 () const;
		virtual unsigned char GetEventValue2 () const;
		virtual unsigned char GetEventValue3 () const;
		virtual unsigned char GetEventValue4 () const;
		virtual unsigned char GetEventValue5 () const;
		virtual unsigned char GetEventValue6 () const;

		virtual void Free() = 0;
		virtual void Trace ( FILE *hFile ) = 0;
		virtual void Save ( FILE *hFile ) = 0;
		virtual const unsigned char *GetEventText () const = 0;
		virtual unsigned char *GetTextAddress ( unsigned short cSubCode = MASK_ALL ) const;

		//
		virtual void SetEventIndex ( unsigned int event );
		virtual void SetChannelAndCode ( unsigned char cChannel );
		virtual void SetText ( const unsigned char *pBuffer );
		virtual void SetEventValue1 ( unsigned char val1 );
		virtual void SetEventValue2 ( unsigned char val2 );
		virtual void SetEventValue3 ( unsigned char val3 );
		virtual void SetEventValue4 ( unsigned char val4 );
		virtual void SetEventValue5 ( unsigned char val5 );
		virtual void SetEventValue6 ( unsigned char val6 );
		virtual void SetCodeAndChannel ( unsigned char code );
		virtual void SetSubCode( unsigned char  subCode );

		virtual const char *GetEventLabel1 () const
		{
			return "";
		}
		virtual const char *GetEventLabel2 () const
		{
			return "";
		}
		virtual const char *GetEventLabel3 () const
		{
			return "";
		}
		virtual const char *GetEventLabel4 () const
		{
			return "";
		}
		virtual const char *GetEventLabel5 () const
		{
			return "";
		}
		virtual const char *GetEventLabel6 () const
		{
			return "";
		}

		virtual bool useChannel() const
		{
			return false;
		}

		virtual bool usePort() const
		{
			return false;
		}

		virtual int valueCount() const
		{
			return 0;
		}
};

#endif // !defined(AFX_MIDIEVENT_H__3B8FC601_AD51_11D2_8147_444553540000__INCLUDED_)
