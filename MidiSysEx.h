// MidiSysEx.h: interface for the CMidiSysEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDISYSEX_H__C056CA81_01B8_11D3_8148_444553540000__INCLUDED_)
#define AFX_MIDISYSEX_H__C056CA81_01B8_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiClass.h"

class CMidiSysEx : public CMidiClass  
{
		DECLARE_DYNAMIC(CMidiSysEx)

	public:
		bool Empty();
		unsigned Get ( char *pSysEx );
		void Set ( const char *pSysExText );
		void Delete();
		void Init();
		CMidiSysEx();
		void Set ( unsigned iLength, const char *pSysEx );
		virtual ~CMidiSysEx();

	protected:
		char		* m_pSysEx;
		unsigned	m_iLength;
};

#endif // !defined(AFX_MIDISYSEX_H__C056CA81_01B8_11D3_8148_444553540000__INCLUDED_)
