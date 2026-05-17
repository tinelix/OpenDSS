#include <stdio.h>
#include <crocon/crocon.h>
#include <dse/opendse.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

typedef enum {
	DSS_PLAYBACK_NOT_PLAYING,
	DSS_PLAYBACK_PLAYING,
	DSS_PLAYBACK_PAUSED,
	DSS_PLAYBACK_STOPPED
} dss_status_t;

dss_status_t dss_status = DSS_PLAYBACK_NOT_PLAYING;

void dss_listenkbd();

void dss_start_playback();

void dss_print_fileinfo(const char* path, DSE_OUTDEV outdev);

void dss_print_bytes_progress(
	unsigned int progress_width,
	unsigned int bytes_read, unsigned int bytes_total
);

void dss_wait_to_free_buffer();

int main(int argc, char** argv) {
	
	CROCVERSION crocon_ver;
	int crocon_result;
	DSE_OUTDEV outdev;
	
	setlocale(LC_ALL, NULL);
	
	crocon_result = crocon_getver(&crocon_ver);
	
	crocon_initscr();
	crocon_hidecurs();
	crocon_settitle("OpenDSS");
	
	crocon_cprintf(
		COLOR_BRIGHT_CYAN, 
		"OpenDSS Audio Player\r\n"
	);
	crocon_mvcprintf3(
		0, 1, COLOR_TRANSPARENT, 300,
		"Powered by CroconTUI %d.%d.%d", 
		crocon_ver.major, crocon_ver.minor, crocon_ver.patch
	);

	if(argc > 1) {

		int result = 0;

		outdev.id = 0;
		
		crocon_mvcprintf(
			0, 3, COLOR_BRIGHT_CYAN,
			"OPENING"
		);

		crocon_mvcprintf2(
			0, stdscr->metrics.height - 1, COLOR_BRIGHT_CYAN, COLOR_BLACK,
			" p "
		);

		crocon_mvcprintf(
			4, stdscr->metrics.height - 1, COLOR_TRANSPARENT,
			"Play/Pause"
		);

		crocon_mvcprintf2(
			17, stdscr->metrics.height - 1, COLOR_BRIGHT_CYAN, COLOR_BLACK,
			" s "
		);

		crocon_mvcprintf(
			21, stdscr->metrics.height - 1, COLOR_TRANSPARENT,
			"Stop"
		);

		crocon_mvcprintf2(
			34, stdscr->metrics.height - 1, COLOR_BRIGHT_CYAN, COLOR_BLACK,
			" q "
		);

		crocon_mvcprintf(
			38, stdscr->metrics.height - 1, COLOR_TRANSPARENT,
			"Quit"
		);

		if(dse_open_input(argv[1]) >= 0) {
			DSE_MMIO* mmio = stdmmio;
			int full_progress_width = stdscr->metrics.width;

			result = dse_open_outdev(&outdev);

			dss_print_fileinfo(argv[1], outdev);

			if(result < 0) {
				crocon_mvcprintf3(
					0, 3, COLOR_BRIGHT_RED, 120,
					"FAILED / OutDev Error: %d",
					result
				);
			} else {

				dse_alloc_audio();

				crocon_fillchar(0, 12, stdscr->metrics.width, 1, 0xB0);
				
				dss_start_playback();

				if (dss_status == DSS_PLAYBACK_STOPPED) {

					dss_print_bytes_progress(
						full_progress_width,
						stdmmio->bytes_total, stdmmio->bytes_total
					);
				}

				dss_listenkbd();
				
				dss_wait_to_free_buffer();

				crocon_fillcolor(
					0, 12, full_progress_width, 1, 
					COLOR_TRANSPARENT, COLOR_BRIGHT_CYAN
				);

				dse_close_outdev(&outdev);
			}
		} else {
			crocon_mvcprintf(
				0, 3, COLOR_BRIGHT_RED,
				"FAILED"
			);

		}


	}		

	crocon_freescr();

	return 0;
}

void dss_listenkbd() {
	while(1) {		
		char c;

		c = (char)crocon_getch();

		if(c == 'p') {
			if(dss_status == DSS_PLAYBACK_STOPPED)
				stdmmio->bytes_read = 47;

			dss_start_playback();

			if (dss_status == DSS_PLAYBACK_STOPPED) {
				dss_print_bytes_progress(
					stdscr->metrics.width,
					stdmmio->bytes_total, stdmmio->bytes_total
				);
			}

		} else if(c == 's') {
			dss_status = DSS_PLAYBACK_STOPPED;

			dss_print_bytes_progress(
				stdscr->metrics.width,
				stdmmio->bytes_read, stdmmio->bytes_total
			);
		} else if(c == 'q'){
		  	break;
		}

	};
}

