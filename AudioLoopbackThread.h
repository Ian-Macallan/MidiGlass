#pragma once

#include "AudioLoopback.h"

//
///////////////////////////////////////////////////////////////////////////////////
// CAudioLoopbackThread
//
///////////////////////////////////////////////////////////////////////////////////
class CAudioLoopbackThread : public CWinThread
{
	DECLARE_DYNCREATE(CAudioLoopbackThread)

	protected:
		CAudioLoopbackThread();           // constructeur protégé utilisé par la création dynamique
		virtual ~CAudioLoopbackThread();

	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();

	protected:
		DECLARE_MESSAGE_MAP()
};


