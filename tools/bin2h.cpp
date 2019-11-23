#include <stdio.h>

int main(int argc, char *argv[]) {
    char header_filename[256];

    if (argc < 3) {
        printf("Usage: %s [file] [variable]\n", argv[0]);
        printf("       %s eng.fnt eng_font\n", argv[0]);
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Error! file not found: %s\n", argv[1]);
        return 0;
    }

    sprintf(header_filename, "%s.h", argv[2]);
    FILE *out = fopen(header_filename, "w");

    fprintf(out, "const unsigned char %s[] = {\n", argv[2]);

    while (!feof(fp)) {
        unsigned char value = 0;
        fread(&value, 1, 1, fp);
        fprintf(out, "0x%02x,", value);
    }

    fprintf(out, "};\n\n");

    fclose(out);
    fclose(fp);

    return 0;
}
