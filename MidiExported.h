#if !defined(AFX_MIDIEXPORTED_H__FB9271E1_DEF8_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDIEXPORTED_H__FB9271E1_DEF8_11D2_8147_444553540000__INCLUDED_

extern "C" BOOL PASCAL EXPORT MidiPlay ( HWND hWnd, char *pBuffer, int iLen, int iMidiDevice  );
extern "C" BOOL PASCAL EXPORT MidiPlayAndLoop ( HWND hWnd, char *pBuffer, int iLen, int iMidiDevice  );
extern "C" BOOL PASCAL EXPORT MidiPlayFile ( HWND hWnd, const char *pFile, int iMidiDevice  );
extern "C" BOOL PASCAL EXPORT MidiPlayFileAndLoop ( HWND hWnd, const char *pFile, int iMidiDevice  );
extern "C" BOOL PASCAL EXPORT MidiStop ( );
extern "C" BOOL PASCAL EXPORT WaveStop ( );
extern "C" BOOL PASCAL EXPORT WavePlayFile ( HWND hWnd, const char *pFile );
extern "C" bool PASCAL EXPORT MidiOrWaveObject ( );
extern "C" bool PASCAL EXPORT MidiObject ( );
extern "C" bool PASCAL EXPORT WaveObject ( );
extern "C" unsigned long PASCAL EXPORT MidiGetTime ( );
extern "C" void PASCAL EXPORT MidiSlide ( unsigned long iSeconds );
extern "C" bool PASCAL EXPORT MidiIsPlaying ( );
extern "C" bool PASCAL EXPORT MidiIsRecording ( );
extern "C" bool PASCAL EXPORT MidiIsPaused ( );
extern "C" bool PASCAL EXPORT MidiIsStopped ( );
extern "C" void PASCAL EXPORT MidiPause ( );
extern "C" void PASCAL EXPORT MidiResume ( );
extern "C" void PASCAL EXPORT MidiStartRecording ( HWND hWnd, int iMidiDevice );
extern "C" void PASCAL EXPORT MidiStopRecording ( );
extern "C" void PASCAL EXPORT MidiSaveRecording ( const char *pFile );
extern "C" unsigned int  PASCAL EXPORT MidiGetMaximumHMS ( );
extern "C" char * PASCAL EXPORT MidiGetChannelProgram ( int iChannel );
extern "C" void PASCAL EXPORT MidiGetCurrentNoteAndVolume (
					int iChannel,
					unsigned char *cNote,
					unsigned char *cVolume );
extern "C" void PASCAL EXPORT MidiSetSpeed ( unsigned int iSpeed );
#include <mmsystem.h>

#endif
