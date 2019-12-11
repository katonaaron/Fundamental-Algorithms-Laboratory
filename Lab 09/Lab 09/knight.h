#ifndef KNIGHT_H_INCLUDED
#define KNIGHT_H_INCLUDED

#include "bfs.h"

void knight_grid_to_graph(const Grid* grid, Graph* graph);
int knight_get_neighbors(const Grid* grid, Point p, Point neighb[]);

#endif // !KNIGHT_H_INCLUDED

