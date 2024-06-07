#pragma once

#include <Windows.h>
#include "afxwin.h"

class CMWPen : public CPen
{
    public :
        virtual ~CMWPen()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};

class CMWFont : public CFont
{
    public :
        virtual ~CMWFont()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};

class CMWBrush : public CBrush
{
    public :
        virtual ~CMWBrush()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};

class CMWBitmap: public CBitmap
{
    public :
        virtual ~CMWBitmap()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};

