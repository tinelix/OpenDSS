#include "dse.h"

#ifdef __cplusplus
extern "C" {
#endif

int result = 0;

int dse_init(void) {

    #ifdef _WIN32

    #else
        result = dse_alsa_init();
    #endif

    return result;
}

int dse_prepare(void) {

    #ifdef _WIN32

    #else
        result = dse_alsa_prepare();
    #endif

    return result;
}

int dse_open_src(char path[400]) {
    return 0;
}

#ifdef __cplusplus
}
#endif
