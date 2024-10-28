#include "dse.h"

DigitalSoundEngine *dse;
int result = -1;

int dse_mci_init(void) {

    if(result >= 0) {
        dse = new DigitalSoundEngine();
    }

    return result;
}

int dse_mci_prepare(void) {
    if(result < 0) {
        snd_pcm_hw_params_free(hwparams);
        snd_pcm_close(handle);
        return result;
    }

    return result;
}

int dse_mci_open_src(char path[400]) {
    return 0;
}
