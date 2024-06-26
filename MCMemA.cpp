/*
 ************************************************************************************
 *
 *      File:           MCMemA.cpp
 *      Class:          MCMemA
 *
 ************************************************************************************
 *      Description:
 *
 ************************************************************************************
 *      Notes:
 *
 ************************************************************************************
 *
 *      E/Mail: ian.macallan@gmail.com
 *      (c) 2003 - 2023 / Macallan
 *      
 ************************************************************************************
 *
 */
#include "stdafx.h"
#include "MCMemA.h"

#include <Windows.h>
#include <stdlib.h>

//
//====================================================================================
//
//====================================================================================
MCMemA::MCMemA(void)
{
    len = 0;
    ptr = NULL;
}

//
//====================================================================================
//
//====================================================================================
MCMemA::MCMemA(int sizeInBytes )
{
    len = 0;
    ptr = NULL;
    AllocateA ( sizeInBytes );
}

//
//====================================================================================
//
//====================================================================================
MCMemA::MCMemA ( const char *pText, int sizeInBytes  )
{
    if ( sizeInBytes < 0 )
    {
        sizeInBytes = (int) strlen(pText);
    }

    AllocateA ( sizeInBytes );

    memcpy_s ( ptr, len, pText, sizeInBytes );
}

//
//====================================================================================
//
//====================================================================================
char *MCMemA::AllocateA(int sizeInBytes )
{
    if ( sizeInBytes < 8 )
    {
        sizeInBytes = 8;
    }
    Free();
    len =  sizeInBytes + 1;
    ptr = (char*) malloc(len);
    ZeroMemory ( ptr, len );
    return (char *) ptr;
}

//
//====================================================================================
//
//====================================================================================
void MCMemA::fromPointer( char *pText, int sizeInBytes )
{
    Free();

    len = sizeInBytes;
    ptr = pText;
}

//
//====================================================================================
//
//====================================================================================
void MCMemA::Free(void)
{
    if ( ptr != NULL )
    {
        free ( ptr );
        ptr = NULL;
    }
}

//
//====================================================================================
//
//====================================================================================
MCMemA::~MCMemA(void)
{
    Free();
}
