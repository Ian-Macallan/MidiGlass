//
///////////////////////////////////////////////////////////////////////////////////////////
// MidiClass.cpp: implementation of the CMidiClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "MidiEvent.h"

IMPLEMENT_DYNAMIC(CMidiClass, CMidiError)

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Instruments table.
//
///////////////////////////////////////////////////////////////////////////////////////////
struct  Struct_Instrument
{
	unsigned int				iNumber;
	char						*pText;
};

typedef	struct Struct_Instrument	MIDI_INSTRUMENTS;

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
static MIDI_INSTRUMENTS	InstrumentsTable [ ] =
{
	0,		"Acoustic Grand Piano",
	1,		"Bright Acoustic Piano",
	2,		"Electric Grand Piano",
	3,		"Honky-tonk Piano",
	4,		"Rhodes Piano",
	5,		"Chorused Piano",
	6,		"Harpsichord",
	7,		"Clavinet",
	8,		"Celesta",
	9,		"Glockenspiel",
	10,		"Music Box",
	11,		"Vibraphone",
	12,		"Marimba",
	13,		"Xylophone",
	14,		"Tubular Bells",
	15,		"Dulcimer",
	16,		"Hammond Organ",
	17,		"Percussive Organ",
	18,		"Rock Organ",
	19,		"Church Organ",
	20,		"Reed Organ",
	21,		"Accordion",
	22,		"Harmonica",
	23,		"Tango Accordion",
	24,		"Acoustic Guitar (nylon)",
	25,		"Acoustic Guitar (steel)",
	26,		"Electric Guitar (jazz)",
	27,		"Electric Guitar (clean)",
	28,		"Electric Guitar (muted)",
	29,		"Overdriven Guitar",
	30,		"Distortion Guitar",
	31,		"Guitar Harmonics ",
	32,		"Acoustic Bass",
	33,		"Electric Bass (finger)",
	34,		"Electric Bass (pick)",
	35,		"Fretless Bass",
	36,		"Slap Bass 1",
	37,		"Slap Bass 2",
	38,		"Synth Bass 1",
	39,		"Synth Bass 2",
	40,		"Violin",
	41,		"Viola ",
	42,		"Cello",
	43,		"Contrabass",
	44,		"Tremolo Strings",
	45,		"Pizzicato Strings",
	46,		"Orchestral Harp",
	47,		"Timpani",
	48,		"String Ensemble 1",
	49,		"String Ensemble 2",
	50,		"SynthStrings 1",
	51,		"SynthStrings 2",
	52,		"Choir Aahs",
	53,		"Voice Oohs",
	54,		"Synth Voice",
	55,		"Orchestra Hit",
	56,		"Trumpet",
	57,		"Trombone ",
	58,		"Tuba",
	59,		"Muted Trumpet",
	60,		"French Horn ",
	61,		"Brass Section",
	62,		"Synth Brass 1",
	63,		"Synth Brass 2",
	64,		"Soprano Sax",
	65,		"Alto Sax",
	66,		"Tenor Sax",
	67,		"Baritone Sax",
	68,		"Oboe",
	69,		"English Horn",
	70,		"Bassoon",
	71,		"Clarinet",
	72,		"Piccolo",
	73,		"Flute",
	74,		"Recorder",
	75,		"Pan Flute",
	76,		"Bottle Blow",
	77,		"Shakuhachi",
	78,		"Whistle",
	79,		"Ocarina",
	80,		"Lead 1 (square)",
	81,		"Lead 2 (sawtooth)",
	82,		"Lead 3 (calliope lead)",
	83,		"Lead 4 (chiff lead)",
	84,		"Lead 5 (charang)",
	85,		"Lead 6 (voice)",
	86,		"Lead 7 (fifths)",
	87,		"Lead 8 (bass + lead)",
	88,		"Pad 1 (new age)",
	89,		"Pad 2 (warm)",
	90,		"Pad 3 (polysynth)",
	91,		"Pad 4 (choir)",
	92,		"Pad 5 (bowed)",
	93,		"Pad 6 (metallic)",
	94,		"Pad 7 (halo)",
	95,		"Pad 8 (sweep)",
	96,		"FX 1 (rain)",
	97,		"FX 2 (soundtrack)",
	98,		"FX 3 (crystal)",
	99,		"FX 4 (atmosphere)",
	100,	"FX 5 (brightness)",
	101,    "FX 6 (goblins)",
	102,	"FX 7 (echoes) ",
	103,	"FX 8 (sci-fi)",
	104,	"Sitar",
	105,	"Banjo",
	106,	"Shamisen",
	107,	"Koto",
	108,	"Kalimba",
	109,	"Bagpipe",
	110,	"Fiddle",
	111,	"Shanai",
	112,	"Tinkle Bell",
	113,	"Agogo",
	114,	"Steel Drums",
	115,	"Woodblock",
	116,	"Taiko Drum",
	117,	"Melodic Tom",
	118,	"Synth Drum ",
	119,	"Reverse Cymbal",
	120,	"Guitar Fret Noise",
    121,	"Breath Noise",
	122,	"Seashore",
	123,	"Bird Tweet",
	124,	"Telephone Ring",
	125,	"Helicopter",
	126,	"Applause",
	127,	"Gunshot",

	0,		NULL
};

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Percussions Table.
//
///////////////////////////////////////////////////////////////////////////////////////////
struct Struct_Percussion
{
	unsigned int				iNumber;
	char						*pText;
};

