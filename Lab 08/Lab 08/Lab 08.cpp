#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <time.h>
#include "Profiler.h"

using namespace std;

#define DEMO

template <class T>
class DisjointSet
{
    struct Node {
        T parent;
        int rank;
    };

    unordered_map<T, Node> objectsToNodes_;

public:
    DisjointSet();
    void MakeSet(const T& element, Operation op);
    const T& FindSet(const T& element, Operation op);
    void Union(const T& x, const T& y, Operation op);

    //Demo
    void Dump();
private:
    void Link(const T& x, const T& y, Operation op);
};

class Graph {
    struct Edge {
        int x;
        int y;
        int weight;
        bool operator < (const Edge& e) const
        {
            if (weight == e.weight)
            {
                if (x == e.x)
                    return y < e.y;
                return x < e.x;
            }
            return weight < e.weight;
        }
    };
    unordered_set<int> V;
    vector<Edge> E;
public:
    Graph();
    void AddEdge(int x, int y, int w);
    Graph MSTKruskal(Operation opMake, Operation opFind, Operation opUnion);

    //Demo
    void Dump();
    //Evaluation
    static Graph GenerateConnectedGraph(int v, int e);
};

void DemoDisjointSet()
{
    cout << "Disjoint set demo\n";
    Profiler profiler("DemoDisjointSet");
    Operation op = profiler.createOperation("dummy", 0);

    DisjointSet<int> ds;
    const vector<int> keys = { 1, 2, 3, 4, 5 ,6 ,7 ,8 ,9 ,10 };
    const vector<pair<int, int>> unions = { {1, 2}, {1, 3}, {2, 4}, {8, 9}, {1, 9} };


    for (const int& key : keys)
    {
        ds.MakeSet(key, op);
    }
    for (const pair<int, int>& p : unions)
    {
        cout << "union of " << p.first << " and " << p.second << "\n";
        ds.Union(p.first, p.second, op);
        for (const int& key : keys)
        {
            cout << key << "->" << ds.FindSet(key, op) << " ";
        }
        cout << "\n";
    }

    ds.Dump();
    cout << "\n";
}

void DemoKruskal()
{
    cout << "Kruskal demo\n";
    Profiler profiler("DemoKruskal");
    Operation op = profiler.createOperation("dummy", 0);

    Graph graph;
    graph.AddEdge(1, 2, 1);
    graph.AddEdge(1, 5, 1);
    graph.AddEdge(2, 3, 2);
    graph.AddEdge(2, 4, 4);
    graph.AddEdge(2, 5, 2);
    graph.AddEdge(3, 4, 2);
    graph.AddEdge(4, 5, 5);

    cout << "Initial graph:\n\t";
    graph.Dump();
    Graph mst = graph.MSTKruskal(op, op, op);
    cout << "MST:\n\t";
    mst.Dump();
    cout << "\n";
}

void Evaluate()
{
    const int range_min = 100;
    const int range_max = 10000;
    const int step_size = 100;
    const int nr_of_measurements = 5;

    Profiler profiler("DisjointSet_Kruskal");

    for (int n = range_min; n <= range_max; n += step_size)
    {
        cout << "size: " << n << '\n';

        Operation opMake = profiler.createOperation("MAKE_SET", n);
        Operation opFind = profiler.createOperation("FIND_SET", n);
        Operation opUnion = profiler.createOperation("UNION", n);

        cout << "\tGenerating graph\n";
        Graph graph = Graph::GenerateConnectedGraph(n, n * 4);
        cout << "\tKruskal\n";
        Graph mst = graph.MSTKruskal(opMake, opFind, opUnion);
    }
    profiler.addSeries("FIND_UNION", "FIND_SET", "UNION");
    profiler.addSeries("TOTAL", "FIND_UNION", "MAKE_SET");
    profiler.createGroup("TOTAL", "MAKE_SET", "FIND_SET", "UNION", "TOTAL");

    profiler.showReport();
}

