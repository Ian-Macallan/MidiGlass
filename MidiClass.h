//
////////////////////////////////////////////////////////////////////////
// MidiClass.h: interface for the CMidiClass class.
//
////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIDICLASS_H__6855DC33_ACD3_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDICLASS_H__6855DC33_ACD3_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiDefs.h"
#include "MidiError.h"

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class CMidiClass  : public CMidiError
{
	DECLARE_DYNAMIC(CMidiClass)

	protected:

		//	The length and the buffer.
		unsigned long		m_iMidiClassLength;
		unsigned char		m_MidiClassBuffer [ 8 ];

		//	The midi value.
		unsigned long		m_iMidiClassValue;

		//	The midi coded value.
		unsigned long		m_iMidiClassCoded;

		//	Start and End Of Buffers.
		unsigned char		*m_MidiClassStartOfBuffer;
		unsigned char		*m_MidiClassEndOfBuffer;

	public:
		bool IsTypeFile ( const char *pFilename, const char *pExtension );
		void ConvertToUpperCase ( char *pString );
		void ConvertToLowerCase ( char *pString );
		bool IsMidFile ( const char *pFilename );
		bool IsKarFile ( const char *pFilename );
		bool IsRmiFile ( const char *pFilename );
		bool IsMidiFile ( const char *pFilename );
		char * GetFileExtension ( char *pFilename );
		void ConvertHexa2Ascii ( const char *pHexa, unsigned iLen,
									char *pText, size_t iText );
		unsigned ConvertAscii2Hexa ( const char *pText, char *pHexa );
		unsigned short SearchClassEventText(const char *pText ) const;
		unsigned char GetHeight ( unsigned int iNote ) const;
		bool IsDiese ( unsigned int iNote ) const;
		bool IsBemol ( unsigned int iNote ) const;
		unsigned int GetIndex ( DWORD dwChunk ) const;
		char * GetPercussionText ( unsigned int iNum ) const;
		char * GetControlerText ( unsigned int iNum ) const;
		char * GetInstrumentText ( unsigned int iNum ) const;
		int SearchControlerByText ( const char *pText ) const;
		int SearchProgramByText ( const char *pText ) const;
		unsigned int GetClassControlerNum ( unsigned int iX ) const;
		char * GetClassControlerText ( unsigned short iX ) const;
		char * GetClassEventText ( unsigned short iX ) const;
		unsigned long GetMidiClassLength(unsigned long iVal );
		unsigned short Swap_Short_Endian ( unsigned char *pBuffer );
		unsigned long Swap_Long_Endian ( unsigned char * pBuffer );
		unsigned long ConvertValue2Midi ( unsigned long iVal );
		unsigned char *ConvertValue2Text ( unsigned long iVal );
		unsigned long ConvertText2Value ( const unsigned char *pBuffer );

		char *NoteText ( unsigned int iNote ) const;
		char *NoteTextFrench ( unsigned int iNote ) const;
		int NoteLevel ( unsigned int iNote ) const;

		static const unsigned char * GetEventSmallText ( unsigned short iEvent );
		static const char *InstrumentText ( unsigned int iInstrument );
		static const char *DrumText ( unsigned int iDrum );
		static const char *ControlerText ( unsigned int iControler );
		static const char *NoteFullText ( unsigned int iNote );

		CMidiClass();
		virtual ~CMidiClass();

};

#endif // !defined(AFX_MIDICLASS_H__6855DC33_ACD3_11D2_8147_444553540000__INCLUDED_)
