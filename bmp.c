#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "bmp.h"

static bool __is_valid_bmp_header(uint8_t *header, int size);

uint8_t *bmp_read_bitmapfile(char *name) {
	FILE *image = fopen(name, "r");
	uint8_t *buf = malloc(HEADER_SIZE);
	int read_bytes = fread(buf, 1, HEADER_SIZE, image);
	
	if (__is_valid_bmp_header(buf, read_bytes) == false) {
		fclose(image);
		return NULL;
	}
	
	BITMAPHEADER *header = (BITMAPHEADER *)buf;
	int height = header->biHeight;
	int width = header->biWidth;
	/* Every line that isn't word(4 bytes) aligend has a padding */
	int padding = (4 - (width * 3) % 4) % 4;
	
	buf = realloc(buf, HEADER_SIZE + height * width * 3 + height * padding);
	read_bytes = fread(buf + HEADER_SIZE, 1, height * width * 3 + height * padding, image);
	
	fclose(image);
	return buf;
}

static bool __is_valid_bmp_header(uint8_t *header, int size) {
	if (size != HEADER_SIZE) {
		return false;
	}
	/* the first 2 bytes are alway 0x4d42 */
	if (*(short *)header != 0x4d42) {
		return false;
	}
	return true;
}

BITMAPHEADER bmp_create_bitmapheader(int height, int width) {
	BITMAPHEADER header = {};
	int padding = (4 - (width * 3) % 4) % 4;
	
	header.bfType = 0x4d42;
    header.bfSize = HEADER_SIZE + width * height * 3 + height * padding;
    header.bfReserved = 0;
    header.bfOffBits = HEADER_SIZE;
    header.biSize = 40;
    header.biWidth = width;
    header.biHeight = height;
    header.biPlanes = 1;
    header.biBitCount = 24;
    header.biCompression = 0;
    header.biSizeImage = width * height * 3 + height * padding;
    header.biXPelsPerMeter = 0;
    header.biYPelsPerMeter = 0;
    header.biClrUsed = 0;
    header.biClrImportant = 0;
	
	return header;
}

int bmp_create_image(BITMAPHEADER header, uint8_t *data, char *name) {
	int padding = (4 - (header.biWidth * 3) % 4) % 4;
	FILE *image = fopen(name, "w");
	if (image == NULL) {
		return -1;
	}
	uint8_t *buf = malloc(HEADER_SIZE + header.biHeight * header.biWidth * 3 + padding * header.biHeight);
	if (buf == NULL) {
		fclose(image);
		return -1;
	}
	
	memcpy(buf, &header, HEADER_SIZE);
	memcpy(buf + HEADER_SIZE, data, header.biHeight * header.biWidth * 3 + padding * header.biHeight);
	fwrite(buf, 1, HEADER_SIZE + header.biHeight * header.biWidth * 3 + padding * header.biHeight, image);
	fclose(image);
	free(buf);
	return 1;
}