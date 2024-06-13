#pragma once

#ifndef AUTOMATICPRODUCTNAME_H
#define AUTOMATICPRODUCTNAME_H    1

#ifdef _WIN64
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"MidiGlass (x64 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"MidiGlass - Version 4.0.04.010\r\n(Build 213) - (x64 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "MidiGlass (x64 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "MidiGlass - Version 4.0.04.010\r\n(Build 213) - (x64 MBCS) (MSVC)\0"
#endif
#elif _WIN32
#ifdef UNICODE
#define ORIGINAL_FILENAME   L"MidiGlass (x86 Unicode) (MSVC)\0"
#define PRODUCT_NAME        L"MidiGlass - Version 4.0.04.010\r\n(Build 213) - (x86 Unicode) (MSVC)\0"
#else
#define ORIGINAL_FILENAME   "MidiGlass (x86 MBCS) (MSVC)\0"
#define PRODUCT_NAME        "MidiGlass - Version 4.0.04.010\r\n(Build 213) - (x86 MBCS) (MSVC)\0"
#endif
#else
#define ORIGINAL_FILENAME   "MidiGlass (MSVC)\0"
#define PRODUCT_NAME        "MidiGlass - Version 4.0.04.010\r\n(Build 213) - (MSVC)\0"
#endif

#endif
