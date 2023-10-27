#include <stdio.h>
#include <stdlib.h>
#include "signal-private.h"


static int *terminate_ptr;

void signal_handler(int sig_num) {
    if (sig_num == SIGINT) {
        if (terminate_ptr) {
            *terminate_ptr = 1;
        }
        printf("SIGINT received\n");
        exit(0);
    }
}

void ctrlC_handler(int *terminate) {
    terminate_ptr = terminate;
    signal(SIGINT, signal_handler);
}