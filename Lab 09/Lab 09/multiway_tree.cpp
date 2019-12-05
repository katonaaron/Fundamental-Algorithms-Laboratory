#include "multiway_tree.h"

MultiwayTree::Node::Node(Point key) : key(key)
{
}

MultiwayTree::Node::~Node()
{
    for (Node*& child : children)
    {
        delete child;
    }
}

MultiwayTree::MultiwayTree() : root_(nullptr)
{
}

MultiwayTree::MultiwayTree(const int* parents, int parents_size, const Point* repr)
{
    std::vector<Node*> nodes;
    for (int i = 0; i < parents_size; i++)
    {
        nodes.push_back(new Node(repr[i]));
    }

    root_ = nullptr;
    for (int i = 0; i < parents_size; i++)
    {
        if (parents[i] == -1)
            root_ = nodes.at(i);
        else
            nodes.at(parents[i])->children.push_back(nodes.at(i));
    }

    if (root_ == nullptr)
        throw new std::invalid_argument("There is no root in the tree");
}

MultiwayTree::~MultiwayTree()
{
    if (root_ != nullptr)
        delete root_;
}

void MultiwayTree::Print()
{
    Print(root_, 0);
}

void MultiwayTree::Print(Node* node, int indent)
{
    std::cout << std::string(indent, ' ');
    if (node != nullptr)
    {
        std::cout << "(" << node->key.col << "," << node->key.row << ")" << "\n";
        for (Node*& child : node->children)
        {
            Print(child, indent + 4);
        }
    }
}