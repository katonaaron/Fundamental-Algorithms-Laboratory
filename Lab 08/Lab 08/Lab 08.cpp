#include <iostream>
#include <map>

using namespace std;

template <class T>
class DisjointSet
{
    class Node {
        Node* parent;
        int rank;
        T data;
    };

    map<T, Node*> objectsToNodes_;

public:
    DisjointSet();
    ~DisjointSet();
    void MakeSet(T element);
    T FindSet(T element);
    void Union(T x, T y);
private:
    Node* FindSet(Node* node);
    void Link(Node* node1, Node* node2);
};

class Graph {

};

int main()
{
    return 0;
}