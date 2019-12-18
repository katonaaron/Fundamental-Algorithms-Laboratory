#include <iostream>
#include <vector>
#include <utility>
#include "graph.h"

using namespace std;

void DemoDFS()
{
    const int nrVertices = 8;
    const vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {2, 4}, {3, 0}, {4, 3}, {4, 5}, {5, 3}, {6, 0}, {6, 7}
    };

    Graph graph(nrVertices);

    for (auto& edge : edges)
    {
        graph.AddEdge(edge.first, edge.second);
    }

    graph.DFS();
    graph.Dump();
}

int main()
{
    DemoDFS();
}