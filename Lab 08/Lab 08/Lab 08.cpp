#include <iostream>
#include <unordered_map>

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
private:
    void Link(const T& x, const T& y);
};

class Graph {

};

void Demo()
{
    DisjointSet<int> ds;
    for (int i = 1; i <= 10; i++)
    {
        ds.MakeSet(i);
    }
    ds.Union(1, 2);
    ds.Union(1, 3);
    for (int i = 1; i <= 10; i++)
    {
        cout << i << "->" << ds.FindSet(i) << " ";
    }
    cout << "\n";
}

int main()
{

    Demo();

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
void DisjointSet<T>::Link(const T& x, const T& y)
{
    if (x == y)
        return;
    Node& nodeX = objectsToNodes_.at(x);
    Node& nodeY = objectsToNodes_.at(y);
    if (nodeX.rank > nodeY.rank)
    {
        nodeX.parent = y;
    }
    else
    {
        nodeY.parent = x;
        if (nodeX.rank == nodeY.rank)
        {
            nodeY.rank++;
        }
    }

}
