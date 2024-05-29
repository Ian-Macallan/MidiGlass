//
////////////////////////////////////////////////////////////////////////
// MidiPiece.cpp: implementation of the CMidiPiece class.
//
////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiPiece.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#ifdef	DEBUG_NEW
#define new DEBUG_NEW
#endif
#endif

#include <stdio.h>

//		For the frame value
#define	FRAME_VALUE		30

IMPLEMENT_DYNAMIC(CMidiPiece, CMidiClass)

//
////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
////////////////////////////////////////////////////////////////////////
CMidiPiece::CMidiPiece ( unsigned char *cOldNN, unsigned char *cOldDD,
							unsigned char *cOldTempo)
{
	Init ( cOldNN, cOldDD, cOldTempo );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiPiece::CMidiPiece( unsigned char			*cOldNN,
						unsigned char			*cOldDD,
						unsigned char			*cOldTempo,
						unsigned				iEvent, 
						unsigned long			iDelta,
					    unsigned				iChannelPort,
					    unsigned				iLenText,
						const unsigned char		*pText,
						unsigned char			cVal1, 
						unsigned char			cVal2, 
						unsigned char			cVal3, 
						unsigned char			cVal4, 
						unsigned char			cVal5, 
						unsigned char			cVal6 )
{
	Init (cOldNN, cOldDD, cOldTempo);

	m_iMidiDeltaTime	= iDelta;

	switch ( iEvent )
	{
		
		case	MIDI_EVENT_UNKNOWN :
		{
			m_pMidiEvent = new CMidiUnknown ( );
			break;
		}
		
		case	MIDI_EVENT_SEQUENCE :
		{
            m_pMidiEvent = new CMidiSequence ( pText, iLenText );
			break;
		}
		
		case	MIDI_EVENT_TEXT :
		{
            m_pMidiEvent = new CMidiText ( pText );
			break;
		}
		
		case	MIDI_EVENT_COPYRIGHT :
		{
            m_pMidiEvent = new CMidiCopyRight ( pText );
			break;
		}
		
		case	MIDI_EVENT_TRACKNAME :
		{
            m_pMidiEvent = new CMidiTrackName ( pText );
			break;
		}
		
		case	MIDI_EVENT_INSTRNAME :
		{
            m_pMidiEvent = new CMidiInstrumentName ( pText );
			break;
		}
		
		case	MIDI_EVENT_LYRICS :
		{
            m_pMidiEvent = new CMidiLyrics ( pText );
			break;
		}
		
		case	MIDI_EVENT_MARKERS :
		{
            m_pMidiEvent = new CMidiMarkers ( pText );
			break;
		}
		
		case	MIDI_EVENT_CUEPOINT :
		{
            m_pMidiEvent = new CMidiCuePoint ( pText );
			break;
		}
		
		case	MIDI_EVENT_SEQUENCER :
		{
            m_pMidiEvent = new CMidiSequencer ( pText, iLenText );
			break;
		}
		
		case	MIDI_EVENT_SYSEX :
		{
			m_pMidiEvent = new CMidiSystemExclusive ( pText, iLenText );
			break;
		}

		case	MIDI_EVENT_STARTTRACK :
		{
            m_pMidiEvent = new CMidiStartOfTrack ( iChannelPort );
			break;
		}

		case	MIDI_EVENT_STARTTRACK20 :
		{
            m_pMidiEvent = new CMidiStartOfTrack20 ( iChannelPort );
			break;
		}
		
		case	MIDI_EVENT_ENDOFTRACK :
		{
            m_pMidiEvent = new CMidiEndOfTrack ();
			break;
		}
		
		case	MIDI_EVENT_SETTEMPO :
		{
			m_pMidiEvent = new CMidiSetTempo ( cVal1, cVal2, cVal3 );
			break;
		}
		
		case	MIDI_EVENT_SETSMPTE :
		{
			m_pMidiEvent = new CMidiSetSMPTE ( cVal1, cVal2, cVal3, cVal4, cVal5 );
			break;
		}
		
		case	MIDI_EVENT_TIMESIG :
		{
			m_pMidiEvent = new CMidiTimeSignature ( cVal1, cVal2, cVal3, cVal4 );
			break;
		}
		
		case	MIDI_EVENT_KEYSIG :
		{
			m_pMidiEvent = new CMidiKeySignature ( cVal1, cVal2 );
			break;
		}
		
		case	MIDI_EVENT_NOTEOFF :
		{
			m_pMidiEvent = new CMidiNoteOff ( iChannelPort, cVal1, cVal2 );
			break;
		}
		
		case	MIDI_EVENT_NOTEON :
		{
			m_pMidiEvent = new CMidiNoteOn ( iChannelPort, cVal1, cVal2 );
			break;
		}
		
		case	MIDI_EVENT_KEYPRESSURE :
		{
			m_pMidiEvent = new CMidiKeyPressure ( iChannelPort, cVal1, cVal2 );
			break;
		}
		
		case	MIDI_EVENT_PARAMETER :
		{
			m_pMidiEvent = new CMidiParameter ( iChannelPort, cVal1, cVal2 );
			break;
		}
		
		case	MIDI_EVENT_PROGRAM :
		{
			m_pMidiEvent = new CMidiProgram ( iChannelPort, cVal1 );
			break;
		}
		
		case	MIDI_EVENT_CHANNELPRES :
		{
			m_pMidiEvent = new CMidiChannelPressure ( iChannelPort, cVal1 );
			break;
		}
		
		case	MIDI_EVENT_WHEEL :
		{
			m_pMidiEvent = new CMidiWheel ( iChannelPort, cVal1, cVal2 );
			break;
		}
	}


}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiPiece::CMidiPiece (	unsigned char *cOldNN,
							unsigned char *cOldDD,
							unsigned char *cOldTempo,
							unsigned char *pBuffer,
							unsigned char *oldMidiCommand )
{
	unsigned char		*pCommand;

	Init (cOldNN, cOldDD, cOldTempo);

	m_MidiClassStartOfBuffer	= pBuffer;

	//		Convert Delta Time.
	m_iMidiDeltaTime	= ConvertText2Value ( pBuffer );
	pCommand = pBuffer + m_iMidiClassLength;

	//		Now Treat the following command.
    switch ( *pCommand )
    {
        //      Meta events.
        case MIDI_EVENT_CODE_META :
        {
            switch ( * ( pCommand + 1 ) )
            {
                //      Sequence Number. Normally only two bytes
                case MIDI_EVENT_CODE_META_SEQ_1 :
                {
                    m_pMidiEvent = new CMidiSequence ( *pCommand, pCommand + 1 );
                    break;
                }

                //      Text.
                case MIDI_EVENT_CODE_META_TEXT :
                {
                    m_pMidiEvent = new CMidiText ( *pCommand, pCommand + 1 );
                    break;
                }

                //      Copyright.
                case MIDI_EVENT_CODE_META_COPY :
                {
                    m_pMidiEvent = new CMidiCopyRight ( *pCommand, pCommand + 1 );
                    break;
                }
    
                //      Track Name.
                case MIDI_EVENT_CODE_META_TRKNAME :
                {
                    m_pMidiEvent = new CMidiTrackName ( *pCommand, pCommand + 1 );
                    break;
                }
            
                //      Instrument Name.
                case MIDI_EVENT_CODE_META_INSNAME :
                {
                    m_pMidiEvent = new CMidiInstrumentName ( *pCommand, pCommand + 1 );
                    break;
                }

                //      Lyrics.
                case MIDI_EVENT_CODE_META_LYRICS :
                {
                    m_pMidiEvent = new CMidiLyrics ( *pCommand, pCommand + 1 );
                    break;
                }

                //      Midi Marker.
                case MIDI_EVENT_CODE_META_MARKER :
                {
                    m_pMidiEvent = new CMidiMarkers ( *pCommand, pCommand + 1 );
                    break;
                }

                //      Cue Point.
                case MIDI_EVENT_CODE_META_CUEPOINT :
                {
                    m_pMidiEvent = new CMidiCuePoint ( *pCommand, pCommand + 1 );
                    break;
                }
            
                //      A kind of Start of Track
                case MIDI_EVENT_CODE_META_ST20_1 :
                {
                    m_pMidiEvent = new CMidiStartOfTrack20 ( *pCommand, pCommand + 1 );
                    break;
                }
        
                //      Start of track.
                case MIDI_EVENT_CODE_META_START_1 :
                {
                    m_pMidiEvent = new CMidiStartOfTrack ( *pCommand, pCommand + 1 );
                    break;
                }
        
                //      End Of Track.
                case MIDI_EVENT_CODE_META_EOT_1 :
                {
                    m_pMidiEvent = new CMidiEndOfTrack ( *pCommand, pCommand + 1 );
                    break;
                }

                //      Set Tempo.
                case MIDI_EVENT_CODE_META_TEMPO_1 :
                {
					CMidiSetTempo		*pTempo;
                    m_pMidiEvent = new CMidiSetTempo ( *pCommand, pCommand + 1 );
					pTempo = ( CMidiSetTempo * ) m_pMidiEvent;

					//		Set new default Tempo
					memcpy ( cOldTempo, pTempo->GetTempo (), sizeof ( m_cMidiPieceTempo ) ); 
					//		Set Tempo in This event.
					memcpy ( m_cMidiPieceTempo, cOldTempo, sizeof (m_cMidiPieceTempo ) );
                    break;
                }

                //      Set SMPTE.
                case MIDI_EVENT_CODE_META_SMPTE_1 :
                {
                    m_pMidiEvent = new CMidiSetSMPTE ( *pCommand, pCommand + 1 );
                    break;
                }

                //		Time Signature.
                case MIDI_EVENT_CODE_META_TIMSIG_1 :
                {
					CMidiTimeSignature	*pSig;
                    m_pMidiEvent = new CMidiTimeSignature ( *pCommand, pCommand + 1 );
					pSig = ( CMidiTimeSignature * ) m_pMidiEvent;

					//		Set new default Time Signature
					*cOldNN		= pSig->GetNN ();
					*cOldDD		= pSig->GetDD ();

					//		Set Time Signature in This event.
					m_cMidiPieceNN  = *cOldNN;
					m_cMidiPieceDD  = *cOldDD;

                    break;
                }

                //		Key Signature.
                case MIDI_EVENT_CODE_META_KEYSIG_1 :
                {
                    m_pMidiEvent = new CMidiKeySignature ( *pCommand, pCommand + 1 );
                    break;
                }
    
                //     Sequencer.
                case MIDI_EVENT_CODE_META_SEQR :
                {
                    m_pMidiEvent = new CMidiSequencer ( *pCommand, pCommand + 1 );
                    break;
                }

				//		Unknown Midi Event
                default :
                {
                    m_pMidiEvent = new CMidiUnknown ( *pCommand, pCommand + 1 );
                    break;
                }

            } //	Switch

            break;
        }

         //     System Exclusive.
        case MIDI_EVENT_CODE_SYSEX :
        {
            m_pMidiEvent = new CMidiSystemExclusive ( *pCommand, pCommand + 1 );
            break;
        }

		//		All others
        default :
        {
            switch ( *pCommand & OPERATION_MASK )
			{
				//	Note off
				case MIDI_EVENT_CODE_NOTEOFF :
				{
		            m_pMidiEvent = new CMidiNoteOff ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				//	Note on
				case MIDI_EVENT_CODE_NOTEON :
				{
		            m_pMidiEvent = new CMidiNoteOn ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				//	Key Pressure
				case MIDI_EVENT_CODE_KEYPRES :
				{
		            m_pMidiEvent = new CMidiKeyPressure ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				//	Parameter
				case MIDI_EVENT_CODE_PARAM :
				{
		            m_pMidiEvent = new CMidiParameter ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				//	Program
				case MIDI_EVENT_CODE_PROGRAM :
				{
		            m_pMidiEvent = new CMidiProgram ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				//	Channel Pressure
				case MIDI_EVENT_CODE_CHANPRESS :
				{
		            m_pMidiEvent = new CMidiChannelPressure ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				//	Wheel
				case MIDI_EVENT_CODE_WHEEL :
				{
		            m_pMidiEvent = new CMidiWheel ( *pCommand, pCommand + 1 );

					//		Set the Old Command
					*oldMidiCommand			= m_pMidiEvent->GetCodeAndChannel ();

					break;
				}

				default:
				{
					//		Try the case of Command Omission
					switch ( *oldMidiCommand & OPERATION_MASK )
					{
						//	Not off
						case MIDI_EVENT_CODE_NOTEOFF :
						{
							m_pMidiEvent = new CMidiNoteOff ( *oldMidiCommand, pCommand );
							break;
						}

						//	Not on
						case MIDI_EVENT_CODE_NOTEON :
						{
							m_pMidiEvent = new CMidiNoteOn ( *oldMidiCommand, pCommand );
							break;
						}

						case MIDI_EVENT_CODE_KEYPRES :
						{
							m_pMidiEvent = new CMidiKeyPressure ( *oldMidiCommand, pCommand );
							break;
						}

						case MIDI_EVENT_CODE_PARAM :
						{
							m_pMidiEvent = new CMidiParameter ( *oldMidiCommand, pCommand );
							break;
						}

						case MIDI_EVENT_CODE_PROGRAM :
						{
							m_pMidiEvent = new CMidiProgram ( *oldMidiCommand, pCommand );
							break;
						}

						case MIDI_EVENT_CODE_CHANPRESS :
						{
							m_pMidiEvent = new CMidiChannelPressure ( *oldMidiCommand, pCommand );
							break;
						}

						case MIDI_EVENT_CODE_WHEEL :
						{
							m_pMidiEvent = new CMidiWheel ( *oldMidiCommand, pCommand );
							break;
						}

						//		Not Valid.
						default:
						{
							m_pMidiEvent = new CMidiUnknown ( *oldMidiCommand, pCommand );
						}

					} // Switch *oldMidiCommand

					break;

				} // Default

			} // switch *pCommand

            break;

        } // All Others

    } // First Switch

	//		Set the next address to search.
	m_MidiClassEndOfBuffer	= m_pMidiEvent->GetEndOfBufferAddress ();
}

//
////////////////////////////////////////////////////////////////////////
//	Copy COnstructor
////////////////////////////////////////////////////////////////////////
CMidiPiece::CMidiPiece ( const CMidiPiece &midiPiece )
{
	Dupplicate ( &midiPiece );
}

//
////////////////////////////////////////////////////////////////////////
//	Copy COnstructor
////////////////////////////////////////////////////////////////////////
CMidiPiece::CMidiPiece ( const CMidiPiece *midiPiece )
{
	Dupplicate ( midiPiece );
}

//
////////////////////////////////////////////////////////////////////////
//	Copy COnstructor
////////////////////////////////////////////////////////////////////////
void CMidiPiece::Dupplicate ( const CMidiPiece *midiPiece )
{
	m_bNoteOffUsed			= midiPiece->m_bNoteOffUsed;
	m_lNoteDuration			= midiPiece->m_lNoteDuration;
	m_iMidiPieceState		= midiPiece->m_iMidiPieceState;
	m_lAbsoluteHMSm			= midiPiece->m_lAbsoluteHMSm;
	m_iMidiHours			= midiPiece->m_iMidiHours;
	m_iMidiMinutes			= midiPiece->m_iMidiMinutes;
	m_iMidiSeconds			= midiPiece->m_iMidiSeconds;
	m_iMidiMillisecs		= midiPiece->m_iMidiMillisecs;
	m_lTempo				= midiPiece->m_lTempo;
	m_iMidiAbsoluteTime		= midiPiece->m_iMidiAbsoluteTime;
	m_iMidiDeltaTime		= midiPiece->m_iMidiDeltaTime;
	m_cMidiPieceNN			= midiPiece->m_cMidiPieceNN;
	m_cMidiPieceDD			= midiPiece->m_cMidiPieceDD;
	m_iMidiMesure			= midiPiece->m_iMidiMesure;
	m_iMidiBeat				= midiPiece->m_iMidiBeat;
	m_iMidiTimeFrame		= midiPiece->m_iMidiTimeFrame;
	m_iMidiTimeDivision		= midiPiece->m_iMidiTimeDivision;

	memcpy ( m_cMidiPieceTempo, midiPiece->m_cMidiPieceTempo, sizeof(m_cMidiPieceTempo) );

	int iEvent				= midiPiece->m_pMidiEvent->GetEventIndex();
	switch ( iEvent )
	{
		case	MIDI_EVENT_UNKNOWN :
		{
			m_pMidiEvent = NewMidiEvent ( iEvent );
			break;
		}
		
		case	MIDI_EVENT_SEQUENCE :
		{
			CMidiSequence *pEvent = dynamic_cast<CMidiSequence *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, pEvent->GetBinaryBuffer(), pEvent->GetBinaryBufferLength() );
			break;
		}
		
		case	MIDI_EVENT_TEXT :
		{
			CMidiText *pEvent = dynamic_cast<CMidiText *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, pEvent->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_COPYRIGHT :
		{
			CMidiCopyRight *pEvent = dynamic_cast<CMidiCopyRight *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent,  ( ( CMidiText *) midiPiece->m_pMidiEvent )->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_TRACKNAME :
		{
			CMidiTrackName *pEvent = dynamic_cast<CMidiTrackName *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, ( ( CMidiText *) midiPiece->m_pMidiEvent )->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_INSTRNAME :
		{
			CMidiInstrumentName *pEvent = dynamic_cast<CMidiInstrumentName *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, ( ( CMidiText *) midiPiece->m_pMidiEvent )->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_LYRICS :
		{
			CMidiLyrics *pEvent = dynamic_cast<CMidiLyrics *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, ( ( CMidiText *) midiPiece->m_pMidiEvent )->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_MARKERS :
		{
			CMidiMarkers *pEvent = dynamic_cast<CMidiMarkers *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, ( ( CMidiText *) midiPiece->m_pMidiEvent )->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_CUEPOINT :
		{
			CMidiCuePoint *pEvent = dynamic_cast<CMidiCuePoint *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, ( ( CMidiText *) midiPiece->m_pMidiEvent )->GetTextAddress( MASK_ALL ) );
			break;
		}
		
		case	MIDI_EVENT_SEQUENCER :
		{
			CMidiSequencer *pEvent = dynamic_cast<CMidiSequencer *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, pEvent->GetBinaryBuffer(), pEvent->GetBinaryBufferLength() );
			break;
		}
		
		case	MIDI_EVENT_SYSEX :
		{
			CMidiSystemExclusive *pEvent = dynamic_cast<CMidiSystemExclusive *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, pEvent->GetSysexBuffer(), pEvent->GetSysexBufferLength() );
			break;
		}

		case	MIDI_EVENT_STARTTRACK :
		{
			CMidiStartOfTrack *pEvent = dynamic_cast<CMidiStartOfTrack *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetEventValue1() );
			break;
		}

		case	MIDI_EVENT_STARTTRACK20 :
		{
			CMidiStartOfTrack20 *pEvent = dynamic_cast<CMidiStartOfTrack20 *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetEventValue1() );
			break;
		}
		
		case	MIDI_EVENT_ENDOFTRACK :
		{
			CMidiEndOfTrack *pEvent = dynamic_cast<CMidiEndOfTrack *>(midiPiece->m_pMidiEvent);
            m_pMidiEvent =NewMidiEvent ( iEvent );
			break;
		}
		
		case	MIDI_EVENT_SETTEMPO :
		{
			CMidiSetTempo *pEvent = dynamic_cast<CMidiSetTempo *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetEventValue1(), 
				midiPiece->m_pMidiEvent->GetEventValue2(),  midiPiece->m_pMidiEvent->GetEventValue3() );
			break;
		}
		
		case	MIDI_EVENT_SETSMPTE :
		{
			CMidiSetSMPTE *pEvent = dynamic_cast<CMidiSetSMPTE *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2(),  midiPiece->m_pMidiEvent->GetEventValue3(),
				midiPiece->m_pMidiEvent->GetEventValue4(), midiPiece->m_pMidiEvent->GetEventValue5() );
			break;
		}
		
		case	MIDI_EVENT_TIMESIG :
		{
			CMidiTimeSignature *pEvent = dynamic_cast<CMidiTimeSignature *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2(),  midiPiece->m_pMidiEvent->GetEventValue3(),
				midiPiece->m_pMidiEvent->GetEventValue4() );
			break;
		}
		
		case	MIDI_EVENT_KEYSIG :
		{
			CMidiKeySignature *pEvent = dynamic_cast<CMidiKeySignature *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetEventValue1(), midiPiece->m_pMidiEvent->GetEventValue2() );
			break;
		}
		
		case	MIDI_EVENT_NOTEOFF :
		{
			CMidiNoteOff *pEvent = dynamic_cast<CMidiNoteOff *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2() );
			break;
		}
		
		case	MIDI_EVENT_NOTEON :
		{
			CMidiNoteOn *pEvent = dynamic_cast<CMidiNoteOn *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2() );
			break;
		}
		
		case	MIDI_EVENT_KEYPRESSURE :
		{
			CMidiKeyPressure *pEvent = dynamic_cast<CMidiKeyPressure *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2() );
			break;
		}
		
		case	MIDI_EVENT_PARAMETER :
		{
			CMidiParameter *pEvent = dynamic_cast<CMidiParameter *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2() );
			break;
		}
		
		case	MIDI_EVENT_PROGRAM :
		{
			CMidiProgram *pEvent = dynamic_cast<CMidiProgram *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1() );
			break;
		}
		
		case	MIDI_EVENT_CHANNELPRES :
		{
			CMidiChannelPressure *pEvent = dynamic_cast<CMidiChannelPressure *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1() );
			break;
		}
		
		case	MIDI_EVENT_WHEEL :
		{
			CMidiWheel *pEvent = dynamic_cast<CMidiWheel *>(midiPiece->m_pMidiEvent);
			m_pMidiEvent = NewMidiEvent ( iEvent, midiPiece->m_pMidiEvent->GetChannel(), midiPiece->m_pMidiEvent->GetEventValue1(),
				midiPiece->m_pMidiEvent->GetEventValue2() );
			break;
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//	Protected COnstructor
////////////////////////////////////////////////////////////////////////
CMidiPiece::CMidiPiece (  )
{
	m_lNoteDuration				= 0;

	m_iMidiDeltaTime			= 0;
	m_iMidiAbsoluteTime			= 0;
	m_lAbsoluteHMSm				= 0;

	m_cMidiPieceNN				= 4;
	m_cMidiPieceDD				= 2;
	m_iMidiPieceState			= 0;

	m_iMidiHours				= 0;
	m_iMidiMinutes				= 0;
	m_iMidiSeconds				= 0;
	m_iMidiMillisecs			= 0;

	m_cMidiPieceTempo [ 0 ]		= 9;
	m_cMidiPieceTempo [ 1 ]		= 39;
	m_cMidiPieceTempo [ 2 ]		= 192;

	m_lTempo					= m_cMidiPieceTempo [ 2 ] + 256 * ( m_cMidiPieceTempo [ 1 ] + ( 256 * m_cMidiPieceTempo [ 0 ] ) );
	m_iMidiMesure				= 0;
	m_iMidiBeat					= 0;
	m_iMidiTimeFrame			= 0;
	m_iMidiTimeDivision			= 0;

	m_pMidiEvent				= NewMidiEvent ( 0 );

	m_bNoteOffUsed				= false;

}
//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
CMidiPiece * CMidiPiece::newEmptyMidiPiece()
{
	CMidiPiece *pMidiPiece = new CMidiPiece();
	return pMidiPiece;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_UNKNOWN :
		{
			return  new CMidiUnknown ();
		}
		case	MIDI_EVENT_ENDOFTRACK :
		{
            return new CMidiEndOfTrack ();
		}
	}

	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, const unsigned char *pText )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_TEXT :
		{
            return new CMidiText ( pText );
		}
		
		case	MIDI_EVENT_COPYRIGHT :
		{
            return new CMidiCopyRight ( pText );
		}
		
		case	MIDI_EVENT_TRACKNAME :
		{
            return  new CMidiTrackName ( pText );
		}
		
		case	MIDI_EVENT_INSTRNAME :
		{
            return new CMidiInstrumentName ( pText );
		}
		
		case	MIDI_EVENT_LYRICS :
		{
            return new CMidiLyrics ( pText );
		}
		
		case	MIDI_EVENT_MARKERS :
		{
           return new CMidiMarkers ( pText );
		}
		
		case	MIDI_EVENT_CUEPOINT :
		{
            return new CMidiCuePoint ( pText );
		}
	}

	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, unsigned char cByte )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_STARTTRACK :
		{
           return new CMidiStartOfTrack ( cByte );
		}

		case	MIDI_EVENT_STARTTRACK20 :
		{
            return new CMidiStartOfTrack20 ( cByte );
		}
	}

	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2  )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_KEYSIG :
		{
			return new CMidiKeySignature ( cByte1, cByte2 );
		}
		
		case	MIDI_EVENT_PROGRAM :
		{
			return new CMidiProgram ( cByte1, cByte2 );
		}
		
		case	MIDI_EVENT_CHANNELPRES :
		{
			return new CMidiChannelPressure ( cByte1, cByte2 );
		}
	}

	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2, unsigned char cByte3  )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_SETTEMPO :
		{
			return new CMidiSetTempo ( cByte1, cByte2, cByte3 );
		}

		case	MIDI_EVENT_NOTEOFF :
		{
			return new CMidiNoteOff ( cByte1, cByte2, cByte3 );
		}

		case	MIDI_EVENT_NOTEON :
		{
			return new CMidiNoteOn  ( cByte1, cByte2, cByte3 );
		}

		case	MIDI_EVENT_KEYPRESSURE :
		{
			return new CMidiKeyPressure ( cByte1, cByte2, cByte3 );
		}
		
		case	MIDI_EVENT_PARAMETER :
		{
			return new CMidiParameter ( cByte1, cByte2, cByte3 );
		}

		case	MIDI_EVENT_WHEEL :
		{
			return new CMidiWheel ( cByte1, cByte2, cByte3 );
		}
	}

	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2, unsigned char cByte3, unsigned char cByte4 )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_TIMESIG :
		{
			return new CMidiTimeSignature ( cByte1, cByte2, cByte3, cByte4 );
		}
	}

	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, unsigned char cByte1, unsigned char cByte2, unsigned char cByte3, 
										unsigned char cByte4, unsigned char cByte5 )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_SETSMPTE :
		{
			return new CMidiSetSMPTE ( cByte1, cByte2, cByte3, cByte4, cByte5 );
		}
	}
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent * CMidiPiece::NewMidiEvent ( int iEvent, const unsigned char *pText, int len )
{
	switch ( iEvent )
	{
		case	MIDI_EVENT_SEQUENCE :
		{
            return new CMidiSequence ( pText, len );
		}
		case	MIDI_EVENT_SEQUENCER :
		{
            return new CMidiSequencer ( pText, len );
		}
		case	MIDI_EVENT_SYSEX :
		{
			return new CMidiSystemExclusive (pText, len );
		}
		
	}
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::NewEmptyEvent ( int iEvent )
{
	if ( m_pMidiEvent != NULL )
	{
		delete m_pMidiEvent;
		m_pMidiEvent = NULL;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent, (unsigned char *) "" );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	unsigned char szBinary [ 3 ] = { 0x00, 0xf7, 0x00 }; 
	m_pMidiEvent = NewMidiEvent ( iEvent, (unsigned char *) szBinary, 2 );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent, (unsigned char) 0 );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent, 0, 0 );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent, 0, 0, 0 );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent, 0, 0, 0, 0 );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

	m_pMidiEvent = NewMidiEvent ( iEvent, 0, 0, 0, 0, 0 );
	if ( m_pMidiEvent != NULL )
	{
		return;
	}

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiPiece::~CMidiPiece()
{
	Free ( );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::Trace(FILE *hFile)
{
	if ( hFile !=NULL )
	{
		fprintf ( hFile, 
			"%02d:%02d:%02d.%03d - %04d:%02d:%03d - %6d (%5d) - (%2d/%2d) - ",
			m_iMidiHours , m_iMidiMinutes, m_iMidiSeconds, m_iMidiMillisecs,
			m_iMidiMesure + 1 , m_iMidiBeat + 1, m_iMidiTimeFrame, 
			m_iMidiAbsoluteTime, m_iMidiDeltaTime,
			m_cMidiPieceNN, Power ( m_cMidiPieceDD ) );
		if ( m_pMidiEvent != NULL )
		{
			m_pMidiEvent->Trace ( hFile );
		}
		else
		{
			fprintf ( hFile, "NO EVENT\n" );
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::Init( unsigned char *cOldNN, unsigned char *cOldDD, unsigned char *cOldTempo )
{
	m_lNoteDuration			= 0;

	m_iMidiDeltaTime		= 0;
	m_iMidiAbsoluteTime		= 0;
	m_lAbsoluteHMSm			= 0;

	m_cMidiPieceNN			= *cOldNN;
	m_cMidiPieceDD			= *cOldDD;
	m_iMidiPieceState		= 0;

	m_iMidiHours			= 0;
	m_iMidiMinutes			= 0;
	m_iMidiSeconds			= 0;
	m_iMidiMillisecs		= 0;

	m_lTempo				= 0;

	memcpy ( m_cMidiPieceTempo, cOldTempo, sizeof ( m_cMidiPieceTempo ) );

	m_iMidiMesure			= 0;
	m_iMidiBeat				= 0;
	m_iMidiTimeFrame		= 0;
	m_iMidiTimeDivision		= 0;

	m_pMidiEvent			= NULL;

	m_bNoteOffUsed			= false;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::Free()
{
	if ( m_pMidiEvent != NULL )
	{
		m_pMidiEvent->Free ();
		delete m_pMidiEvent;
		m_pMidiEvent = NULL;
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned long CMidiPiece::ComputeAbsoluteTime(unsigned long iTime)
{
	m_iMidiAbsoluteTime	= m_iMidiDeltaTime + iTime;

	return ( m_iMidiAbsoluteTime );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned long CMidiPiece::GetAbsoluteTime() const
{
	return ( m_iMidiAbsoluteTime );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetNN() const
{
	return ( m_cMidiPieceNN ); 
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetDD() const
{
	return ( m_cMidiPieceDD ); 
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetNN(unsigned char cNN)
{
	m_cMidiPieceNN = cNN;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetDD(unsigned char cDD)
{
	m_cMidiPieceDD = cDD;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetMesure() const
{
	return ( m_iMidiMesure );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetBeat() const
{
	return ( m_iMidiBeat );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetTimeFrame() const
{
	return ( m_iMidiTimeFrame );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::ComputeMesure (
		unsigned char	cHi,
		unsigned char	cLo,
		unsigned char	cNN,
		unsigned char	cDD,
		unsigned short	iMesure,
		unsigned short	iBeat,
		unsigned short	iTimeFrame,
		long			lDelta )
{
    unsigned long   iRound;
    unsigned short  iDivisor;
    long			iTime;

	//		Compute with the given time signature.
	iTime = lDelta;

	//		Compute Divisor that is the length of a black
	iRound		= cDD;
	iDivisor	= ( cHi + cLo * 256 ) * 4;
	while ( iRound )
	{
		iDivisor /= 2;
		iRound--;
	}

	if ( iDivisor <= 0 )
	{
		iDivisor = 1;
	}

	m_iMidiTimeFrame = iTimeFrame + ( unsigned short )( iTime % iDivisor );
	iTime = iTime / iDivisor;

	m_iMidiBeat = iBeat + ( unsigned short ) ( iTime % cNN );

	m_iMidiMesure = iMesure + ( unsigned short ) ( iTime / cNN );

	//		Correct Mesure
	while ( m_iMidiTimeFrame >= iDivisor )
	{
		m_iMidiTimeFrame -= iDivisor;
		m_iMidiBeat += 1;
	}

	while ( m_iMidiBeat >= cNN )
	{
		m_iMidiBeat -= cNN;
		m_iMidiMesure += 1;
	}

	//	Seems to be OK
	m_iMidiTimeDivision = m_iMidiTimeFrame  * 120 / ( cHi + cLo * 256 );

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::Power(unsigned char cVal)
{
	unsigned char		cTemp;
	cTemp = 1;
	while ( cVal > 0 )
	{
		cTemp =  2 * cTemp;
		cVal--;
	}
	
	return cTemp;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::Save(FILE *hFile)
{
	if ( m_pMidiEvent != NULL )
	{
		//		Write the delta time
		fwrite ( ConvertValue2Text ( m_iMidiDeltaTime ), 1, GetMidiClassLength ( m_iMidiDeltaTime ), hFile );

		//		Now write the event it self.
		m_pMidiEvent->Save ( hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char * CMidiPiece::GetTextAddress( unsigned short cSubCode ) const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetTextAddress( cSubCode );
	}
	else
	{
		return NULL;
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char * CMidiPiece::GetEventText( ) const
{
	if ( m_pMidiEvent != NULL )
	{
		return ( m_pMidiEvent->GetEventText( ) );
	}
	else
	{
		return ( NULL );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetCode() const
{
	if ( m_pMidiEvent != NULL )
	{
		return ( m_pMidiEvent->GetCode() );
	}
	else
	{
		return ( 0x00 );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetSubCode() const
{
	if ( m_pMidiEvent != NULL )
	{
		return ( m_pMidiEvent->GetSubCode() );
	}
	else
	{
		return ( 0xff );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char * CMidiPiece::GetMesureText() const
{
	static		unsigned char szString [ MAX_MESURE_STRING ];

	sprintf_s ( ( char * ) szString, sizeof ( szString ), "%04d:%02d:%03d", 
			m_iMidiMesure + 1 , m_iMidiBeat + 1, m_iMidiTimeFrame );

	return ( szString );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetChannel() const
{
	if ( m_pMidiEvent != NULL )
	{
		return ( m_pMidiEvent->GetChannel() );
	}
	else
	{
		return ( 0xff );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char * CMidiPiece::GetValues() const
{
	static		unsigned char szString [ 6 ];
	
	memset ( szString, 0, sizeof ( szString ) );
	if ( m_pMidiEvent != NULL )
	{
		szString [ 0 ] = m_pMidiEvent->GetEventValue1 ();
		szString [ 1 ] = m_pMidiEvent->GetEventValue2 ();
		szString [ 2 ] = m_pMidiEvent->GetEventValue3 ();
		szString [ 3 ] = m_pMidiEvent->GetEventValue4 ();
		szString [ 4 ] = m_pMidiEvent->GetEventValue5 ();
		szString [ 5 ] = m_pMidiEvent->GetEventValue6 ();
	}

	return ( szString );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetExtraText() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetExtraText();
	}
	else
	{
		return ( NULL );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned long CMidiPiece::GetDeltaTime() const
{
	return m_iMidiDeltaTime;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::AddDeltaTime(unsigned long iTime)
{
	m_iMidiDeltaTime += iTime;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetDeltaTime(unsigned long iTime)
{
	m_iMidiDeltaTime = iTime;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char * CMidiPiece::GetTempo() const
{
	return m_cMidiPieceTempo;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetTempo(const unsigned char cTempo[])
{
	memcpy ( m_cMidiPieceTempo, cTempo, sizeof ( m_cMidiPieceTempo ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::ComputeTempo ( 
		unsigned char	cHi,
		unsigned char	cLo,
		unsigned char	cTempo [ 3 ],
		unsigned short	cHours, 
		unsigned short	cMinutes,
		unsigned short	cSeconds,
		unsigned short	cMillisecs,
		long			lDelta )
{
	//		Compute the value in micro seconds of a quarter
	//		The quarter value is 120
	m_lTempo	=	cTempo [ 2 ] + 256 * ( cTempo [ 1 ] + ( 256 * cTempo [ 0 ] ) );

	//		Compute the new value for the Time
	unsigned long lMicro		= ( m_lTempo / ( cHi + 256 * cLo ) ) * lDelta;

	unsigned long lTemp			= lMicro / 1000;

	//
	unsigned long lMillisecs	= lTemp % 1000;
	lTemp = ( lTemp - lMillisecs ) / 1000;

	unsigned long lSeconds		= lTemp % 60;
	lTemp = ( lTemp - lSeconds ) / 60;

	unsigned long lMinutes		= lTemp % 60;
	lTemp = ( lTemp - lMinutes ) / 60;

	unsigned long lHours		= lTemp;

#ifdef _DEBUG
	char szBuffer [ 128 ];
	sprintf_s ( szBuffer, sizeof(szBuffer), "%ld %ld %u %u: %ld:%ld:%ld.%ld - ", m_lTempo, lDelta, cHi, cLo, lHours, lMinutes, lSeconds, lMillisecs );
	OutputDebugString ( szBuffer );
#endif

	//
	m_iMidiHours		= cHours		+ ( unsigned short ) lHours;
	m_iMidiMinutes		= cMinutes		+ ( unsigned short ) lMinutes;
	m_iMidiSeconds		= cSeconds		+ ( unsigned short ) lSeconds;
	m_iMidiMillisecs	= cMillisecs	+ ( unsigned short ) lMillisecs;

	m_lAbsoluteHMSm = m_iMidiMillisecs + 1000 * ( m_iMidiSeconds + 60 * ( m_iMidiMinutes + 60 * m_iMidiHours ) );

    lTemp               = m_lAbsoluteHMSm;
    m_iMidiMillisecs    = lTemp % 1000L;
    lTemp               /= 1000L;
    m_iMidiSeconds      = lTemp % 60L;
    lTemp               /= 60;
    m_iMidiMinutes      = lTemp % 60L;
    lTemp               /= 60L;
    m_iMidiHours        = (unsigned short ) lTemp;

#ifdef _DEBUG
	sprintf_s ( szBuffer, sizeof(szBuffer), "%02ld:%02ld:%02ld.%03ld\n", m_iMidiHours, m_iMidiMinutes, m_iMidiSeconds, m_iMidiMillisecs );
	OutputDebugString ( szBuffer );
#endif

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char * CMidiPiece::GetHMSText() const
{
	static		unsigned char szString [ MAX_HOUR_STRING ];


	sprintf_s ( ( char * ) szString, sizeof ( szString ), "%02d:%02d:%02d.%03d", 
			m_iMidiHours , m_iMidiMinutes, 
			m_iMidiSeconds, m_iMidiMillisecs );
/*
	sprintf ( ( char * ) szString, "%02d:%02d:%02d.%03d", 
			m_iMidiHours , m_iMidiMinutes, 
			m_iMidiSeconds, m_iMidiTimeDivision );
 */
 	return ( szString );

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetHours() const
{
	return m_iMidiHours;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetMinutes() const
{
	return m_iMidiMinutes;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetSeconds() const
{
	return m_iMidiSeconds;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetMillisecs() const
{
	return m_iMidiMillisecs;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetMesure(unsigned short lMesure)
{
	m_iMidiMesure = lMesure;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetBeat(unsigned short lMesure)
{
	m_iMidiBeat = lMesure;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetTimeFrame(unsigned short lMesure)
{
	m_iMidiTimeFrame = lMesure;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned long CMidiPiece::GetAbsoluteHMSm() const
{
	return m_lAbsoluteHMSm;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiPiece::GetChunk() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetChunk ();
	}

	return ( NULL );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
MIDIHDR * CMidiPiece::GetSysexChunk()
{
	if ( m_pMidiEvent != NULL )
	{
		return ( m_pMidiEvent->GetSysexChunk () );
	}

	return ( NULL );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetState(int iState)
{
	m_iMidiPieceState = iState;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
int CMidiPiece::GetState() const
{
	return m_iMidiPieceState;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetValue1() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventValue1 ();
	}

	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetValue2() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventValue2 ();
	}

	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetValue3() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventValue3 ();
	}

	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetValue4() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventValue4 ();
	}

	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetValue5() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventValue5 ();
	}

	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiPiece::GetValue6() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventValue6 ();
	}

	return 0x00;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetLabel1() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventLabel1 ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetLabel2() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventLabel2 ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetLabel3() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventLabel3 ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetLabel4() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventLabel4 ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetLabel5() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventLabel5 ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char *CMidiPiece::GetLabel6() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->GetEventLabel6 ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////
//		Note that duration is not the real duration
//		but reflect the value white / black and so on
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetNoteDuration(	unsigned char	cHi,
									unsigned char	cLo,
									unsigned long	lDuration )
{
	//		Compute the new value for the Time Duration
	//		Depending on m_cMidiPieceDD and m_cMidiPieceNN

	//	Seems to be OK
	m_lNoteDuration = lDuration  * 120 / ( cHi + cLo * 256 );

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned long CMidiPiece::GetNoteDuration() const
{
	return m_lNoteDuration;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetNoteOffUsed(bool bUsed)
{
	m_bNoteOffUsed = bUsed;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
bool CMidiPiece::GetNoteOffUsed() const
{
	return m_bNoteOffUsed;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned short CMidiPiece::GetTimeDivision() const
{
	return m_iMidiTimeDivision;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetChannelAndCode(unsigned char cChannel)
{
	if ( m_pMidiEvent != NULL )
	{
		m_pMidiEvent->SetChannelAndCode ( cChannel );
	}

	return;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiPiece::SetAbsoluteTime(unsigned long iTime)
{
	m_iMidiAbsoluteTime = iTime;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned int CMidiPiece::GetEventIndex() const
{
	if ( m_pMidiEvent != NULL )
	{
		return ( m_pMidiEvent->GetEventIndex () );
	}

	return ( 0 );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiEvent *CMidiPiece::GetMidiEvent()
{
	return m_pMidiEvent;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
bool CMidiPiece::useChannel() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->useChannel();
	}

	return false;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
bool CMidiPiece::usePort() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->usePort();
	}

	return false;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
int CMidiPiece::valueCount() const
{
	if ( m_pMidiEvent != NULL )
	{
		return m_pMidiEvent->valueCount();
	}

	return 0;
};
