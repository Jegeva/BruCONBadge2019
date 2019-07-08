#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <getopt.h>
#include <stdarg.h>
#include <ctype.h>
#include <ft2build.h>  
#include FT_FREETYPE_H

typedef struct font_char_s {
    uint16_t h;
    uint16_t w;
    int16_t left;
    int16_t top;
    int16_t advance;
    uint16_t buffer[];
} font_char_t;

void write_uint16(int i, const char * name)
{
    printf("   0x%04x, /* %s: %d */\n", (uint16_t)i, name, i);
}

void parse_part(char *str, char *render)
{
    char *to = index(str, '-');
    int end = 0;
    if (to) {
        *to = '\0';
        to++;
        end = strtol(to, NULL, 0);
    }
    int start = strtol(str, NULL, 0);
    do {
        render[start++] = 1;
    }
    while (start <= end);
}

static void error(char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    (void) vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(1);
}

static void help(const char *name, int exitvalue) {
    printf("Convert a TrueType font to a header\n"
            "       %s [options] <in_file>\n"
            "options:\n"
            "  -n, --name <name>       name of the font\n"
            "  -s, --size <size>       size of the font in pixels\n"
            "  -c, --chars <charlist>  select the characters. (default = 32-127\n"
            "                          other example:   32,48-57,65-90   (space, digits, uppercase chars)\n"
            "  -h, --help              show this help messagen\n"
            "\n",
            name);
    exit(exitvalue);
}

static const struct option longopts[] =
{
    { "name",       required_argument, NULL, 'n' },
    { "size",       required_argument, NULL, 's' },
    { "chars",      required_argument, NULL, 'c' },
    { "help",       no_argument,       NULL, 'h' },
    { NULL,         0,                 NULL, 0 }
};

int main(int argc, char **argv)
{
    int opt;
    char * input = NULL;
    char * name = NULL;
    int size = 16;
    char render[256] = { 0 };
    
    /* By default render characters between 32 (space) and 128 */
    for (int ch = 32; ch < 127; ch++)
        render[ch] = 1;

    while ((opt = getopt_long(argc, (char * const *)argv, "n:s:c:h", longopts, 0)) != -1) {
        switch (opt) {
            case 'n':
                name = optarg;
                break;
            case 's':
                size = strtol(optarg, NULL, 0);
                break;
            case 'c':
                memset(render, 0, sizeof(render));
                char * p = optarg;
                char * q;
                while ((q = index(p, ','))) {
                    *q = '\0';
                    parse_part(p, render);
                    p = q + 1;
                }
                parse_part(p, render);
                break;
            case 'h':
                help(argv[0], 0);
                break;
            default:
                printf("unknown option : -%c\n\n", opt);
                help(argv[0], -1);
        }
    }

    if (optind == argc) {
        printf("must supply font file\n\n");
        help(argv[0], -1);
    }
    else {
        input = (char *)argv[optind];
    }

    if (access(input, R_OK) != 0)
        error("Could not read file: %s\n", input);

    FT_Library library;
    if (FT_Init_FreeType(&library))
        error("Error initializing freetype...\n");

    FT_Face face;
    if (FT_New_Face(library, input, 0, &face))
        error("Could not load font %s\n", input);

    FT_Set_Pixel_Sizes(face, 0, size);

    char * t = rindex(input, '/');
    if (!t) t = input; else t++;

    char * NAME = strdup(name);
    for (char * N = NAME; *N != '\0'; N++) *N = toupper(*N);
    printf("#ifndef _FONT_%s_%d_H\n", NAME, size);
    printf("#define _FONT_%s_%d_H\n", NAME, size);

    printf("\n/* Font rendered from %s */\n", t);
    printf("/* cmdline: ");
    for (char ** argp = argv; *argp; argp++)
        printf("%s ", *argp);
    printf("*/\n\n");

    printf("const uint16_t font_%s_%d_data[] = {\n\n", name, size);

    /* Loop over every character */
    int offset = 0;
    int chars[255] = { 0 };
    int min_index = 255;
    int max_index = 0;
    for (int ch = 32; ch <= 255; ch++) {
        if (!render[ch])
            continue;

        if (ch < min_index) min_index = ch;
        if (ch > max_index) max_index = ch;

        int glyph_index = FT_Get_Char_Index(face, ch);
        FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        FT_Bitmap *bm = &face->glyph->bitmap;

        font_char_t fc;
        fc.h = bm->rows;
        fc.w = bm->width;
        fc.left = face->glyph->bitmap_left;
        fc.top = face->glyph->bitmap_top;
        fc.advance = face->glyph->advance.x >> 6;

        printf("   /*  %c  (%d) */\n\n", ch, offset);

        chars[ch] = offset;

        write_uint16(fc.h,       "height");
        write_uint16(fc.w,       " width");
        write_uint16(fc.left,    "  left");
        write_uint16(fc.top,     "   top");
        write_uint16(fc.advance, "   adv");

        offset +=  5;

        printf("   ");

        if ((fc.h != 0) && (fc.w != 0)) {
            unsigned char * p = bm->buffer;
            int cc = 0;
            uint16_t d = 0;

            for (int y = 0; y < fc.h; y ++) {
                for (int x = 0; x < fc.w; x++) {
                    d |= (uint16_t)((p[x] >> 4) & 0xF) << ((cc % 4) * 4);
                    cc++;
                    if ((cc % 4) == 0) {
                        printf("0x%04x, ", d);
                        d = 0;
                    }
                    if ((cc % 32) == 0)
                        printf("\n   ");
                }
                p += bm->pitch;
            }

            if ((cc % 4) != 0)
                printf("0x%04x, ", d);

            offset += ((fc.w * fc.h) + 3) / 4;
            printf("\n");
        }

        printf("\n");
    }

    printf("};\n\n");

    /* Output the lookup table with offset to each character */
    printf("const uint16_t font_%s_%d_lookup[] = {\n", name, size);

    for (int ch = min_index; ch <= max_index; ch++) {
        printf(" %5d,  ", chars[ch]);
        if(ch >= 32 && ch < 127) printf("/*  %c  */", ch);
        printf("\n");
    }
    printf("};\n\n");

    printf("const font_t font_%s_%d = {\n", name, size);
    printf("    .lookup = font_%s_%d_lookup,\n", name, size);
    printf("    .data   = font_%s_%d_data,\n", name, size);
    printf("    .min    = %d,\n", min_index);
    printf("    .max    = %d,\n", max_index);
    printf("    .height = %d\n", size);
    printf("};\n\n");

    printf("#endif /* _FONT_%s_%d_H */\n\n", NAME, size);

    free(NAME);

    return 0;
}

// vim: expandtab:ts=4:sw=4
