// AudioLoopbackThread.cpp�: fichier d'impl�mentation
//

#include "stdafx.h"
#include "AudioLoopbackThread.h"

#include "AudioLoopback.h"

//
///////////////////////////////////////////////////////////////////////////////////
// CAudioLoopbackThread
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CAudioLoopbackThread, CWinThread)

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CAudioLoopbackThread::CAudioLoopbackThread()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CAudioLoopbackThread::~CAudioLoopbackThread()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CAudioLoopbackThread::InitInstance()
{
	// TODO:  effectuez ici une initialisation par thread
	startAudioLoopBack();

	return TRUE;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CAudioLoopbackThread::ExitInstance()
{
	// TODO:  effectuez ici un nettoyage par thread
	return CWinThread::ExitInstance();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAudioLoopbackThread, CWinThread)
END_MESSAGE_MAP()


// Gestionnaires de messages de CAudioLoopbackThread
