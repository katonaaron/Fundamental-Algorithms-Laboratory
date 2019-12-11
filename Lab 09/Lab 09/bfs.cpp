#include <stdlib.h>
#include <string.h>
#include <queue>
#include <set>
#include "bfs.h"
#include "multiway_tree.h"
#include <cassert>

static inline bool isSafe(const Grid* grid, int col, int row)
{
    if (grid == NULL)
        return false;

    return col >= 0 && row >= 0 && col < grid->cols && row < grid->rows
        && grid->mat[row][col] == 0;
}

int get_neighbors(const Grid* grid, Point p, Point neighb[])
{
    if (grid == NULL)
        return 0;

    const int dir[][2] = { {0, 1}, {0,-1}, {1, 0}, {-1, 0} };
    const int dir_size = sizeof(dir) / sizeof(dir[0]);

    int size = 0;
    int row;
    int col;

    for (int i = 0; i < dir_size; i++)
    {
        col = p.col + dir[i][0];
        row = p.row + dir[i][1];
        if (isSafe(grid, col, row))
        {
            neighb[size].col = col;
            neighb[size].row = row;
            size++;
        }
    }

    // TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
    // the point p will have at most 4 neighbors (up, down, left, right)
    // avoid the neighbors that are outside the grid limits or fall into a wall
    // note: the size of the array neighb is guaranteed to be at least 4

    return size;
}

void grid_to_graph(const Grid* grid, Graph* graph)
{
    //we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
    Node* nodes[MAX_ROWS][MAX_COLS];
    int i, j, k;
    Point neighb[4];

    //compute how many nodes we have and allocate each node
    graph->nrNodes = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (grid->mat[i][j] == 0) {
                nodes[i][j] = (Node*)malloc(sizeof(Node));
                memset(nodes[i][j], 0, sizeof(Node)); //initialize all fields with 0/NULL
                nodes[i][j]->position.row = i;
                nodes[i][j]->position.col = j;
                ++graph->nrNodes;
            }
            else {
                nodes[i][j] = NULL;
            }
        }
    }
    graph->v = (Node**)malloc(graph->nrNodes * sizeof(Node*));
    k = 0;
    for (i = 0; i < grid->rows; ++i) {
        for (j = 0; j < grid->cols; ++j) {
            if (nodes[i][j] != NULL) {
                graph->v[k++] = nodes[i][j];
            }
        }
    }

    //compute the adjacency list for each node
    for (i = 0; i < graph->nrNodes; ++i) {
        graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
        if (graph->v[i]->adjSize != 0) {
            graph->v[i]->adj = (Node**)malloc(graph->v[i]->adjSize * sizeof(Node*));
            k = 0;
            for (j = 0; j < graph->v[i]->adjSize; ++j) {
                if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
                    neighb[j].col >= 0 && neighb[j].col < grid->cols &&
                    grid->mat[neighb[j].row][neighb[j].col] == 0) {
                    graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
                }
            }
            if (k < graph->v[i]->adjSize) {
                //get_neighbors returned some invalid neighbors
                graph->v[i]->adjSize = k;
                graph->v[i]->adj = (Node**)realloc(graph->v[i]->adj, k * sizeof(Node*));
            }
        }
    }
}

void free_graph(Graph* graph)
{
    if (graph->v != NULL) {
        for (int i = 0; i < graph->nrNodes; ++i) {
            if (graph->v[i] != NULL) {
                if (graph->v[i]->adj != NULL) {
                    free(graph->v[i]->adj);
                    graph->v[i]->adj = NULL;
                }
                graph->v[i]->adjSize = 0;
                free(graph->v[i]);
                graph->v[i] = NULL;
            }
        }
        free(graph->v);
        graph->v = NULL;
    }
    graph->nrNodes = 0;
}

void bfs(Graph* graph, Node* s, Operation* op)
{
    // TOOD: implement the BFS algorithm on the graph, starting from the node s
    // at the end of the algorithm, every node reachable from s should have the color BLACK
    // for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
    // for counting the number of operations, the optional op parameter is received
    // since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
    // if(op != NULL) op->count();

    if (graph == NULL || s == NULL)
        return;
    for (int i = 0; i < graph->nrNodes; i++)
    {
        graph->v[i]->color = COLOR_WHITE;
        graph->v[i]->dist = INT_MAX;
        graph->v[i]->parent = NULL;
        if (op != NULL) op->count(3);
    }

    s->color = COLOR_GRAY;
    s->dist = 0;
    s->parent = NULL;
    if (op != NULL) op->count(3);


    std::queue<Node*> q;
    q.push(s);
    Node* u;
    Node* v;
    if (op != NULL) op->count(2);

    if (op != NULL) op->count();
    while (!q.empty())
    {
        u = q.front();
        if (op != NULL) op->count();


        for (int i = 0; i < u->adjSize; i++)
        {
            v = u->adj[i];
            if (op != NULL) op->count(2);

            if (v->color == COLOR_WHITE)
            {
                v->color = COLOR_GRAY;
                v->dist = u->dist + 1;
                v->parent = u;
                q.push(v);
                if (op != NULL) op->count(4);

            }
        }

        q.pop();
        u->color = COLOR_BLACK;
        if (op != NULL) op->count(3);
    }
    
}

