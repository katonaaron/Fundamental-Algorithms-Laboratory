#include <iostream>
#include <cassert>
#include <time.h>

using namespace std;

class OSTree
{
    static class Node
    {
        int key;
        Node* left;
        Node* right;
        size_t size;

    public:

        Node(int key);
        ~Node();
        static Node* BuildPBTFromRange(int range_min, int range_max);

        Node* OS_SELECT(int i);
        void Delete();
        size_t Size();
        int GetKey();

        //Demo
        void Print(int indent);
    };

    Node* root;
    OSTree();
public:
    static OSTree* BUILD_TREE(int n);
    int OS_SELECT(int i);
    void OS_DELETE(int i);
    ~OSTree();

    //Demo
    void Print();
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
    os->root = Node::BuildPBTFromRange(1, n);
    return os;
}

int OSTree::OS_SELECT(int i)
{
    if (root == nullptr || i < 1 || (unsigned int)i > root->Size())
        throw new invalid_argument("i must be between 1 and the size of the tree");
    Node* node = root->OS_SELECT(i);
    assert(node != nullptr);
    return node->GetKey();
}

void OSTree::OS_DELETE(int i)
{
    Node* node = root->OS_SELECT(i);
    node->Delete();
}

OSTree::~OSTree()
{
    if (root != nullptr)
        delete root;
}

void OSTree::Print()
{
    cout << "Tree:\n";
    root->Print(0);
    cout << "\n";
}

OSTree::Node::Node(int key)
{
    this->key = key;
    this->left = this->right = nullptr;
    this->size = 1;
}

OSTree::Node::~Node()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}

OSTree::Node* OSTree::Node::BuildPBTFromRange(int range_min, int range_max)
{
    if (range_min <= range_max)
    {
        int key = (range_min + range_max) / 2;
        Node* node = new Node(key);
        node->left = BuildPBTFromRange(range_min, key - 1);
        node->right = BuildPBTFromRange(key + 1, range_max);
        if (node->left != nullptr)
            node->size += node->left->size;
        if (node->right != nullptr)
            node->size += node->right->size;
        return node;
    }
    return nullptr;
}

OSTree::Node* OSTree::Node::OS_SELECT(int i)
{
    int r = 1 + (left == nullptr ? 0 : left->size);

    if (i == r)
    {
        return this;
    }
    else if (i < r)
    {
        return left->OS_SELECT(i);
    }
    return right->OS_SELECT(i - r);
}

void OSTree::Node::Delete()
{
    if (left == nullptr || right == nullptr)
    {
        
    }
}

size_t OSTree::Node::Size()
{
    return this->size;
}

int OSTree::Node::GetKey()
{
    return this->key;
}

void OSTree::Node::Print(int indent)
{
    cout << string(indent, ' ') << key << "\n";

    indent += 4;
    if (left != nullptr)
        left->Print(indent);
    else
        cout << string(indent, ' ') << "null" << "\n";
    if (right != nullptr)
        right->Print(indent);
    else
        cout << string(indent, ' ') << "null" << "\n";
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