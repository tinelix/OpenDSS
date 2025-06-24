#ifndef _OPENDSS_H
#define _OPENDSS_H

#include "version.h"
#include "utils/cpp_ext.h"

#ifdef _MSVC
	#ifdef _PDCURSES
		#include <curses.h>
	#else
		#include <ncurses.h>
	#endif
#elif _WATCOM
	#include <curses.h>
#else
	#ifdef __PDCURSES__
		#include <curses.h>
	#elif __MINGW64__
		#include <ncurses/ncurses.h>
	#else
		#include <ncurses.h>
	#endif
#endif

#ifdef _WIN32
	#include <windows.h>
	#define WIN32_LEAN_AND_MEAN
#endif

//#include <framedir/include/framedir.h>

class OpenDSS {
public:
	static char* getVersion() {
		char* version = new char[24];
		
		#if _MSC_VER >= 1400	// Microsoft Visual C++ 2005
			#ifdef OPENDSS_SUFFIX
				sprintf_s(version, 24, "%s-%s", OPENDSS_VERSION, OPENDSS_SUFFIX);
			#else
				sprintf_s(version, 24, "%s", OPENDSS_VERSION);
			#endif
		#else
			#ifdef OPENDSS_SUFFIX
				sprintf(version, "%s-%s", OPENDSS_VERSION, OPENDSS_SUFFIX);
			#else
				sprintf(version, "%s", OPENDSS_VERSION);
			#endif
		#endif
		
		return version;	
	}

	static char* getReleaseYears() {
		char* years = new char[15];
		
		#ifdef _MSVC2005G
			sprintf_s(years, 15, "%s", OPENDSS_RELEASE_YEARS);
		#else
			sprintf(years, "%s", OPENDSS_RELEASE_YEARS);
		#endif
		
		return years;	
	}
};

#endif
