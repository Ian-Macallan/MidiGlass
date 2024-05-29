#if !defined(Midi_Definition_File_Included)
#define Midi_Definition_File_Included

//		The Timer used for updating time
#define	MIDI_TIMER					1
#define	CHANNEL_TIMER				2
#define	PLAY_THREAD_TIMER			3
#define	CORRECT_THREAD_TIMER		4
#define	EXPLORER_VIEW_TIMER			5
#define	RECORD_THREAD_TIMER			6
#define	INPUT_DIALOG_TIMER			7

//		The Maximum number of system exclusive lines
#define	MAX_SYSEX_LINES				64

//		The Maximum number of controllers
#define	MAX_CONTROLERS				256

//		The maximum number of channels
#define	MAX_CHANNELS				16

//		The maximum number of columns
#define	MAX_COLUMNS					256

//		The maximum number of notes inside a mesure
#define	MAX_INSIDE_MESURE			1024

//		Define MASKs
#define	MASK_SEQUENCE				0x0001
#define	MASK_TEXT					0x0002
#define	MASK_COPYRIGHT				0x0004
#define	MASK_TRACKNAME				0x0008
#define	MASK_INSTRNAME				0x0010
#define	MASK_LYRICS					0x0020
#define	MASK_MARKERS				0x0040
#define	MASK_CUEPOINT				0x0080
#define	MASK_SEQUENCER				0x0100
#define	MASK_OTHER					0x0200
#define	MASK_ALL					0xffff

//		Do not change the numbers
//		They are used in the image list
#define	MIDI_EVENT_UNKNOWN			0
#define	MIDI_EVENT_SEQUENCE			1
#define	MIDI_EVENT_TEXT				2
#define	MIDI_EVENT_COPYRIGHT		3
#define	MIDI_EVENT_TRACKNAME		4
#define	MIDI_EVENT_INSTRNAME		5
#define	MIDI_EVENT_LYRICS			6
#define	MIDI_EVENT_MARKERS			7
#define	MIDI_EVENT_CUEPOINT			8
#define	MIDI_EVENT_SEQUENCER		9
#define	MIDI_EVENT_SYSEX			10
#define	MIDI_EVENT_STARTTRACK		11
#define	MIDI_EVENT_STARTTRACK20		12
#define	MIDI_EVENT_ENDOFTRACK		13
#define	MIDI_EVENT_SETTEMPO			14
#define	MIDI_EVENT_SETSMPTE			15
#define	MIDI_EVENT_TIMESIG			16
#define	MIDI_EVENT_KEYSIG			17
#define	MIDI_EVENT_NOTEOFF			18
#define	MIDI_EVENT_NOTEON			19
#define	MIDI_EVENT_KEYPRESSURE		20
#define	MIDI_EVENT_PARAMETER		21
#define	MIDI_EVENT_PROGRAM			22
#define	MIDI_EVENT_CHANNELPRES		23
#define	MIDI_EVENT_WHEEL			24
#define	MIDI_EVENT_UNKNOWNTEXT		25
#define	MIDI_EVENT_PLAYABLE			100
#define	MIDI_EVENT_ALL_EVENTS		0xffff

#define	MIDI_PLAYER_STOPPED			0
#define	MIDI_PLAYER_PLAYING			1
#define	MIDI_PLAYER_PAUSED			2
#define	MIDI_RECORDER_INACTIVE		3
#define	MIDI_RECORDER_RECORDING		4

#define	MAX_TRACKS					256

//		View masks
#define	LEFT_MASK_TRACKS			0X00100000
#define	LEFT_MASK_EXPLORER			0X00200000

#define	LEFT_MASK_ALL				0xfff00000

#define	RIGHT_MASK_TRACKS			0x00000001
#define	RIGHT_MASK_HEADER			0x00000002
#define	RIGHT_MASK_EVENTS			0x00000004
#define	RIGHT_MASK_TEXT				0x00000008
#define	RIGHT_MASK_PLAYER			0X00000010
#define	RIGHT_MASK_ERRORS			0x00000020
#define	RIGHT_MASK_PLAYLIST			0X00000040
#define	RIGHT_MASK_CHANNELS			0X00000080
#define	RIGHT_MASK_BROWSER			0x00000100
#define	RIGHT_MASK_STAFF			0x00000200
#define	RIGHT_MASK_NOTES			0x00000400
#define	RIGHT_MASK_ALL				0x000fffff

