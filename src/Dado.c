#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/timer.h"

// ----------------------------------------------
int rolarDado20() {
    static unsigned long long seed = 0;
    if (seed == 0) {
        timerInit(1);
        seed = (unsigned long long)time(NULL) ^ (unsigned long long)DadoTimeOver();
        timerUpdateTimer(1);
    }

    if (DadoTimeOver()) {
        seed = (seed * 6364136223846793005ULL + 1442695040888963407ULL);
        timerUpdateTimer(1);
    }

    return (int)((seed >> 32) % 20) + 1;
}
// ----------------------------------------------

