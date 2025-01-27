/**
 * @author Katona Aron
 * @group 30423
 *
 *  Problem specification:
 *      Implement correctly and efficiently the base operations for disjoint set and the Kruskal's algorithm using disjoint sets.
 *
 *  Structure: 
 *      In order to map the generic keys to the nodes of the tree, a hash table is used, the unordered_map from STL. The
 *      nodes and their keys are stored in the hash table. A node points to its key in the hash table, to another node from
 *      the hash table, which is its parent, and has a rank field which is used to do union by rank.
 *  Denote n as the number of MAKE_SET operation calls, i.e. the number of nodes in the forest.
 *  MAKE_SET:
 *      Insertion to the hash table is O(1) in the average case. The node is initialized in O(1). Thus the total complexity 
 *      is O(1).
 *  Efficiency:
 *      Due to the usage of path compression and union by rank, the height of each tree will become much smaller. For m number
 *      of MAKE_SET, FIND_SET, and UNION operations the running time is O(m * alpha(n)). alpha(n) is the inverse Ackerman function,
 *      which is a very slowly growing function, having alpha(n) <= 4 for all practical purposes. Thus the running time of m
*       disjoint-set operations is very close to O(m).
 *  Additional space:
 *      O(n) additional space is used to store each key and the corresponding node in the hash table.
 *  Chart interpretation:    
 *      In the Kruskal algorithm, the number of MAKE_SET, FIND_SET, and UNION operations is (V + 2*E). Thus the running time
 *      of the disjoint set operations is O((V + E) * alpha(V)).
 *      In the evaluation E is the function of V, E = V * 4. Thus the running time  of the disjoint set operations is 
 *      O(V * alpha(V)).
 *      From the chart we can deduce, that alpha(V) is truly a small number, and the curves of the operations are close to linear.
 */
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
        const T* element;
        int rank;
        Node* parent;
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
    void Link(Node& x, Node& y, Operation op);
    Node& FindSet(Node& node, Operation op);
};

class Graph {
    struct Edge {
        int x;
        int y;
        int weight;
        bool operator < (const Edge& e) const { return weight < e.weight; }
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
    profiler.addSeries("MAKE_UNION", "MAKE_SET", "UNION");
    profiler.addSeries("TOTAL", "MAKE_UNION", "FIND_SET");
    profiler.createGroup("TOTAL", "MAKE_SET", "FIND_SET", "UNION", "TOTAL");

    profiler.addSeries("HALF_FIND_SET", "FIND_SET", "FIND_SET");
    profiler.divideValues("HALF_FIND_SET", 4);
    profiler.addSeries("TOTAL_FOR_HALF_FIND_SET", "MAKE_UNION", "HALF_FIND_SET");
    profiler.createGroup("TOTAL_FOR_HALF_FIND_SET", "MAKE_SET", "HALF_FIND_SET", "UNION", "TOTAL_FOR_HALF_FIND_SET");

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
    objectsToNodes_.insert({ element, {nullptr, 0, nullptr} });

    auto it = objectsToNodes_.find(element);
    it->second.parent = &it->second;
    it->second.element = &it->first;
    op.count(5);
}

template<class T>
const T& DisjointSet<T>::FindSet(const T& element, Operation op)
{
    Node& node = objectsToNodes_.at(element);
    op.count();
    return *FindSet(node, op).element;
}

template<class T>
void DisjointSet<T>::Union(const T& x, const T& y, Operation op)
{
    Node& nodeX = objectsToNodes_.at(x);
    Node& nodeY = objectsToNodes_.at(y);
    op.count(2);
    Link(FindSet(nodeX, op), FindSet(nodeY, op), op);
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
        cout << *element.second.parent->element << "\t";
    }
    cout << "\nRank:\t";
    for (const pair<T, Node>& element : objectsToNodes_)
    {
        cout << element.second.rank << "\t";
    }
    cout << "\n";
}

template<class T>
void DisjointSet<T>::Link(Node& x, Node& y, Operation op)
{
    op.count();
    if (&x == &y)
        return;

    op.count();
    if (x.rank < y.rank)
    {
        x.parent = &y;
        op.count();
    }
    else
    {
        y.parent = &x;
        op.count(2);
        if (x.rank == y.rank)
        {
            op.count();
            x.rank++;
        }
    }

}

template<class T>
typename DisjointSet<T>::Node& DisjointSet<T>::FindSet(Node& node, Operation op)
{
    op.count();
    if (node.parent != &node)
        node.parent = &FindSet(*node.parent, op), op.count();
    return *node.parent;
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
