#ifndef DSE_DSE_H
#define DSE_DSE_H

#ifdef _WIN32
    #include "windows/dse.h"
#else
    #include <alsa/asoundlib.h>
    #include "linux/dse.h"
#endif

typedef struct {
    char src_path[400];
    DseError *last_error;
} DigitalSoundEngine;

int dse_init(void);
int dse_prepare(void);
int dse_open_src(char path[400]);

#endif // DSE_DSE_H