typedef struct Struct_Percussion MIDI_PERCUSSIONS;

static	MIDI_PERCUSSIONS PercussionsTable [ ] =
{
	0,		"000",
	1,		"001",
	2,		"002",
	3,		"003",
	4,		"004",
	5,		"005",
	6,		"006",
	7,		"007",
	8,		"008",
	9,		"009",
	10,		"010",
	11,		"011",
	12,		"012",
	13,		"013",
	14,		"014",
	15,		"015",
	16,		"016",
	17,		"017",
	18,		"018",
	19,		"019",
	20,		"020",
	21,		"021",
	22,		"022",
	23,		"023",
	24,		"024",
	25,		"025",

	//
	26,		"High Q (GM2)",
    27,		"Slap",
    28,		"Scratch 1",
    29,		"Scratch 2",
    30,		"Sticks",
    31,		"Square Click",
    32,		"Metronome Click",
    33,		"metronome Bell",

	//	START OF GM1
    34,		"Acoustic Bass Drum",
    35,		"Bass Drum 1",
    36,		"Side Stick",
    37,		"Acoustic Snare",
    38,		"Hand Clap",
    39,		"Electric Snare",
    40,		"Low Floor Tom",
    41,		"Closed Hi-Hat",
    42,		"High Floor Tom",
    43,		"Pedal Hi-Hat",
    44,		"Low Tom",
    45,		"Open Hi-Hat",
    46,		"Low-Mid Tom",
    47,		"Hi-Mid Tom",
    48,		"Crash Cymbal",
    49,		"High Tom",
    50,		"Ride Cymbal 1",
    51,		"Chinese Cymbal",
    52,		"Ride Bell",
    53,		"Tambourine",
    54,		"Splash Cymbal",
    55,		"Cowbell",
    56,		"Crash Cymbal 2",
    57,		"Vibraslap",
    58,		"Ride Cymbal 2",
    59,		"Hi Bongo",
    60,		"Low Bongo",
    61,		"Mute Hi Conga",
    62,		"Open Hi Conga",
    63,		"Low Conga",
    64,		"High Timbale",
    65,		"Low Timbale",
    66,		"High Agogo",
    67,		"Low Agogo",
    68,		"Cabasa",
    69,		"Maracas",
    70,		"Short Whistle",
    71,		"Long Whistle",
    72,		"Short Guiro",
    73,		"Long Guiro",
    74,		"Claves",
    75,		"Hi Wood Block",
    76,		"Low Wood Block",
    77,		"Mute Cuica",
    78,		"Open Cuica",
    79,		"Mute Triangle",
    80,		"Open Triangle",
	//	END OF GM1

    81,		"Shaker",
	82,		"Jingle Bell",
    83,		"Bell Tree",
    84,		"Castanets",
    85,		"Mute Surdo",
    86,		"Open Surdo",
    87,		"Slap High Conga",
    88,		"Small Gong",
    89,		"Big Gong",
    90,		"Mute Pandiero",
    91,		"Open Pandiero",
    92,		"tambourin",
    93,		"Tree Chime",
    94,		"caxixi",
	95,		"095",
	96,		"096",
	97,		"097",
	98,		"098",
	99,		"099",
	100,	"100",
	101,	"101",
	102,	"102",
	103,	"103",
	104,	"104",
	105,	"105",
	106,	"106",
	107,	"107",
	108,	"108",
	109,	"109",
	110,	"110",
	111,	"111",
	112,	"112",
	113,	"113",
	114,	"114",
	115,	"115",
	116,	"116",
	117,	"117",
	118,	"118",
	119,	"119",
	120,	"120",
	121,	"121",
	122,	"122",
	123,	"123",
	124,	"124",
	125,	"125",
	126,	"126",
    127,	"High Q",

    0,		NULL
};

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Controllers Table
//
///////////////////////////////////////////////////////////////////////////////////////////
struct Struct_Controlers
{
	unsigned int				iNumber;
	char						*pText;
};

