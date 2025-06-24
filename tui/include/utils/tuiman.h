#ifndef _OPENDSS_UTILS_PGUIMAN_H
#define _OPENDSS_UTILS_PGUIMAN_H

#include <opendss.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_DEEP_BLACK		 16
#define COLOR_LIGHT_CYAN         11

#ifdef _PDCURSES_256					// <-- Default console colors
  #define COLOR_LIGHT_WHITE 	255
  #define COLOR_GRAY			240
  #define COLOR_DARK_GRAY		235
  #define COLOR_LIGHT_RED		 82
  #define COLOR_LIGHT_GREEN		 47
  #define COLOR_BLUE_SKY		 68
#else 									// <-- Colors for VGA/EGA console
  #define COLOR_LIGHT_WHITE 	  6
  #define COLOR_GRAY			  15
  #define COLOR_DARK_GRAY		  8
  #define COLOR_LIGHT_RED		  1
  #define COLOR_LIGHT_GREEN		  2
  #define COLOR_BLUE_SKY		  3
#endif

class TUIManager {
	public:
		TUIManager(/*ITUIManager* pInterface*/);
		~TUIManager();
		void 	showTopBanner();
		void 	listenKbd();
		//void listenKbd(ExtWindowCtrl* pExtWnd);
		void    syncColors();
		WINDOW* getScreen();
		#ifdef _WIN32
			HWND 	findCmdWindow();
		#endif
		int 	gActiveWidth;
		int 	gActiveHeight;
		bool	isFullscreen;
	private:
		WINDOW* screen;
};

#endif
