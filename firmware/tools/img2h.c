#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <ctype.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static int verbose_enabled = 0;

static void verbose(char *fmt, ...) {
    va_list ap;

    if (!verbose_enabled)
        return;

    va_start(ap, fmt);
    (void) vfprintf(stderr, fmt, ap);
    va_end(ap);
}

static void error(char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    (void) vfprintf(stderr, fmt, ap);
    va_end(ap);
}

static void help(const char *name, int exitvalue) {
    printf("Convert image to B12 header\n"
            "       %s [option] <in_file>\n"
            "option:\n"
            "  -h, --help              show this help messagen\n"
            "  -v, --verbose           show info about the process\n"
            "\n",
            name);
    exit(exitvalue);
}

static const struct option longopts[] =
{
    { "help",       no_argument,       NULL, 'h' },
    { "verbose",    no_argument,       NULL, 'v' },
    { NULL,         0,                 NULL, 0 }
};

int main(int argc, char **argv) {
    int opt;
    char *input = NULL;

    while ((opt = getopt_long(argc, (char * const *)argv, "hv", longopts, 0)) != -1) {
        switch (opt) {
            case 'h':
                help(argv[0], 0);
                break;
            case 'v':
                verbose_enabled = 1;
                break;
            default:
                printf("unknown option : -%c\n", opt);
                help(argv[0], -1);
        }
    }

    if (optind == argc) {
        printf("must input source image\n");
        help(argv[0], -1);
    }
    else {
        input = (char *)argv[optind];
    }

    if (access(input, R_OK) != 0) {
        error("Could not read file: %s\n", input);
    }

    int w, h, bpp;
    unsigned char *img = stbi_load(input, &w, &h, &bpp, STBI_rgb_alpha);
    verbose("image:   %d x %d x %d\n", w, h, bpp);

    char * t = rindex(input, '/');
    if (!t) t = input; else t++;
    char * name = strdup(t);
    char *dot = index(name, '.');
    *dot = '\0';

    char * NAME = strdup(name);
    for (char * N = NAME; *N != '\0'; N++) *N = toupper(*N);
    printf("#ifndef _IMG_%s_H\n", NAME);
    printf("#define _IMG_%s_H\n", NAME);

    printf("\n/* image from %s */\n", t);
    printf("/* cmdline: ");
    for (char ** argp = argv; *argp; argp++)
        printf("%s ", *argp);
    printf("*/\n\n");

    printf("const image_t img_%s = {\n", name);
    printf("    .w = %d,\n", w);
    printf("    .h = %d,\n", h);
    printf("    .buffer = {");
    unsigned char * p = img;
    for (int i = 0; i < w * h; i++) {
        unsigned char a, r, g, b;
        r = *p++ >> 4;
        g = *p++ >> 4;
        b = *p++ >> 4;
        a = *p++ >> 4;

        if (i % 16 == 0)
            printf("\n        ");
        else
            printf(" ");
        printf("0x%04x,", ((a << 12) | (r << 8) | (g << 4) | b));
    }
    printf("\n    }\n};\n\n");
    printf("#endif /* _IMG_%s_H */\n", NAME);
    printf("\n");

    free(name);
    free(NAME);
    stbi_image_free(img);
}

// vim: expandtab:ts=4:sw=4
