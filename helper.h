#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdint.h>

uint8_t *get_processed_body_of_image(uint8_t *image_data);
uint8_t *array_to_image_array(uint8_t *arr, int image_height, int image_width);

#endif