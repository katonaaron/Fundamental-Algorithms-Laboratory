#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;

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
    void MakeSet(const T& element);
    const T& FindSet(const T& element);
    void Union(const T& x, const T& y);

    //Demo
    void Dump();
private:
    void Link(const T& x, const T& y);
};

class Graph {
    struct Edge {
        int x;
        int y;
        int weight;
        bool operator < (const Edge& e) const
        {
            return weight < e.weight;
        }
    };
    unordered_set<int> V;
    vector<Edge> E;
public:
    Graph();
    void AddEdge(int x, int y, int w);
    Graph MSTKruskal();

    //Demo
    void Dump();
};

void DemoDisjointSet()
{
    cout << "Disjoint set demo\n";
    DisjointSet<int> ds;
    const vector<int> keys = { 1, 2, 3, 4, 5 ,6 ,7 ,8 ,9 ,10 };
    const vector<pair<int, int>> unions = { {1, 2}, {1, 3}, {2, 4}, {8, 9}, {1, 9} };
    for (const int& key : keys)
    {
        ds.MakeSet(key);
    }
    for (const pair<int, int>& p : unions)
    {
        cout << "union of " << p.first << " and " << p.second << "\n";
        ds.Union(p.first, p.second);
        for (const int& key : keys)
        {
            cout << key << "->" << ds.FindSet(key) << " ";
        }
        cout << "\n";
    }

    ds.Dump();
    cout << "\n";
}

void DemoKruskal()
{
    cout << "Kruskal demo\n";
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
    Graph mst = graph.MSTKruskal();
    cout << "MST:\n\t";
    mst.Dump();
    cout << "\n";
}

int main()
{

    DemoDisjointSet();
    DemoKruskal();
    return 0;
}

template<class T>
DisjointSet<T>::DisjointSet()
{
}

template<class T>
void DisjointSet<T>::MakeSet(const T& element)
{
    objectsToNodes_.insert({ element, {element, 0} });
}

template<class T>
const T& DisjointSet<T>::FindSet(const T& element)
{
    Node& node = objectsToNodes_.at(element);
    if (node.parent != element)
        node.parent = FindSet(node.parent);
    return node.parent;
}

template<class T>
void DisjointSet<T>::Union(const T& x, const T& y)
{
    Link(FindSet(x), FindSet(y));
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
void DisjointSet<T>::Link(const T& x, const T& y)
{
    if (x == y)
        return;
    Node& nodeX = objectsToNodes_.at(x);
    Node& nodeY = objectsToNodes_.at(y);
    if (nodeX.rank < nodeY.rank)
    {
        nodeX.parent = y;
    }
    else
    {
        nodeY.parent = x;
        if (nodeX.rank == nodeY.rank)
        {
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

Graph Graph::MSTKruskal()
{
    DisjointSet<int> ds;
    Graph mst;
    for (const int& vertex : V)
    {
        ds.MakeSet(vertex);
    }
    sort(E.begin(), E.end());
    for (const Edge& edge : E)
    {
        if (ds.FindSet(edge.x) != ds.FindSet(edge.y))
        {
            ds.Union(edge.x, edge.y);
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
