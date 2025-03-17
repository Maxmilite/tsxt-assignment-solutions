#ifndef __LOGGER_H
#define __LOGGER_H

#include <stdio.h>

#define DEBUG_LOG

#define LOG_INFO (0)
#define LOG_WARN (1)
#define LOG_ERR (-1)

void logger(int type, char* message) {
    #ifdef DEBUG_LOG
    if (type == LOG_INFO) {
        fprintf(stderr, "[INFO] ");
    } else if (type == LOG_WARN) {
        fprintf(stderr, "[WARN] ");
    } else if (type == LOG_ERR) {
        fprintf(stderr, "[ERROR] ");
    } else {
        fprintf(stderr, "[UNKNOWN] ");
    }
    fprintf(stderr, "%s\n", message);
    #endif
}

#endif