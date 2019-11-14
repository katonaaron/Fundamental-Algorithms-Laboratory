#include <iostream>
#include <cassert>
#include <time.h>

using namespace std;

class OSTree
{
    class Node
    {
    public:
        int key;
        Node* left;
        Node* right;
        size_t size;

        Node(int key);
    };

    Node* root;

    OSTree();
public:
    static OSTree* BUILD_TREE(int n);
    int OS_SELECT(int i);
    ~OSTree();
    //Demo
    void Print();
private:
    static Node* BuildTreeRec(int minVal, int maxVal);
    Node* OS_SELECT(OSTree::Node* root, int i);
    void DeleteTree(Node* root);
    //Demo
    void PrintRec(Node* node, int indent);
};

OSTree::OSTree()
{
    root = nullptr;
}

OSTree* OSTree::BUILD_TREE(int n)
{
    if (n < 1)
        throw new invalid_argument("n must be >= 1");
    OSTree* os = new OSTree();
    os->root = BuildTreeRec(1, n);
    return os;
}

int OSTree::OS_SELECT(int i)
{
    if (root == nullptr || i < 1 || (unsigned int)i > root->size)
        throw new invalid_argument("i must be between 1 and the size of the tree");
    Node* node = OS_SELECT(root, i);
    assert(node != nullptr);
    return node->key;
}

OSTree::~OSTree()
{
    DeleteTree(root);
}

void OSTree::Print()
{
    cout << "Tree:\n";
    PrintRec(root, 0);
    cout << "\n";
}

OSTree::Node* OSTree::BuildTreeRec(int minVal, int maxVal)
{
    if (minVal <= maxVal)
    {
        int key = (minVal + maxVal) / 2;
        Node* node = new Node(key);
        node->left = BuildTreeRec(minVal, key - 1);
        node->right = BuildTreeRec(key + 1, maxVal);
        if (node->left != nullptr)
            node->size += node->left->size;
        if (node->right != nullptr)
            node->size += node->right->size;
        return node;
    }
    return nullptr;
}

OSTree::Node* OSTree::OS_SELECT(OSTree::Node* root, int i)
{
    if (root == nullptr)
        return nullptr;

    int r = 1;
    if (root->left != nullptr)
    {
        r += root->left->size;
    }

    if (i == r)
    {
        return root;
    }
    else if (i < r)
    {
        return OS_SELECT(root->left, i);
    }
    return OS_SELECT(root->right, i - r);
}

void OSTree::DeleteTree(Node* root)
{
    if (root == nullptr)
        return;
    DeleteTree(root->left);
    DeleteTree(root->right);
    delete root;
}

void OSTree::PrintRec(Node* node, int indent)
{
    cout << string(indent, ' ');
    if (node == nullptr)
    {
        cout << "null\n";
    }
    else {
        cout << node->key << "\n";
        PrintRec(node->left, indent + 4);
        PrintRec(node->right, indent + 4);
    }
}

OSTree::Node::Node(int key)
{
    this->key = key;
    this->left = this->right = nullptr;
    this->size = 1;
}

void Demo()
{
    const int size = 11;
    const int nrOfSearches = 3;

    OSTree* os = OSTree::BUILD_TREE(size);
    os->Print();

    for (int i = 0; i < nrOfSearches; i++)
    {
        int index = rand() % (size - 1) + 1;
        cout << "Searching for \"" << index << "\". Result: \"" << os->OS_SELECT(index) << "\"\n";
    }

    delete os;
}

int main()
{
    srand(time(NULL));
    Demo();
    return 0;
}