void print_bfs_tree(Graph* graph)
{
    //first, we will represent the BFS tree as a parent array
    int n = 0; //the number of nodes
    int* p = NULL; //the parent array
    Point* repr = NULL; //the representation for each element in p

    //some of the nodes in graph->v may not have been reached by BFS
    //p and repr will contain only the reachable nodes
    int* transf = (int*)malloc(graph->nrNodes * sizeof(int));
    for (int i = 0; i < graph->nrNodes; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            transf[i] = n;
            ++n;
        }
        else {
            transf[i] = -1;
        }
    }
    if (n == 0) {
        //no BFS tree
        free(transf);
        return;
    }

    int err = 0;
    p = (int*)malloc(n * sizeof(int));
    repr = (Point*)malloc(n * sizeof(Node));
    for (int i = 0; i < graph->nrNodes && !err; ++i) {
        if (graph->v[i]->color == COLOR_BLACK) {
            if (transf[i] < 0 || transf[i] >= n) {
                err = 1;
            }
            else {
                repr[transf[i]] = graph->v[i]->position;
                if (graph->v[i]->parent == NULL) {
                    p[transf[i]] = -1;
                }
                else {
                    err = 1;
                    for (int j = 0; j < graph->nrNodes; ++j) {
                        if (graph->v[i]->parent == graph->v[j]) {
                            if (transf[j] >= 0 && transf[j] < n) {
                                p[transf[i]] = transf[j];
                                err = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    free(transf);
    transf = NULL;

    if (!err) {
        // TODO: pretty print the BFS tree
        // the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
        // when printing the node k, print repr[k] (it contains the row and column for that point)
        // you can adapt the code for transforming and printing multi-way trees from the previous labs

        MultiwayTree tree(p, n, repr);
        tree.Print();
    }

    if (p != NULL) {
        free(p);
        p = NULL;
    }
    if (repr != NULL) {
        free(repr);
        repr = NULL;
    }
}

int shortest_path(Graph* graph, Node* start, Node* end, Node* path[])
{
    // TODO: compute the shortest path between the nodes start and end in the given graph
    // the nodes from the path, should be filled, in order, in the array path
    // the number of nodes filled in the path array should be returned
    // if end is not reachable from start, return -1
    // note: the size of the array path is guaranteed to be at least 1000

    if (graph == NULL || start == NULL || end == NULL)
        throw new std::invalid_argument("The arguments must not be NULL");

    bfs(graph, start, NULL);

    if(end->color != COLOR_BLACK)
        return -1;

    Node* node = end;
    for (int i = end->dist; i >= 0; i--)
    {
        path[i] = node;
        node = node->parent;
    }
    return end->dist + 1;
}

void AddEdge(Graph* graph, int x, int y)
{
    if (graph == NULL || x < 0 || y < 0 || x >= graph->nrNodes || y >= graph->nrNodes)
        throw new std::invalid_argument("Graph is null or the coordinates are out of range");

    Node* nodeX = graph->v[x];
    Node* nodeY = graph->v[y];
    assert(nodeX != NULL && nodeY != NULL);

    nodeX->adj[nodeX->adjSize++] = nodeY;
    nodeY->adj[nodeY->adjSize++] = nodeX;
}

 void GenerateEdges(Graph* graph, int v, int e)
{
    if (graph == NULL)
        throw new std::invalid_argument("graph is NULL");
    if (e < v - 1 || e > v* (v - 1) / 2)
    {
        throw new std::invalid_argument("no connected undirected simple graph can be made");
    }

    for (int i = 0; i < graph->nrNodes; i++)
    {
        graph->v[i]->adj = (Node**)malloc((graph->nrNodes - 1) * sizeof(Node*));
        if (graph->v[i]->adj == NULL)
            throw new std::bad_alloc;
    }

    std::set<std::pair<int, int>> edges;
    int x, y;

    for (int y = 1; y < v; y++)
    {
        x = rand() % y;
        AddEdge(graph, x, y);
        edges.insert({ x, y });
        edges.insert({ y, x });
    }

    for (int i = v - 1; i < e; i++)
    {
        do {
            x = rand() % v;
            do
            {
                y = rand() % v;
            } while (x == y);
        } while (edges.count({ x, y }) != 0);

        AddEdge(graph, x, y);
        edges.insert({ x, y });
        edges.insert({ y, x });
    }
}

void performance()
{
    int n, i;
    Profiler p("bfs");

    // vary the number of edges
    for (n = 1000; n <= 4500; n += 100) {
        Operation op = p.createOperation("bfs-edges", n);
        Graph graph;
        graph.nrNodes = 100;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate n random edges
        // make sure the generated graph is connected
        GenerateEdges(&graph, graph.nrNodes, n);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    // vary the number of vertices
    for (n = 100; n <= 200; n += 10) {
        Operation op = p.createOperation("bfs-vertices", n);
        Graph graph;
        graph.nrNodes = n;
        //initialize the nodes of the graph
        graph.v = (Node**)malloc(graph.nrNodes * sizeof(Node*));
        for (i = 0; i < graph.nrNodes; ++i) {
            graph.v[i] = (Node*)malloc(sizeof(Node));
            memset(graph.v[i], 0, sizeof(Node));
        }
        // TODO: generate 4500 random edges
        // make sure the generated graph is connected
        GenerateEdges(&graph, graph.nrNodes, 4500);

        bfs(&graph, graph.v[0], &op);
        free_graph(&graph);
    }

    p.showReport();
}
