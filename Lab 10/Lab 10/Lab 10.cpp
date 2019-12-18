#include <iostream>
#include <vector>
#include <utility>
#include "graph.h"

using namespace std;

void DemoDFS()
{
    cout << "DFS demo:\n\n";

    const int nrVertices = 8;
    const vector<pair<int, int>> edges = {
        {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {2, 4}, {3, 0}, {4, 3}, {4, 5}, {5, 3}, {6, 0}, {6, 7} // G1
    };

    Graph graph(nrVertices);

    for (auto& edge : edges)
    {
        graph.AddEdge(edge.first, edge.second);
    }

    graph.PrintAdjacencyList();
    graph.DFS();
    graph.Dump();
    cout << "\n\n";
}

void TopologicalSortDemo()
{
    cout << "Topological sort demo:\n\n";

    const int nrVertices = 9;
    const vector<pair<int, int>> edges = {
        {0, 1}, {0, 6}, {1, 2}, {1, 6}, {2, 5}, {3, 2}, {3, 4}, {4, 5}, {7, 6} // DAG
    };

    Graph graph(nrVertices);

    for (auto& edge : edges)
    {
        graph.AddEdge(edge.first, edge.second);
    }

    graph.PrintAdjacencyList();
    std::list<unsigned> result = graph.TopologicalSort();

    cout << "Result:";
    for (unsigned& vertex : result)
    {
        cout << " " << vertex;
    }
    cout << "\n\n";
}

void StronglyConnectedComponentsDemo() {
    cout << "Strongly connected components demo:\n\n";

    const int nrVertices = 9;
    const vector<pair<int, int>> edges = {
        //{0, 1}, {0, 6}, {1, 2}, {1, 6}, {2, 5}, {3, 2}, {3, 4}, {4, 5}, {7, 6} //DAG
        //{0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {2, 4}, {3, 0}, {4, 3}, {4, 5}, {5, 3}, {6, 0}, {6, 7} //G1
        {0, 1}, {1, 2}, {1, 4}, {1, 5}, {2, 3}, {2, 6}, {3, 2}, {3, 7}, {4, 0}, {4, 5}, {5, 6}, {6, 5}, {6, 7}, {7, 7} //G2
    };

    Graph graph(nrVertices);

    for (auto& edge : edges)
    {
        graph.AddEdge(edge.first, edge.second);
    }

    graph.PrintAdjacencyList();
    std::vector<std::vector<unsigned>> components = graph.StronglyConnectedComponents();

    cout << "Strongly connected components:\n";
    for (size_t i = 0; i < components.size(); i++)
    {
        cout << i << ":";
        for (unsigned& key : components.at(i))
        {
            cout << " " << key;
        }
        cout << "\n";
    }
    cout << "\n\n";
}

int main()
{
    DemoDFS();
    TopologicalSortDemo();
    StronglyConnectedComponentsDemo();
    return 0;
}