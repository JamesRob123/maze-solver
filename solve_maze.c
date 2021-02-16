#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bmp.h"
#include "helper.h"
#include "maze.h"

void handle_invalid_usage(char *argv[]);
void handle_error(char *message);

int main(int argc, char *argv[]) {
	clock_t begin = clock();
	
	if (argc < 2) {
		handle_invalid_usage(argv); 
	}
	
	char *file_name = argv[1];
	
	printf("Loading image... ");
	fflush(stdout);
	uint8_t *buf = bmp_read_bitmapfile(file_name);
	if (buf == NULL) {
		printf("[failed]\n");
		handle_error("Error in bmp_read_bitmapfile(). Check that the filename is correct");
	}
	printf("[done]\n");
	
	BITMAPHEADER *header = (BITMAPHEADER *)buf;
	printf("Processing image... ");
	fflush(stdout);
	uint8_t *processed_body = get_processed_body_of_image(buf);
	if (processed_body == NULL) {
		printf("[failed]\n");
		handle_error("Error in get_processed_body_of_images()");
	}
	printf("[done]\n");
	
	int *path = NULL;
	printf("Solving maze... ");
	fflush(stdout);
	int length = maze_solve_dfs(processed_body, header->biHeight, header->biWidth, &path);
	if (length == -1) {
		printf("[failed]\n");
		handle_error("Error in maze_solve_dfs(). The maze might not have a solvable path");
	}
	printf("[done]\n");
	printf("Path length: %d\n", length);
	
	for (int i = 0; i < length; ++i) {
		processed_body[path[i]] = 2;
	}
	
	printf("Converting solved maze to image format... ");
	fflush(stdout);
	uint8_t *image_array = array_to_image_array(processed_body, header->biHeight, header->biWidth);
	if (image_array == NULL) {
		printf("[failed]\n");
		handle_error("Error in array_to_image_array()");
	}
	printf("[done]\n");
	
	printf("Creating image... ");
	int result = bmp_create_image(*header, image_array, "temp.bmp");
	if (result == -1) {
		printf("[failed]");
		handle_error("Error in bmp_create_image");
	}
	printf("[done]\n");
	
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time elapsed: %.3fs\n", time_spent);
	
	free(image_array);
	free(processed_body);
	free(buf);
	free(path);
	return 0;
}

void handle_invalid_usage(char *argv[]) {
	fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
	exit(EXIT_FAILURE);
}

void handle_error(char *message) {
	char *err_message = malloc(strlen(message) + 50);
	strcpy(err_message, message);
	strcat(err_message, " : terminating the process...");
	fprintf(stderr, "%s\n", message);
	free(err_message);
	exit(EXIT_FAILURE);
}