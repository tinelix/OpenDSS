#ifndef _CPP_EXT
#define _CPP_EXT

// Missing macros for Microsoft Visual C++ 2.0 and Open Watcom
#ifdef _MSVC
#if _MSC_VER = 800
    typedef unsigned char bool;
    #define true ((bool)1)
    #define false ((bool)0)
#endif
#elif _WATCOM
    #define true ((bool)1)
    #define false ((bool)0)
#endif


#endif