void dss_print_fileinfo(const char* path, DSE_OUTDEV outdev) {
	double sample_rate = (stdmmio->audio.sample_rate / 1000.0);
	double bitrate = (stdmmio->audio.bitrate / 1000.0);
	char oem_outdev_name[160];
	char oem_path[300];
	char channel_label[14];

#ifdef MSVC_GE_800
	switch (outdev.channels) {
	case 1:
		sprintf_s(channel_label, 14, "Mono");
		break;
	case 2:
		sprintf_s(channel_label, 14, "Stereo");
		break;
	case 6:
		sprintf_s(channel_label, 14, "5.1 Surround");
		break;
	case 8:
		sprintf_s(channel_label, 14, "7.1 Surround");
		break;
	default:
		sprintf_s(channel_label, 14, "%2d ch.", outdev.channels);
		break;
	}
#else
	switch (outdev.channels) {
	case 1:
		sprintf(channel_label, "Mono");
		break;
	case 2:
		sprintf(channel_label, "Stereo");
		break;
	case 6:
		sprintf(channel_label, "5.1 Surround");
		break;
	case 8:
		sprintf(channel_label, "7.1 Surround");
		break;
	default:
		sprintf(channel_label, "%2d ch.", outdev.channels);
		break;
	}
#endif

	CharToOem(outdev.product_name, oem_outdev_name);
	CharToOem(path, oem_path);

	crocon_mvcprintf3(
		0, 4, COLOR_TRANSPARENT, 320,
		"File path:\t%s",
		oem_path
	);

	crocon_mvcprintf3(
		0, 5, COLOR_TRANSPARENT, 80,
		"Format:\t\t%s",
		stdmmio->audio.str_id
	);

	crocon_mvcprintf3(
		stdscr->metrics.width - 34, 4, COLOR_TRANSPARENT, 80,
		"  Sample rate:\t%7.2f kHz",
		sample_rate
	);

	if (bitrate < 1000)
		crocon_mvcprintf3(
			stdscr->metrics.width - 34, 5, COLOR_TRANSPARENT, 80,
			"  Bitrate:\t%7.2f kbps\r\n",
			bitrate
		);
	else
		crocon_mvcprintf3(
			stdscr->metrics.width - 34, 5, COLOR_TRANSPARENT, 80,
			"  Bitrate:\t%7.2f Mbps\r\n",
			bitrate / 1000
		);

	crocon_mvcprintf3(
		0, 7, COLOR_TRANSPARENT, 300,
		"OutDev name:\t%s\r\n",
		oem_outdev_name
	);

	crocon_mvcprintf3(
		0, 8, COLOR_TRANSPARENT, 300,
		"OutDev params:\t%7.2f kHz, %2d bit, %s\r\n",
		sample_rate, outdev.bit_depth, channel_label
	);
}

void dss_print_bytes_progress(
	unsigned int progress_width,
	unsigned int bytes_read, unsigned int bytes_total
) {
	
	double read_progress = ((double)bytes_read / bytes_total);
	rgbi4_t fg_color;
	
	switch(dss_status) {
		case DSS_PLAYBACK_PLAYING:
			fg_color = COLOR_BRIGHT_GREEN;

			crocon_mvcprintf(
				0, 3, fg_color,
				"PLAYING"
			);
			break;
		case DSS_PLAYBACK_PAUSED:
			fg_color = COLOR_BRIGHT_YELLOW;

			crocon_mvcprintf(
				0, 3, fg_color,
				"PAUSED "
			);
			break;
		case DSS_PLAYBACK_STOPPED:
			fg_color = COLOR_BRIGHT_CYAN;

			crocon_mvcprintf(
				0, 3, fg_color,
				"STOPPED"
			);
			break;
		default:
			fg_color = COLOR_TRANSPARENT;
			break;
	}

	if(bytes_read >= 1048576)
		crocon_mvcprintf3(
			0, 10, COLOR_TRANSPARENT, 120,
			"Position:\t%7.2f MB (%6.2f%%)  \r\n",
			((double)bytes_read / 1048576),
			read_progress * 100
		);
	else if(bytes_read >= 1024)
		crocon_mvcprintf3(
			0, 10, COLOR_TRANSPARENT, 120,
			"Position:\t%7.2f kB (%6.2f%%)  \r\n",
			((double)bytes_read / 1024),
			read_progress * 100
		);
	else 
		crocon_mvcprintf3(
			0, 10, COLOR_TRANSPARENT, 120,
			"Position:\t%4d bytes (%6.2f%%)\r\n",
			bytes_read,
			read_progress * 100
		);

	crocon_fillchar(0, 12, (int)(progress_width * read_progress), 1, 0xDB);
	crocon_fillcolor(0, 12, progress_width, 1, COLOR_TRANSPARENT, fg_color);
}

void dss_start_playback() {

	int full_progress_width = stdscr->metrics.width;
	
	dss_status = DSS_PLAYBACK_PLAYING;
	crocon_fillchar(0, 12, full_progress_width, 1, 0xB0);

	int result = 0;
	char c;
	int update_count = 0;
	
	while (stdmmio->bytes_read < stdmmio->bytes_total) {

		update_count++;

		if(crocon_kbhit()) {

			c = (char)crocon_getch();
			
			switch(c) { // 's' - to stop, 'p' - to pause
				case 's':
					dss_status = DSS_PLAYBACK_STOPPED;
					crocon_mvcprintf(
						0, 3, COLOR_BRIGHT_CYAN,
						"STOPPED"
					);
					break;
				case 'p':
					crocon_mvcprintf(
						0, 3, COLOR_BRIGHT_YELLOW,
						"PAUSED  "
					);
					dss_status = DSS_PLAYBACK_PAUSED;
					break;
			}	
		}
		
		dss_print_bytes_progress(
			full_progress_width,
			stdmmio->bytes_read, stdmmio->bytes_total
		);

		if(dss_status == DSS_PLAYBACK_PAUSED || dss_status == DSS_PLAYBACK_STOPPED)
			break;

		result = dse_decode_audio2(stdmmio->bytes_read);

		if(result < 0) {
			dss_status = DSS_PLAYBACK_STOPPED;
			break;
		}
	}
}

void dss_wait_to_free_buffer() {

	int result = 0;

	do {
		Sleep(100);
		result = dse_is_busy();
	} while(result != 0);

	dse_free_audio();
}
