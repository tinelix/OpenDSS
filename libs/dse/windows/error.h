#ifndef DSE_ERROR_H
#define DSE_ERROR_H

typedef struct {
    int err_num;
    int err_cmd;
} DseError;

// Error codes
#define DSE_ERROR_PCM_OPEN -1

// Commands
#define DSE_COMMAND_PREPARE 0x8001

#endif // DSE_ERROR_H
