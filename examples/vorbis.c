#include "stb_vorbis.c"  // https://github.com/nothings/stb/blob/master/stb_vorbis.c

#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(int argc, char *argv[]) {
    const char *filepath = argc == 2 ? argv[1] : "./examples/data/sample.ogg";

    int error = 0;
    stb_vorbis *vorbis = stb_vorbis_open_filename(filepath, &error, NULL);
    if (vorbis == NULL) {
        fprintf(stderr, "Vorbis error: %d\n", error);
        return EXIT_FAILURE;
    }

    uprintf("Vorbis data: %S\n", vorbis);

    stb_vorbis_close(vorbis);
    return EXIT_SUCCESS;
}
