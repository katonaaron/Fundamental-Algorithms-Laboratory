#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <time.h>

#include "Profiler.h"
#include "graph.h"

using namespace std;

#define DEMO true

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

Graph GenerateGraph(int v, int e)
{
    Graph graph(v);

    set<pair<int, int>> available_edges;

    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < v; j++)
        {
            available_edges.emplace(i, j);
        }
    }

    set<pair<int, int>>::iterator it;

    for (int i = 0; i < e; i++)
    {
        it = available_edges.begin();
        advance(it, rand() % available_edges.size());

        graph.AddEdge(it->first, it->second);
        available_edges.erase(*it);
    }
    return graph;
}

void Evaluate()
{
    const int v_fixed = 100;
    const int e_min = 1000;
    const int e_max = 5000;
    const int e_steps = 100;

    const int e_fixed = 9000;
    const int v_min = 100;
    const int v_max = 200;
    const int v_steps = 10;

    srand(time(0));
    Profiler profiler("DFS");

    for (int e = e_min; e <= e_max; e += e_steps)
    {
        cout << "Evaluation for v = " << v_fixed << ", e = " << e << "\n";
        Operation op = profiler.createOperation("v_fixed", e);

        Graph graph = GenerateGraph(v_fixed, e);
        graph.DFS(&op);
    }

    for (int v = v_min; v <= v_max; v += v_steps)
    {
        cout << "Evaluation for v = " << v << ", e = " << e_fixed << "\n";
        Operation op = profiler.createOperation("e_fixed", v);

        Graph graph = GenerateGraph(v, e_fixed);
        graph.DFS(&op);
    }

    profiler.showReport();
}

int main()
{
#if DEMO
    DemoDFS();
    TopologicalSortDemo();
    StronglyConnectedComponentsDemo();
#else
    Evaluate();
#endif // DEMO
    return 0;
}