//		View numbers for the registry
#define	LEFT_VIEW_TRACKS			0

#define	LEFT_VIEW_EXPLORER			10

#define	RIGHT_VIEW_HEADER			0

#define	RIGHT_VIEW_EVENTS			10

#define	RIGHT_VIEW_TEXT				20
#define	RIGHT_VIEW_COPYRIGHT		21
#define	RIGHT_VIEW_CUEPOINT			22
#define	RIGHT_VIEW_INSTRNAME		23
#define	RIGHT_VIEW_LYRICS			24
#define	RIGHT_VIEW_MARKERS			25
#define	RIGHT_VIEW_SEQUENCE			26
#define	RIGHT_VIEW_SEQUENCER		27
#define	RIGHT_VIEW_TRACKNAME		28

#define	RIGHT_VIEW_TRACKS			30

#define	RIGHT_VIEW_PLAYER			40

#define	RIGHT_VIEW_ERRORS			50

#define	RIGHT_VIEW_PLAYLIST			60

#define	RIGHT_VIEW_CHANNELS			70

// #define	RIGHT_VIEW_BROWSER			80

#define	RIGHT_VIEW_STAFF			90

#define	RIGHT_VIEW_NOTES			100

//		Actions
#define		ACTION_NOACTION			0
#define		ACTION_BACKWARD			1
#define		ACTION_PLAY				2
#define		ACTION_PLAY_NEXT		3
#define		ACTION_PAUSE			4
#define		ACTION_STOP				5
#define		ACTION_FORWARD			6

//		Maximum number of lines
#define	MAX_LINES					250000

//		Minumum wait time
#define	MIN_WAIT_TIME				1
#define	SLEEP_TIME_SYSEX			250

//		Slide delta
#define	SLIDE_SECONDS				30

//		The number of tools
#define	TOOLS_NUMBER				20

//		The great string
#define	MAX_LONG_STRING				256
#define	MAX_STRING					128
#define	MAX_SHORT_STRING			64
#define	MAX_SMALL_STRING			32

//		The text associated with an integer
#define	MAX_INT_STRING				32

//		The text "Track: nnn"
#define	MAX_TRACK_STRING			32

//		The text "Controler: nnn"
//		or controler text
#define	MAX_CONTROLER_STRING		128 

//		The program / instrument
#define	MAX_INSTRUMENT_STRING		128 

//		The text "Tune: nnn"
#define	MAX_TUNE_STRING				32

//		The pathname 
#define	MAX_PATHNAME_STRING			256

//		The hour:minutes:seconds
#define	MAX_HOUR_STRING				32

//		The mesure string
#define	MAX_MESURE_STRING			32

//		The note string
#define	MAX_NOTE_STRING				64

//		The menu string
#define	MAX_MENU_STRING				128

//		The registry values
#define	MAX_REGISTRY_STRING			128

//		Application Messages
#define	WM_MW_EDIT_BEGIN			(WM_APP+1)
#define	WM_MW_EDIT_END				(WM_APP+2)
#define	WM_MW_EDIT_GET_TEXT			(WM_APP+3)
#define	WM_MW_EDIT_SET_TEXT			(WM_APP+4)
#define	WM_MW_EDIT_SELECT			(WM_APP+5)

#define	WM_MW_LISTCTRL_END			(WM_APP+10)
#define	WM_MW_LISTBOX_END			(WM_APP+11)
#define	WM_MW_LISTEDITCTRL_END		(WM_APP+12)
#define	WM_MW_LISTEDITBOX_END		(WM_APP+13)

#define	WM_MW_EVENT_OCCURS			(WM_APP+20)

//		Note duration
#define	NOTE_RONDE					480
#define	NOTE_BLANCHE				240
#define	NOTE_NOIRE					120
#define	NOTE_CROCHE					 60
#define	NOTE_DOUBLE_CROCHE			 30
#define	NOTE_TRIPLE_CROCHE			 15

#define	MAX_NOTE_CX					3
#define	MAX_NOTE_CY					3

#define	MAX_BLACK_DIVISION			32

#define	MAX_QUARTER_IN_BLACK		32

#define	MAX_NOTES					128

#define	MAX_BEAT_MESURE				16

#define	MAX_PLAYFILES				4096

#define	MIDI_BUFFER_SIZE			1024

#endif	// !defined(Midi_Definition_File_Included)