typedef struct Struct_Controlers MIDI_CONTROLERS;

static MIDI_CONTROLERS ControlersTable [ ] =
{
    0x0,	"Bank Select High",
    0x1,	"Modulation Wheel",
    0x2,	"Breath Control",

    0x4,	"Foot Controller",
    0x5,	"Portamento Time",
    0x6,	"Data Entry High",
    0x7,	"Channel Volume (formerly Main Volume)",
    0x8,	"Balance",
    0xA,	"Pan Pot",
    0xB,	"Expression",
    0xC,	"Effect Control 1",
    0xD,	"Effect Control 2",

    0x10,	"General Purpose Controller 1",
    0x11,   "General Purpose Controller 2",
    0x12,   "General Purpose Controller 3",
    0x13,   "General Purpose Controller 4",

    0x20,   "Bank Select Low",
    0x21,   "Modulation Wheel Low",
    0x22,   "Breath Control Low",

    0x24,   "Foot Controller Low",
    0x25,   "Portamento Time Low",
    0x26,   "Data Entry Low",
    0x27,   "Channel Volume Low (formerly Main Volume)",
    0x28,   "Balance Low",

    0x2A,   "Pan Pot Low",
    0x2B,   "Expression Low",
    0x2C,   "Effect control 1 Low",
    0x2D,   "Effect control 2 Low",

    0x30,   "General Purpose Controller 1 Low",
    0x31,   "General Purpose Controller 2 Low",
    0x32,   "General Purpose Controller 3 Low",
    0x33,   "General Purpose Controller 4 Low",

    0x40,   "Damper pedal on/off (Sustain) (Hold)",
    0x41,   "Portamento On/Off",
    0x42,   "Sustenuto on/Off",
    0x43,   "Soft Pedal On/off",
    0x44,   "Legato Footswitch",
    0x45,   "Hold 2",
    0x46,   "Sound Controller 01 (Sound Variation)",
    0x47,   "Sound Controller 02 (Timbre)",
    0x48,   "Sound Controller 03 (Release Time)",
    0x49,   "Sound Controller 04 (Attack Time)",
    0x4A,   "Sound Controller 05 (Brightness)",
    0x4B,   "Sound Controller 06",
    0x4C,   "Sound Controller 07",
    0x4D,   "Sound Controller 08",
    0x4E,   "Sound Controller 09",
    0x4F,   "Sound Controller 10",

    0x50,   "General Purpose Controller 5",
    0x51,   "General Purpose Controller 6",
    0x52,   "General Purpose Controller 7",
    0x53,   "General Purpose Controller 8",
    0x54,   "Portamento Control",

    0x5B,   "Effect Depth - Reverb Send Level",
    0x5C,   "Effect Depth - Tremolo Send Level",
    0x5D,   "Effect Depth - Chorus Send Level",
    0x5E,   "Effect Depth - Celeste Send Level",
    0x5F,   "Effect Depth - Phaser Send Level",

    0x60,   "Data Entry + 1",
    0x61,   "Data Entry - 1",
    0x62,   "Non-Registered Parameter Number LSB",
    0x63,   "Non-Registered Parameter Number MSB",
    0x64,   "Registered Parameter Number LSB",
    0x65,   "Registered Parameter Number MSB",

    0x78,   "All Sound Off",
    0x79,   "Reset All Controllers",
    0x7A,   "Local Control On/Off",
    0x7B,   "All Notes Off",
    0x7C,   "Omni Mode Off (+ all notes off)",
    0x7D,   "Omni Mode On (+ all notes off)",
    0x7E,   "Poly Mode On/Off (Mono + all notes off)",
    0x7F,   "Poly Mode On (incl mono=off +all notes off)",

    0x0,    NULL
};

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Notes Table
//
///////////////////////////////////////////////////////////////////////////////////////////
struct Struct_Notes_Tables
{
	unsigned int				iNumber;
	char						*pTextNormal;
	char						*pTextFlat;
	char						*pTextSharp;
	char						*pTextFrench;
	short						iBemolDiese;
	bool						bBemol;
	bool						bDiese;
	unsigned char				cHeightBemol;
	unsigned char				cHeight;
	unsigned char				cHeightDiese;
};

