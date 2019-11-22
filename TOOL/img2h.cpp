#include <stdint.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma pack(push, 1)
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;
#pragma pack(pop)

static char g_header_filename[256];
static FILE *g_fp;
static uint8_t *g_buf;
static uint16_t g_width;
static uint16_t g_height;
static rgb_t g_pal[256];

uint8_t findNearestIndex(uint8_t r, uint8_t g, uint8_t b) {
    return r;
}

void convert32bpp() {
}

void convert24bpp() {
    for (int i = 0; i < g_height; i++) {
        for (int j = 0; j < g_width; j++) {
            uint8_t r = g_buf[(g_width * i + j) * 3 + 0];
            uint8_t g = g_buf[(g_width * i + j) * 3 + 1];
            uint8_t b = g_buf[(g_width * i + j) * 3 + 2];

            fprintf(g_fp, "0x%02x,", findNearestIndex(r, g, b));
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s [image] [palette] [variable]\n", argv[0]);
        printf("       %s sample.png dp256.pal sample\n", argv[0]);
        return 0;
    }

    FILE *fp = fopen(argv[2], "r");
    if (!fp) {
        printf("File not found: %s\n", argv[2]);
        return 0;
    }

    fread(g_pal, 3, 256, fp);
    fclose(fp);

    int width = 0;
    int height = 0;
    int channels = 0;
    g_buf = stbi_load(argv[1], &width, &height, &channels, 0);

    g_width = (uint16_t)width;
    g_height = (uint16_t)height;

    printf("* Info\n");
    printf("- Width: %d\n", g_width);
    printf("- Height: %d\n", g_height);
    printf("- Channels: %d\n", channels);

    sprintf(g_header_filename, "%s.h", argv[3]);
    g_fp = fopen(g_header_filename, "w");

    printf("- Output: %s\n", g_header_filename);

    fprintf(g_fp, "unsigned char %s[] = {\n", argv[3]);
    fprintf(g_fp, "0x%02x, 0x%02x,\n", *(((uint8_t *)&g_width) + 0), *(((uint8_t *)&g_width) + 1));
    fprintf(g_fp, "0x%02x, 0x%02x,\n", *(((uint8_t *)&g_height) + 0), *(((uint8_t *)&g_height) + 1));

    switch (channels) {
        case 4:
            convert32bpp();
            break;

        case 3:
            convert24bpp();
            break;

        default:
            printf("Unsupported format!\n");
            break;
    }

    fprintf(g_fp, "};\n\n");

    fclose(g_fp);

    stbi_image_free(g_buf);

    return 0;
}
