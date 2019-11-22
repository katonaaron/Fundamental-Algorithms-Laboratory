#include <iostream>
#include <vector>

using namespace std;

class MultiwayTree
{
    struct Node
    {
        int key;
        vector<Node*> children;

        Node(int key);
        ~Node();
    };

    Node* root_;

    friend class BinaryMultiwayTree;
public:
    MultiwayTree();
    MultiwayTree(vector<int> parents);
    ~MultiwayTree();
    void Print();

private:
    void Print(Node* node, int indent);
};

class BinaryMultiwayTree
{
    struct Node
    {
        int key;
        Node* left;
        Node* right;

        Node(int key);
        ~Node();
    };

    Node* root_;

public:
    BinaryMultiwayTree();
    BinaryMultiwayTree(MultiwayTree& mTree);
    void Print();

private:
    BinaryMultiwayTree::Node* ConvertMultiwayToBinary(MultiwayTree::Node* source, BinaryMultiwayTree::Node* sibling);
    void Print(Node* node, int indent);
};

void PrintVector(vector<int> vec, string message = "")
{
    cout << message;
    for (int& element : vec)
    {
        cout << element << " ";
    }
    cout << "\n\n";
}

int main()
{
    vector<int> parents = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    MultiwayTree mTree(parents);
    BinaryMultiwayTree bTree(mTree);

    PrintVector(parents, "Parent representation: ");
    mTree.Print();
    bTree.Print();
    return 0;
}

MultiwayTree::Node::Node(int key) : key(key)
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

MultiwayTree::MultiwayTree(vector<int> parents)
{
    vector<Node*> nodes;
    for (unsigned i = 1; i <= parents.size(); i++)
    {
        nodes.push_back(new Node(i));
    }

    root_ = nullptr;
    for (unsigned i = 0; i < parents.size(); i++)
    {
        if (parents.at(i) == -1)
            root_ = nodes.at(i);
        else
            nodes.at(parents.at(i) - 1)->children.push_back(nodes.at(i));
    }

    if (root_ == nullptr)
        throw new invalid_argument("There is no root in the tree");
}

MultiwayTree::~MultiwayTree()
{
    if (root_ != nullptr)
        delete root_;
}

void MultiwayTree::Print()
{
    cout << "Multi-way representation:\n";
    Print(root_, 0);
    cout << "\n";
}

void MultiwayTree::Print(Node* node, int indent)
{
    cout << string(indent, ' ');
    if (node == nullptr)
    {
        cout << "null\n";
    }
    else {
        cout << node->key << "\n";
        for (Node*& child : node->children)
        {
            Print(child, indent + 4);
        }
    }
}

BinaryMultiwayTree::BinaryMultiwayTree() : root_(nullptr)
{
}

BinaryMultiwayTree::BinaryMultiwayTree(MultiwayTree& mTree)
{
    root_ = nullptr;
    if (mTree.root_ == nullptr)
        return;

    root_ = ConvertMultiwayToBinary(mTree.root_, nullptr);
}

void BinaryMultiwayTree::Print()
{
    cout << "Binary tree representation:\n";
    Print(root_, 0);
    cout << "\n";
}

BinaryMultiwayTree::Node* BinaryMultiwayTree::ConvertMultiwayToBinary(MultiwayTree::Node* source, BinaryMultiwayTree::Node* sibling)
{
    if (source == nullptr)
        return nullptr;

    Node* node = new Node(source->key);
    node->right = sibling;

    if (source->children.empty())
        return node;

    Node* child = nullptr;

    for (int i = source->children.size() - 1; i >= 0; i--)
    {
        child = ConvertMultiwayToBinary(source->children.at(i), child);
    }

    node->left = child;
    return node;
}

void BinaryMultiwayTree::Print(Node* node, int indent)
{
    if (node != nullptr)
    {
        cout << string(indent, ' ') << node->key << "\n";
        Print(node->left, indent + 4);
        Print(node->right, indent);
    }
}

BinaryMultiwayTree::Node::Node(int key) : key(key), left(nullptr), right(nullptr)
{

}

BinaryMultiwayTree::Node::~Node()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}
