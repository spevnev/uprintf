#include "stb_vorbis.c"  // https://github.com/nothings/stb/blob/master/stb_vorbis.c
#define UPRINTF_IMPLEMENTATION
#include "uprintf.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [path-to-ogg]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int error = 0;
    stb_vorbis *vorbis = stb_vorbis_open_filename(argv[1], &error, NULL);
    if (vorbis == NULL) {
        fprintf(stderr, "Vorbis error: %d\n", error);
        return EXIT_FAILURE;
    }

    uprintf("Vorbis data: %S\n", vorbis);

    stb_vorbis_close(vorbis);
    return EXIT_SUCCESS;
}
