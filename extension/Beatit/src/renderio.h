#ifndef RENDERIO_H
#define RENDERIO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderdef.h"

typedef struct Info_Header{
    int width, height;
    uint16_t bits;
} header;

uint8_t getOffset(FILE *file);

void read_info_header(FILE *file, header *header);

void read_pixels(pixel_t **matrix, FILE *file, header *header, int offset);

spriteSt_t* readbmp(char* path);

spriteSt_t* read_display(char* path);

#endif
