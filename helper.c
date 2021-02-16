#include <stdint.h>
#include <stdlib.h>
#include "helper.h"
#include "bmp.h"

uint8_t *get_processed_body_of_image(uint8_t *image_data) {
	BITMAPHEADER *header = (BITMAPHEADER *)image_data;
	int height = header->biHeight;
	int width = header->biWidth;
	int padding = (4 - (width * 3) % 4) % 4;
	
	uint8_t *processed_body = malloc(height * width);
	if (processed_body == NULL) {
		return NULL;
	}
	uint8_t *current = image_data + HEADER_SIZE;
	int n = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width * 3; j += 3) {
			processed_body[n++] = current[i * width * 3 + i * padding + j];
		}
	}
	return processed_body;
}

uint8_t *array_to_image_array(uint8_t *arr, int image_height, int image_width) {
	/* Every line needs to be word(4 bytes) aligned and this is the padding if it isn't */
	int padding = (4 - (image_width * 3) % 4) % 4;
	uint8_t *image_array = malloc(image_height * image_width * 3 + padding * image_height);
	if (image_array == NULL) {
		return NULL;
	}
	for (int i = 0, j = 0; i < image_height * image_width; ++i) {
		/* Need to repeat 3 times because every pixel is 3 bytes*/
		for (int k = 0; k < 3; ++k) {
			if (arr[i] == 2 && (k == 1 || k == 2)) {
				image_array[j++] = 0;
				continue;
			}
			image_array[j++] = arr[i] == 0 ? 0 : arr[i] == 2 ? 0x7f : 0xff;
		}
		if (i % image_width == image_width - 1) {
			for (int k = 0; k < padding; ++k) {
				image_array[j++] = 0;
			}
		}
	}
	return image_array;
}