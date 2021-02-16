#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "maze.h"
#include "array_list.h"

// static int __maze_solve_dfs(uint8_t *maze, int height, int width, bool *visited, int current_cell, int end_cell);
static int __find_start(uint8_t *maze, int height, int width);
static int __find_end(uint8_t *maze, int height, int width);
static int __get_path(uint8_t *maze, int height, int width, int start_cell, int end_cell, int **path);
static bool __is_valid_cell(int current_cell, int height, int width);
static int *__array_list_to_array(struct array_list *array_l);

int maze_solve_dfs(uint8_t *maze, int height, int width, int **path) {
	/* Using recursen */
	/* bool *visited = calloc(1, height * width);
	if (visited == NULL) {
		return -1;
	}
	int start = __find_start(maze, height, width);
	int end = __find_end(maze, height, width);
	int iResult = __maze_solve_dfs(maze, height, width, visited, start, end);
	free(visited); */
	
	int start_cell = __find_start(maze, height, width);
	if (start_cell == -1) {
		return -1;
	}
	int end_cell = __find_end(maze, height, width);
	if (end_cell == -1) {
		return -1;
	}
	
	int path_length = __get_path(maze, height, width, start_cell, end_cell, path);
	
	return path_length;
}

static int __find_start(uint8_t *maze, int height, int width) {
	height = height;
	for (int i = 0; i < width; ++i) {
		if (maze[i] == 255) {
			return i;
		}
	}
	return -1;
}

static int __find_end(uint8_t *maze, int height, int width) {
	height = height;
	for (int i = 0; i < width; ++i) {
		if (maze[(height - 1) * width + i] == 255) {
			return (height - 1) * width + i;
		}
	}
	return -1;
}

static int __get_path(uint8_t *maze, int height, int width, int start_cell, int end_cell, int **path) {
	bool *visited = calloc(1, height * width);
	if (visited == NULL) {
		*path = NULL;
		return -1;
	}
	/* Using an array list as a stack because I don't have implemented a stack yet */
	struct array_list *st = array_list_create(sizeof(int));
	int result = array_list_add(st, (void *)&start_cell);
	if (result == -1) {
		goto exit;
	}
	while (st->length > 0) {
		void *temp = array_list_get(st, st->length - 1);
		if (temp == NULL) {
			goto exit;
		}
		int current_cell = *(int *)temp;
		visited[current_cell] = true;
		if (current_cell == end_cell) {
			break;
		}
		int neighbours[4] = {-width, 1, width, -1};
		for (int i = 0; i < 4; ++i) {
			int neighbour = current_cell + neighbours[i];
			if (__is_valid_cell(neighbour, height, width) && maze[neighbour] == 255 && visited[neighbour] == false) {
				result = array_list_add(st, (void *)&(neighbour));
				if (result == -1) {
					goto exit;
				}					
				goto found_neighbour;
			}
		}
		/* Goes here if it didn't find a valid neightbour */
		result = array_list_remove(st, st->length - 1);
		if (result == -1) {
			goto exit;
		}
found_neighbour:;
	}
	*path = __array_list_to_array(st);
	int length = st->length;
exit:
	array_list_free(st);
	free(visited);
	return length;
}


/*static int __maze_solve_dfs(uint8_t *maze, int height, int width, bool *visited, int current_cell, int end_cell) {
	if (visited[current_cell] == 1) {
		return 1;
	}
	if (current_cell == end_cell) {
		printf("Found it!!!\n");
		return 1;
	}
	visited[current_cell] = true;
	int neighbours[4] = {-width, 1, width, -1};
	for (int i = 0; i < 4; ++i) {
		if (maze[current_cell + neighbours[i]] == 255) {
			__maze_solve_dfs(maze, height, width, visited, current_cell + neighbours[i], end_cell);
		}
	}
	return 1;
}*/

static bool __is_valid_cell(int current_cell, int height, int width) {
	if (current_cell > 0 && current_cell < height * width) {
		return true;
	}
	return false;
}

static int *__array_list_to_array(struct array_list *array_l) {
	int *array = malloc(sizeof(int) * array_l->length);
	for (int i = 0; i < (int)(array_l->length); ++i) {
		array[i] = ((int *)(array_l->data))[i];
	}
	return array;
}
