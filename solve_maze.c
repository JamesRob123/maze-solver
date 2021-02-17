#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bmp.h"
#include "helper.h"
#include "maze.h"

void handle_invalid_usage(char *argv[]);
void handle_error(char *message);
uint8_t *load_image(char *file_name);
uint8_t *get_maze_from_image(uint8_t *image);
int solve_maze(uint8_t *maze, int height, int width, int **path);
void write_path_to_maze(uint8_t *maze, int *path, int length);
uint8_t *create_image_array_from_maze(uint8_t *maze, int height, int width);
void create_image_from_image_array(BITMAPHEADER header, uint8_t *maze, char *file_name);
void print_elapsed_time(clock_t begin);

int main(int argc, char *argv[]) {
	clock_t begin = clock();
	
	if (argc < 2) {
		handle_invalid_usage(argv); 
	}
	
	char *file_name = argv[1];
	uint8_t *image = load_image(file_name);
	BITMAPHEADER *header = (BITMAPHEADER *)image;
	int height = header->biHeight;
	int width = header->biWidth;
	uint8_t *maze = get_maze_from_image(image);
	
	int *path;
	int length = solve_maze(maze, height, width, &path);
	write_path_to_maze(maze, path, length);
	uint8_t *image_array = create_image_array_from_maze(maze, height, width);
	create_image_from_image_array(*header, image_array, "temp.bmp");
	
	print_elapsed_time(begin);
	
	free(image_array);
	free(maze);
	free(image);
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

uint8_t *load_image(char *file_name) {
	printf("Loading image... ");
	fflush(stdout);
	uint8_t *image = bmp_read_bitmapfile(file_name);
	if (image == NULL) {
		printf("[failed]\n");
		handle_error("Error in bmp_read_bitmapfile(). Check that the filename is correct");
	}
	printf("[done]\n");
	return image;
}

uint8_t *get_maze_from_image(uint8_t *image) {
	printf("Processing image... ");
	fflush(stdout);
	uint8_t *processed_body = get_processed_body_of_image(image);
	if (processed_body == NULL) {
		printf("[failed]\n");
		handle_error("Error in get_processed_body_of_images()");
	}
	printf("[done]\n");
	return processed_body;
}

int solve_maze(uint8_t *maze, int height, int width, int **path) {
	printf("Solving maze... ");
	fflush(stdout);
	int length = maze_solve_dfs(maze, height, width, path);
	if (length == -1) {
		printf("[failed]\n");
		handle_error("Error in maze_solve_dfs(). The maze might not have a solvable path");
	}
	printf("[done]\n");
	printf("Path length: %d\n", length);
	return length;
}

void write_path_to_maze(uint8_t *maze, int *path, int length) {
	for (int i = 0; i < length; ++i) {
		maze[path[i]] = 2;
	}
}

uint8_t *create_image_array_from_maze(uint8_t *maze, int height, int width) {
	printf("Converting solved maze to image format... ");
	fflush(stdout);
	uint8_t *image_array = array_to_image_array(maze, height, width);
	if (image_array == NULL) {
		printf("[failed]\n");
		handle_error("Error in array_to_image_array()");
	}
	printf("[done]\n");
	return image_array;
}

void create_image_from_image_array(BITMAPHEADER header, uint8_t *maze, char *file_name) {
	printf("Creating image... ");
	int result = bmp_create_image(header, maze, file_name);
	if (result == -1) {
		printf("[failed]");
		handle_error("Error in bmp_create_image");
	}
	printf("[done]\n");
}

void print_elapsed_time(clock_t begin) {
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time elapsed: %.3fs\n", time_spent);
}