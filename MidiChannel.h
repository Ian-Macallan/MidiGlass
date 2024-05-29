//
///////////////////////////////////////////////////////////////////////////////////////////
// MidiChannel.h: interface for the CMidiChannel class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDICHANNEL_H__FB9271E1_DEF8_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDICHANNEL_H__FB9271E1_DEF8_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiClass.h"

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
class CMidiChannel : public CMidiClass  
{
	public:
		void ResetVolume();
		void GetCurrentNoteAndVolume(unsigned char *cNote, unsigned char *cVolume) const;
		void SetCurrentNoteAndVolume ( DWORD dwChunk );
		void Init();
		void SetVolume ( unsigned char cVolume );
		bool IsVolumeSet() const;
		bool GetVolume ( unsigned char *cVolume ) const;
		unsigned char GetProgram() const;
		bool IsProgramSet() const;
		void SetProgram ( unsigned char iProgram );
		bool GetMuteState() const;
		void SetMuteState ( bool bMute );
		int GetChannel () const;
		void SetChannel ( int iChannel );
		const char *GetSF2File();
		void SetSF2File( const char *sf2file);
		CMidiChannel();
		virtual ~CMidiChannel();

	protected:
		unsigned char	m_cCurrentVolume;
		unsigned char	m_cCurrentNote;
		bool			m_bVolumeSet;
		unsigned char	m_iVolume;
		unsigned char	m_iProgram;
		bool			m_bProgramSet;
		bool			m_bMuted;
		int				m_iChannel;
		CString			m_SF2_file;
};

#endif // !defined(AFX_MIDICHANNEL_H__FB9271E1_DEF8_11D2_8147_444553540000__INCLUDED_)
