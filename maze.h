#ifndef _MAZE_H_
#define _MAZE_H_

#include <stdint.h>

int maze_solve_dfs(uint8_t *maze, int height, int width, int **path);

#endif