typedef	struct Struct_Notes_Tables	MIDI_NOTES;

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
static MIDI_NOTES NotesTable [ ] =
{
    0,	"C ",	"C ",	"C ",	"Do   ",	0,	false,	false,	0,	0,	0,
    1,	"C#",	"Db",	"C#",	"Do#  ",	1,	true,	true,	1,	0,	0,
    2,	"D ",	"D ",	"D ",	"Re   ",	0,	false,	false,	1,	1,	1,
    3,	"D# ",	"Eb",	"D#",	"Re#  ",	1,	true,	true,	2,	1,	1,
    4,	"E ",	"E ",	"E ",	"Mi   ",	0,	false,	false,	2,	2,	2,
    5,	"F ",	"F ",	"F ",	"Fa   ",	0,	false,	false,	3,	3,	3,
    6,	"F# ",	"Gb ",	"F#",	"Fa#  ",	1,	true,	true,	4,	3,	3,
    7,	"G ",	"G ",	"G ",	"Sol  ",	0,	false,	false,	4,	4,	4,
    8,	"G# ",	"Ab",	"G#",	"Sol# ",	1,	true,	true,	5,	4,	4,
    9,	"A ",	"A ",	"A ",	"La   ",	0,	false,	false,	5,	5,	5,
    10,	"Bb",	"Bb",	"A#",	"La#  ",	2,	true,	true,	6,	6,	5,
    11,	"B ",	"B ",	"B ",	"Si   ",	0,	false,	false,	6,	6,	6,
    0,	NULL,	NULL,	NULL,	NULL,		0,	false,	false,	0,	0,	0
};

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Event Table
//
///////////////////////////////////////////////////////////////////////////////////////////
struct Struct_Event_Tables
{
	unsigned int				iNumber;
	char						*pText;
};

typedef	struct Struct_Event_Tables	MIDI_EVENTS;

MIDI_EVENTS EventsTable [ ] =
{
	MIDI_EVENT_UNKNOWN,			"Unknown Event",
	MIDI_EVENT_SEQUENCE,		"Sequence",
	MIDI_EVENT_TEXT,			"Text",
	MIDI_EVENT_COPYRIGHT,		"Copyright",
	MIDI_EVENT_TRACKNAME,		"Track Name",
	MIDI_EVENT_INSTRNAME,		"Instrument Name",
	MIDI_EVENT_LYRICS,			"Lyrics",
	MIDI_EVENT_MARKERS,			"Markers",
	MIDI_EVENT_CUEPOINT,		"Cue Point",
	MIDI_EVENT_SEQUENCER,		"Sequencer",
	MIDI_EVENT_SYSEX,			"System Exclusive",
	MIDI_EVENT_STARTTRACK,		"Start Of Track",
	MIDI_EVENT_STARTTRACK20,	"Start Of Track 0x20",
	MIDI_EVENT_ENDOFTRACK,		"End Of Track", 
	MIDI_EVENT_SETTEMPO,		"Set Tempo",
	MIDI_EVENT_SETSMPTE,		"Set SMPTE",
	MIDI_EVENT_TIMESIG,			"Time Signature",	
	MIDI_EVENT_KEYSIG,			"Key Signature",
	MIDI_EVENT_NOTEOFF,			"Note Off",
	MIDI_EVENT_NOTEON,			"Note On",
	MIDI_EVENT_KEYPRESSURE,		"Key pressure",
	MIDI_EVENT_PARAMETER,		"Parameter",
	MIDI_EVENT_PROGRAM,			"Program Change",
	MIDI_EVENT_CHANNELPRES,		"Channel Pressure",
	MIDI_EVENT_WHEEL,			"Wheel",
	MIDI_EVENT_UNKNOWNTEXT,		"Unknown Text",
	0,							NULL
};

