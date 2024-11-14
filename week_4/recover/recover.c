#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //Accept only a single line command argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    //Open memory card
    FILE *raw_file = fopen(argv[1], "r");

    if (raw_file == NULL)
    {
        printf("File could not be opened\n");
        return 1;
    }

    //Repeat until end of card:
    uint8_t buffer[512];
    int file_count = 0;
    char *file_name = malloc(sizeof("###.jpg"));
    bool first_jpg_found = false;
    unsigned char jpg_header[] = {0xff, 0xd8, 0xff, 0xe0};
    FILE *img;
    while (fread(&buffer, sizeof(int8_t), 512,raw_file) == 512)
    {
        //If start of first JPEG
        if (buffer[0] == jpg_header[0] && buffer[1] == jpg_header[1] && buffer[2] == jpg_header[2] && (buffer[3] & 0xf0) == jpg_header[3]) {
            //If first JPEG
            if (first_jpg_found == false) {
                first_jpg_found = true;
            }
            else {
                fclose(img);
                file_count++;
            }
            sprintf(file_name, "%03i.jpg", file_count);
            img = fopen(file_name, "w");
            fwrite(&buffer, sizeof(int8_t), 512, img);

        }
        else if (first_jpg_found == true) {
            fwrite(&buffer, sizeof(int8_t), 512, img);
        }

    }
    //close any remaining files
    free(file_name);
    fclose(img);
    fclose(raw_file);
    return 0;
}