int main()
{
    srand((unsigned int)time(NULL));
#ifdef DEMO
    DemoDisjointSet();
    DemoKruskal();
#else
    Evaluate();
#endif // DEMO
    return 0;
}

template<class T>
DisjointSet<T>::DisjointSet()
{
}

template<class T>
void DisjointSet<T>::MakeSet(const T& element, Operation op)
{
    objectsToNodes_.insert({ element, {element, 0} });
    op.count(3);
}

template<class T>
const T& DisjointSet<T>::FindSet(const T& element, Operation op)
{
    Node& node = objectsToNodes_.at(element);
    op.count(2);
    if (node.parent != element)
        node.parent = FindSet(node.parent, op), op.count();
    return node.parent;
}

template<class T>
void DisjointSet<T>::Union(const T& x, const T& y, Operation op)
{
    Link(FindSet(x, op), FindSet(y, op), op);
}

template<class T>
void DisjointSet<T>::Dump()
{
    cout << "Key:\t";
    for (const pair<T, Node>& element : objectsToNodes_)
    {
        cout << element.first << "\t";
    }
    cout << "\nParent:\t";
    for (const pair<T, Node>& element : objectsToNodes_)
    {
        cout << element.second.parent << "\t";
    }
    cout << "\nRank:\t";
    for (const pair<T, Node>& element : objectsToNodes_)
    {
        cout << element.second.rank << "\t";
    }
    cout << "\n";
}

template<class T>
void DisjointSet<T>::Link(const T& x, const T& y, Operation op)
{
    op.count();
    if (x == y)
        return;
    Node& nodeX = objectsToNodes_.at(x);
    Node& nodeY = objectsToNodes_.at(y);
    op.count(3);
    if (nodeX.rank < nodeY.rank)
    {
        nodeX.parent = y;
        op.count();
    }
    else
    {
        nodeY.parent = x;
        op.count(2);
        if (nodeX.rank == nodeY.rank)
        {
            op.count();
            nodeX.rank++;
        }
    }

}

Graph::Graph()
{
}

void Graph::AddEdge(int x, int y, int w)
{
    V.insert(x);
    V.insert(y);
    E.push_back({ x, y, w });
}

Graph Graph::MSTKruskal(Operation opMake, Operation opFind, Operation opUnion)
{
    DisjointSet<int> ds;
    Graph mst;
    for (const int& vertex : V)
    {
        ds.MakeSet(vertex, opMake);
    }
    sort(E.begin(), E.end());
    for (const Edge& edge : E)
    {
        if (ds.FindSet(edge.x, opFind) != ds.FindSet(edge.y, opFind))
        {
            ds.Union(edge.x, edge.y, opUnion);
            mst.AddEdge(edge.x, edge.y, edge.weight);
        }
    }
    return mst;
}

void Graph::Dump()
{
    cout << "Edges: ";
    for (const Edge& edge : E)
    {
        cout << "(" << edge.x << "," << edge.y << ",w=" << edge.weight << ") ";
    }
    cout << "\n";
}

Graph Graph::GenerateConnectedGraph(int v, int e)
{
    if (e < v - 1 || e > v* (v - 1) / 2)
    {
        throw new invalid_argument("no connected undirected simple graph can be made");
    }

    Graph graph;
    set<pair<int, int>> edges;
    int x, y;

    for (int y = 2; y <= v; y++)
    {
        x = (y == 2) ? 1 : rand() % (y - 2) + 1;
        graph.AddEdge(x, y, rand());
        edges.insert({ x, y });
        edges.insert({ y, x });
    }

    for (int i = v - 1; i < e; i++)
    {
        do {
            x = rand() % (v - 1) + 1;
            do
            {
                y = rand() % (v - 1) + 1;
            } while (x == y);
        } while (edges.count({ x, y }) != 0);

        graph.AddEdge(x, y, rand());
        edges.insert({ x, y });
        edges.insert({ y, x });
    }

    return graph;
}