//
///////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
///////////////////////////////////////////////////////////////////////////////////////////
CMidiClass::CMidiClass()
{
	m_iMidiClassLength			= 0;
	memset ( m_MidiClassBuffer, 0, sizeof ( m_MidiClassBuffer ) );
	m_iMidiClassValue			= 0;
	m_iMidiClassCoded			= 0;

	m_MidiClassStartOfBuffer	= NULL;
	m_MidiClassEndOfBuffer		= NULL;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CMidiClass::~CMidiClass()
{
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned long CMidiClass::ConvertText2Value( const unsigned char *pBuffer)
{
    unsigned char   ucByte;

    m_iMidiClassLength = 0;

    ucByte = *pBuffer;
	pBuffer++;
    m_iMidiClassLength++;

    m_iMidiClassValue = ( unsigned long ) ucByte;
    if ( m_iMidiClassValue & 0x80 )
	{
        m_iMidiClassValue &= 0x7f;
		do
		{
            ucByte = *pBuffer;
			pBuffer++;
            m_iMidiClassLength++;
            m_iMidiClassValue = ( m_iMidiClassValue << 7 ) + ( ucByte & 0x7f );
        }
        while ( ucByte & 0x80 );
	}

    return ( m_iMidiClassValue );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char *CMidiClass::ConvertValue2Text ( unsigned long iVal )
{
	unsigned char	szTemp [ MAX_INT_STRING ];
	unsigned char	*pBuffer;

	pBuffer = szTemp;

    m_iMidiClassLength = 0;

	m_iMidiClassValue = iVal;

	/*
	 *		First convert the value to a midi coded value.
	 */
	iVal = ConvertValue2Midi ( iVal );

	/*
	 *		Then put it in a string.
	 */
    while ( true )
    {
        *pBuffer = * ( ( char * )( & iVal ) );
        pBuffer++;
        m_iMidiClassLength++;

        if ( iVal & 0x80 )
        {
            iVal >>= 8;
        }
        else
        {
            break;
        }
    }

	memset ( m_MidiClassBuffer, 0, sizeof ( m_MidiClassBuffer ) );
	memcpy ( m_MidiClassBuffer, szTemp, m_iMidiClassLength );

    return ( m_MidiClassBuffer );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned long CMidiClass::ConvertValue2Midi(unsigned long iVal)
{

	m_iMidiClassValue = iVal;

    m_iMidiClassCoded = iVal & 0x7f;
    while ( ( iVal >>= 7 ) > 0 )
    {
        m_iMidiClassCoded <<= 8;
        m_iMidiClassCoded |= 0x80;
        m_iMidiClassCoded += ( iVal & 0x7f );
    }

	return ( m_iMidiClassCoded );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned long CMidiClass::Swap_Long_Endian(unsigned char *pBuffer)
{
    unsigned char		cTemp0;
    unsigned char		cTemp1;

    cTemp0		= pBuffer[0];
    cTemp1		= pBuffer[1];

    pBuffer[0]	= pBuffer[3];
    pBuffer[1]	= pBuffer[2];

    pBuffer[2]	= cTemp1;
    pBuffer[3]	= cTemp0;

    return ( * ( ( unsigned long *) pBuffer ) );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned short CMidiClass::Swap_Short_Endian(unsigned char *pBuffer)
{
    unsigned char		cTemp;

    cTemp		= pBuffer[0];
    pBuffer[0]	= pBuffer[1];
    pBuffer[1]	= cTemp;

    return ( * ( ( unsigned short *) pBuffer ) );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//      Controller text.
//
///////////////////////////////////////////////////////////////////////////////////////////
const char *CMidiClass::ControlerText ( unsigned int iControler )
{
    unsigned int        iX;

    iX = 0;
    while ( ControlersTable [ iX ].pText != NULL )
    {
        if ( ControlersTable [ iX ].iNumber == iControler )
        {
            return ( ControlersTable [ iX ].pText );
        }
        iX++;
    }

    return "";

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//      Instrument text.
//
///////////////////////////////////////////////////////////////////////////////////////////
const char *CMidiClass::InstrumentText ( unsigned int iInstrument )
{
    unsigned int        iX;

    iX = 0;
    while ( InstrumentsTable [ iX ].pText != NULL )
    {
        if ( InstrumentsTable [ iX ].iNumber == iInstrument )
        {
            return ( InstrumentsTable [ iX ].pText );
        }
        iX++;
    }

    return "";

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//      Drum text.
//
///////////////////////////////////////////////////////////////////////////////////////////
const char *CMidiClass::DrumText ( unsigned int iDrum )
{
    unsigned int        iX;

    iX = 0;
    while ( PercussionsTable [ iX ].pText != NULL )
    {
        if ( PercussionsTable [ iX ].iNumber == iDrum )
        {
            return ( PercussionsTable [ iX ].pText );
        }
        iX++;
    }

    return "";

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//      Note text.
///////////////////////////////////////////////////////////////////////////////////////////
char *CMidiClass::NoteText ( unsigned int iNote ) const
{
	return ( NotesTable [ iNote % 12 ].pTextFlat );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
const char *CMidiClass::NoteFullText ( unsigned int iNote )
{
	static char szNoteFullText [ 64 ];

	if ( iNote >= 0 && iNote <= 127 )
	{
		sprintf_s ( szNoteFullText, sizeof(szNoteFullText), "%s #%d", NotesTable [ iNote % 12 ].pTextFlat, iNote / 12 );
		return ( szNoteFullText );
	}

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
char *CMidiClass::NoteTextFrench ( unsigned int iNote ) const
{
	return ( NotesTable [ iNote % 12 ].pTextFrench );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
int CMidiClass::NoteLevel ( unsigned int iNote ) const
{
	int             iLevel;

	iLevel = ( int ) ( iNote / 12 );

	return ( iLevel );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get midi class length
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned long CMidiClass::GetMidiClassLength ( unsigned long iVal )
{
	unsigned char	szTemp [ MAX_INT_STRING ];
	unsigned char	*pBuffer;

	pBuffer			= szTemp;

    m_iMidiClassLength = 0;

	m_iMidiClassValue = iVal;

	/*
	 *		First convert the value to a midi coded value.
	 */
	iVal = ConvertValue2Midi ( iVal );

	/*
	 *		Then put it in a string.
	 */
    while ( true )
    {
        *pBuffer = * ( ( char * )( & iVal ) );
        pBuffer++;
        m_iMidiClassLength++;

        if ( iVal & 0x80 )
        {
            iVal >>= 8;
        }
        else
        {
            break;
        }
    }

	memset ( m_MidiClassBuffer, 0, sizeof ( m_MidiClassBuffer ) );
	memcpy ( m_MidiClassBuffer, szTemp, m_iMidiClassLength );

	return ( m_iMidiClassLength );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get Small Text associated to events
//
///////////////////////////////////////////////////////////////////////////////////////////
const unsigned char * CMidiClass::GetEventSmallText(unsigned short iEvent)
{
    unsigned int        iX;

    iX = 1;
    while ( EventsTable [ iX ].pText != NULL )
    {
        if ( EventsTable [ iX ].iNumber == iEvent )
        {
            return ( ( unsigned char * ) EventsTable [ iX ].pText );
        }
        iX++;
    }

    return ( ( unsigned char * ) EventsTable [ 0 ].pText );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get the small text associated to the index.
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMidiClass::GetClassEventText(unsigned short iX) const
{
	return ( EventsTable [ iX ].pText );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get the controler text associated with an index
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMidiClass::GetClassControlerText(unsigned short iX) const
{
	return ( ControlersTable [ iX ].pText );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get the controler number associated with an index
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned int CMidiClass::GetClassControlerNum(unsigned int iX) const
{
	return ( ControlersTable [ iX ].iNumber );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Search the controler number associated with a text
//
///////////////////////////////////////////////////////////////////////////////////////////
int CMidiClass::SearchControlerByText( const char *pText ) const
{
	int		iX;

	iX = 0;
	while ( ControlersTable [ iX ].pText != NULL )
	{
		if ( strcmp ( pText, ControlersTable [ iX ].pText ) == 0 )
		{
			return ( ControlersTable [ iX ].iNumber );
		}
		iX++;
	}

	return -1;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get the instrument by numbers
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMidiClass::GetInstrumentText(unsigned int iNum) const
{
    unsigned int        iX;

    iX = 0;
    while ( InstrumentsTable [ iX ].pText != NULL )
    {
        if ( InstrumentsTable [ iX ].iNumber == iNum )
        {
            return ( InstrumentsTable [ iX ].pText );
        }

		if ( InstrumentsTable [ iX ].iNumber > iNum )
		{
			return "None";
		}
        iX++;
    }

    return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMidiClass::GetControlerText(unsigned int iNum) const
{
    unsigned int        iX;

    iX = 0;
    while ( ControlersTable [ iX ].pText != NULL )
    {
        if ( ControlersTable [ iX ].iNumber == iNum )
        {
            return ( ControlersTable [ iX ].pText );
        }

		if ( ControlersTable [ iX ].iNumber > iNum )
		{
			return "None";
		}
        iX++;
    }

    return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMidiClass::GetPercussionText(unsigned int iNum) const
{
    unsigned int        iX;

    iX = 1;
    while ( PercussionsTable [ iX ].pText != NULL )
    {
        if ( PercussionsTable [ iX ].iNumber == iNum )
        {
            return ( PercussionsTable [ iX ].pText );
        }

		if ( PercussionsTable [ iX ].iNumber > iNum )
		{
			return "None";
		}
        iX++;
    }

    return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned int CMidiClass::GetIndex(DWORD dwChunk) const
{
	switch ( dwChunk & OPERATION_MASK )
	{
		case MIDI_EVENT_CODE_NOTEOFF:
		{
			return MIDI_EVENT_NOTEOFF;
		}
		case MIDI_EVENT_CODE_NOTEON:
		{
			return MIDI_EVENT_NOTEON;
		}
		case MIDI_EVENT_CODE_KEYPRES :
		{
			return MIDI_EVENT_KEYPRESSURE;
		}
		case MIDI_EVENT_CODE_PARAM:
		{
			return MIDI_EVENT_PARAMETER;
		}
		case MIDI_EVENT_CODE_PROGRAM:
		{
			return MIDI_EVENT_PROGRAM;
		}
		case MIDI_EVENT_CODE_CHANPRESS:
		{
			return MIDI_EVENT_CHANNELPRES;
		}
		case MIDI_EVENT_CODE_WHEEL:
		{
			return MIDI_EVENT_WHEEL;
		}
	}

	return MIDI_EVENT_UNKNOWN;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsBemol(unsigned int iNote) const
{
	return ( NotesTable [ iNote % 12 ].bBemol );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsDiese(unsigned int iNote) const
{
	return ( NotesTable [ iNote % 12 ].bDiese );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiClass::GetHeight(unsigned int iNote) const
{
	return ( NotesTable [ iNote % 12 ].cHeightBemol );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned short CMidiClass::SearchClassEventText(const char *pText) const
{
	int				iX;

	iX = 0;
	while ( EventsTable [ iX ].pText != NULL )
	{
		if ( strcmp ( EventsTable [ iX ].pText, pText ) == 0 )
		{
			return  EventsTable [ iX ].iNumber;
		}

		iX++;
	}

	return MIDI_EVENT_UNKNOWN;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Search the program number associated with a text
//
///////////////////////////////////////////////////////////////////////////////////////////
int CMidiClass::SearchProgramByText( const char *pText ) const
{
	int		iX;

	iX = 0;
	while ( InstrumentsTable [ iX ].pText != NULL )
	{
		if ( strcmp ( pText, InstrumentsTable [ iX ].pText ) == 0 )
		{
			return ( InstrumentsTable [ iX ].iNumber );
		}
		iX++;
	}

	return -1;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned CMidiClass::ConvertAscii2Hexa(const char *pText, char *pHexa)
{
	unsigned		iLen = 0;
	char			cByte;

	*pHexa			= 0;

	while ( *pText )
	{
		cByte = *pText;
		if ( cByte >= 'a' && cByte <= 'f' )
		{
			cByte -= 0x20;
		}

		if ( cByte >= '0' && cByte <= '9' )
		{
			*pHexa = ( *pHexa ) << 4;
			*pHexa += ( cByte - '0' );
		}

		else if ( cByte >= 'A' && cByte <= 'F' )
		{
			*pHexa = ( *pHexa ) << 4;
			*pHexa += ( cByte - 'A' + 10 );
		}
		else
		{
			pHexa++;
			*pHexa = 0;
			iLen++;
		}

		pText++;
	}

	return iLen;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiClass::ConvertHexa2Ascii(	const char *pHexa, unsigned int iLen,
									char *pText, size_t iText )
{
	memset ( pText, 0, iText );
	unsigned char		cByte;

	while ( iLen > 0 )
	{
		cByte = ( unsigned char ) *pHexa;
		sprintf_s ( pText + strlen(pText), iText - strlen(pText), "%02x ", cByte & 0xff );
		pHexa++;
		iLen--;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMidiClass::GetFileExtension(char *pFilename)
{
	char		*pExtension;
	if ( *pFilename == '\0' )
	{
		return pFilename;
	}

	pExtension = pFilename + strlen ( pFilename ) - 1;
	while ( pExtension >= pFilename )
	{
		if ( *pExtension == '.' )
		{
			return pExtension;
		}
		pExtension--;
	}

	return ( pFilename + strlen ( pFilename ) );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsMidFile(const char *pFilename)
{
	return IsTypeFile (pFilename, ".mid" );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsKarFile(const char *pFilename)
{
	return IsTypeFile (pFilename, ".kar" );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsRmiFile(const char *pFilename)
{
	return IsTypeFile (pFilename, ".rmi" );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsMidiFile(const char *pFilename)
{
	if ( IsMidFile ( pFilename ) )
	{
		return true;
	}

	if ( IsKarFile ( pFilename ) )
	{
		return true;
	}

	if ( IsRmiFile ( pFilename ) )
	{
		return true;
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiClass::ConvertToUpperCase(char *pString)
{
	while ( *pString )
	{
		if ( *pString >= 'a' && *pString <= 'z' )
		{
			*pString = *pString - 0x20;
		}
		pString++;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiClass::ConvertToLowerCase(char *pString)
{
	while ( *pString )
	{
		if ( *pString >= 'A' && *pString <= 'Z' )
		{
			*pString = *pString + 0x20;
		}
		pString++;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiClass::IsTypeFile(const char *pFilename, const char *pExtension)
{
	char		szFilename [ MAX_PATHNAME_STRING ];
	char		szExtension [ MAX_PATHNAME_STRING ];
	char		szFileExtension [ MAX_PATHNAME_STRING ];

	strcpy_s (	szFilename, sizeof ( szFilename ), pFilename );
	strcpy_s (	szFileExtension, sizeof ( szFileExtension ), GetFileExtension ( szFilename ) );
	strcpy_s (	szExtension, sizeof ( szExtension ), pExtension );

	ConvertToLowerCase ( szFileExtension );
	ConvertToLowerCase ( szExtension );

	if ( strcmp ( szFileExtension, szExtension ) == 0 )
	{
		return true;
	}

	return false;
}
