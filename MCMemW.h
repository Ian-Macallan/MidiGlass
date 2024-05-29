/*
 ************************************************************************************
 *
 *      File:           MCMemW.h
 *      Class:          MCMemW
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
#pragma once

#include <Windows.h>

//
//====================================================================================
//
//====================================================================================
class MCMemW
{
    public :
        //  Size In Bytes
        int len;
        WCHAR *ptr;

    public:
        MCMemW(void);
        //  Size In Chars
        MCMemW( int sizeInChars );
        MCMemW ( const WCHAR *pText, int sizeInChars  );
        virtual  WCHAR *AllocateW( int sizeInChars );
        virtual void fromPointer ( WCHAR *pText, int sizeInChars );
        virtual size_t wlen();
        virtual void Free();
        virtual ~MCMemW(void);

};

