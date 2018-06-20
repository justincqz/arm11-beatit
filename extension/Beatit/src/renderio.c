#include "renderio.h"

uint8_t getOffset(FILE *file) {
    uint32_t temp;
    uint32_t offset;
    fread(&temp, sizeof(uint16_t),1, file);  // Type
    fread(&temp, sizeof(uint32_t),1, file);  // Size
    fread(&temp, sizeof(uint16_t),1, file);  // RSv1
    fread(&temp, sizeof(uint16_t),1, file);  // RSv2
    fread(&offset, sizeof(uint32_t),1, file);  // Offset
    return offset;
}

void read_info_header(FILE *file, header *header) {
    uint32_t temp;
    fread(&temp, sizeof(uint32_t),1, file);  //Size
    fread(&header->width, sizeof(header->width),1, file); // Width
    fread(&header->height, sizeof(header->height),1, file); //Height
    fread(&temp, sizeof(uint16_t),1, file);  // Planes
    fread(&header->bits, sizeof(header->bits),1, file); // Bits
}

void read_pixels(pixel_t **matrix, FILE *file, header *header, int offset) {
    pixel_t *pixel = malloc(sizeof(pixel_t));
    if (pixel == NULL) {
        perror("memory allocation error");
        exit(EXIT_FAILURE);
    }
    fseek(file, offset, SEEK_SET);

    char x;

    for (int y = 0; y < header->height; y++) {
      for (int x = 0; x < header->width; x++) {
        fread(&pixel->blue, sizeof(char), 1, file);
        fread(&pixel->green, sizeof(char), 1, file);
        fread(&pixel->red, sizeof(char), 1, file);
        if (header->bits == 32) {
          fread(&x, sizeof(char), 1, file);
        }
        matrix[header->height - y - 1][x] = *pixel;
      }

      uint32_t padding = ((header->bits) / 8) * (header->width);
      while (padding % 4 != 0) {
        fread(&x, sizeof(char), 1, file);
        padding++;
      }
    }

    free(pixel);
}

spriteSt_t* readbmp(char* path) {
  FILE *file = fopen(path, "rb");
  if (!file) {
      perror("Wrong directory for input file\n");
      exit(EXIT_FAILURE);
  }

  /*initialize headers*/
  header *info_header = malloc(sizeof(header));
  uint8_t offset = getOffset(file);
  read_info_header(file, info_header);

  /*Initialise matrix*/
  pixel_t **matrix = allocPixelMatrix(info_header->height, info_header->width);
  read_pixels(matrix, file, info_header, offset);

  spriteSt_t* ret = new_spriteSt();
  ret->matrix = matrix;
  ret->dimX = info_header->width;
  ret->dimY = info_header->height;

  ret->nextState = NULL;

  free(info_header);
  return ret;
}

// Caps image at 714 x 374
spriteSt_t* read_display(char* path) {
  int width = 714;
  int height = 374;
  spriteSt_t* og = readbmp(path);

  if (og->dimX < width || og->dimY < height) {
    perror("File too small to fit into display!\n");
    exit(EXIT_FAILURE);
  }

  spriteSt_t* ret = new_spriteSt();
  ret->matrix = cpyPixelMatrix(og->matrix, width, height);
  ret->dimX = width;
  ret->dimY = height;
  ret->nextState = NULL;
  ret->frames = 0;
  ret->frames_left = 0;
  ret->currentStNo = 0;

  free_spriteSt(og);
  return ret;
}
