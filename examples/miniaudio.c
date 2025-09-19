#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"  // https://github.com/mackron/miniaudio
#include <stdlib.h>

#define UPRINTF_MAX_DEPTH -1
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(void) {
    ma_engine engine;
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) return EXIT_FAILURE;

    ma_engine_play_sound(&engine, "./examples/data/sample.flac", NULL);
    uprintf("Miniaudio engine: %S\n", &engine);

    ma_engine_uninit(&engine);
    return EXIT_SUCCESS;
}
