#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check for single command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }
    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Unable to open file.  Usage: ./recover FILE\n");
        return 1;
    }
    // Create a buffer to store the data
    uint8_t buffer[512];
    // While there's still data left to read from the memory card
    int counter = 0;
    FILE *img;
    img = NULL;
    char filename[8];
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Check for new JPEG Header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Check if the img file is already open and close
            if (img != NULL)
            {
                fclose(img);
            }
            // Create JPEGs from the data
            // create the new file, making it at least 3 digits and providing leading zeroes
            sprintf(filename, "%03d.jpg", counter);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                return 1;
            }
            counter++;
            fwrite(buffer, 512, 1, img);
        }
        else if (img != NULL)
        {
            fwrite(buffer, 512, 1, img);
        }
    }
    fclose(img);
    fclose(card);
}
