#ifndef DSE_LINUX_DSE_H
#define DSE_LINUX_DSE_H

#include <alsa/asoundlib.h>

typedef struct {
    char src_path[400];
    DseError *last_error;
} DigitalSoundEngine;

int dse_init(void);
int dse_prepare(void);
int dse_open_src(char path[400]);

#endif // DSE_LINUX_DSE_H
