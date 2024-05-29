#pragma once

#include "mmsystem.h"

#include "MidiEvent.h"

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiBinary : public CMidiEvent  
{
	DECLARE_DYNAMIC(CMidiBinary)

	protected :
		//	Text Length.
		unsigned long		m_iMidiBinaryLength;

		//	Text Value.
		unsigned char		*m_MidiBinaryBuffer;

	public:
		CMidiBinary ( unsigned char cCommand, unsigned char *pBuffer );
		CMidiBinary ( const unsigned char *pBuffer, const int iSize );
		//	Buffer is hexadecimal
		CMidiBinary( const unsigned char *pBuffer );
		virtual ~CMidiBinary();

		virtual void Trace ( FILE *hFile );

		virtual void Free();

		void  Save ( FILE *hFile);
		virtual const unsigned char *GetEventText () const;
		virtual unsigned char *GetTextAddress( unsigned short cSubCode = 0xffff ) const;
		//	Take an hexadeimal string and store it
		//	Must be on the form hx hx hx hx
		virtual void SetText ( const unsigned char *pBuffer );
		const char * GetExtraText() const;

		const unsigned char * GetBinaryBuffer() const; 
		const unsigned long GetBinaryBufferLength() const; 

		virtual const char *GetEventLabel1 () const
		{
			return "Binary";
		}
};


//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiSequence : public CMidiBinary
{
	DECLARE_DYNAMIC(CMidiSequence)

	public:
		CMidiSequence( unsigned char cCommand, unsigned char *pBuffer );
		CMidiSequence( const unsigned char *pBuffer, const int iSize );
		//	Buffer is hexadecimal
		CMidiSequence( const unsigned char *pBuffer );
		virtual ~CMidiSequence();

		//	Take an hexadeimal string and store it
		//	Must be on the form hx hx hx hx
		virtual void SetText ( const unsigned char *pBuffer );

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Sequence";
		}
};

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
class CMidiSequencer : public CMidiBinary  
{
	DECLARE_DYNAMIC(CMidiSequencer)

	public:
		CMidiSequencer( unsigned char cCommand, unsigned char *pBuffer );
		CMidiSequencer( const unsigned char *pBuffer, const int iSize );

		//	Buffer is hexadecimal
		CMidiSequencer( const unsigned char *pBuffer );
		virtual ~CMidiSequencer();

		//	Take an hexadeimal string and store it
		//	Must be on the form hx hx hx hx
		virtual void SetText ( const unsigned char *pBuffer );

		virtual const unsigned char *GetEventText () const;
		virtual void Trace ( FILE *hFile );

		virtual const char *GetEventLabel1 () const
		{
			return "Sequencer";
		}
};

