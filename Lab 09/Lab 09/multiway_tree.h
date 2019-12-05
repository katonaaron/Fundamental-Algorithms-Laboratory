#ifndef MULTIWAY_TREE_H
#define MULTIWAY_TREE_H

#include <iostream>
#include <vector>
#include "bfs.h"

class MultiwayTree
{
    struct Node
    {
        Point key;
        std::vector<Node*> children;

        Node(Point key);
        ~Node();
    };

    Node* root_;

    friend class BinaryMultiwayTree;
public:
    MultiwayTree();
    MultiwayTree(const int* parents, int parents_size, const Point* repr);
    ~MultiwayTree();
    void Print();

private:
    void Print(Node* node, int indent);
};

#endif // !MULTIWAY_TREE_H

