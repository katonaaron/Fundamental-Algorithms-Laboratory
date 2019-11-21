/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *	Implement correctly and efficiently the management operations of an order statistics tree. A balanced, augmented Binary Search Tree
 *  has to be used. Each node in the tree holds, besides the necessary information, also the size field (i.e. the size of the sub-tree
 *  rooted at the node).
 *
 * Efficiency:
 *  BUILD_TREE:
 *      A post-order traversal is used to create a perfectly balanced tree with keys from the range 1...n. It's a divide and conquer
 *      algorithm. By applying the Master theorem for a = 2, b = 2, c = 0, we obtain O(n) running time. And the resulting binary
 *      search tree will be perfectly balanced, thus its height is log n, where n = number of nodes in the tree.
 *  
 *  OS_SELECT:
 *      The algorithm is similar to QuickSelect: We partition the input data in two parts, separated by a single element "q". If the size of
 *      the left partition + 1 = r is equal to the required index "i", we return q as the ith element, if it's smaller, we search i in the left
 *      partition, if it's greater we search i-r in the right partition.
 *      The major improvement over QuickSelect is, that the latter performs the partitioning procedure in O(n) time, while OS_SELECT does
 *      only O(1) operations at each level. The partitioning is done by selecting the left or the right child of the current root. The tree
 *      is balanced so it splits the input data in two at each level. Thus by applying the Master theorem for a = 1, b = 2, c = 0, we obtain 
 *      O( log n) running time.
 *
 *  OS_DELETE:
 *      
 */

#include <iostream>
#include <cassert>
#include <time.h>
#include "Profiler.h"

using namespace std;

#define DEMO

class OSTree
{
    struct Node
    {
        int data;
        Node* left;
        Node* right;
        Node* parent;
        size_t size;

        Node(int data);
        ~Node();
    };

    Node* root;

    OSTree();
    static Node* BuildPBTFromRange(int range_min, int range_max, Operation op);

    Node* FindMin(Node* node, Operation op);
    Node* Select(Node* root, int i, Operation op);
    Node* Delete(Node* node, Operation op);
    void DecrementSizeOfAncestors(Node* node, Operation op);

    //Demo
    void Print(Node* node, int indent);
public:
    ~OSTree();
    static OSTree* Build(int n, Operation op);
    int Select(int i, Operation op);
    void Delete(int i, Operation op);
    size_t Size();

    //Demo
    void Print();
};

OSTree::OSTree() : root(nullptr)
{
}

OSTree* OSTree::Build(int n, Operation op)
{
    if (n < 1)
        throw new invalid_argument("n must be >= 1");
    OSTree* os = new OSTree();
    os->root = BuildPBTFromRange(1, n, op);
    op.count(2);
    return os;
}

int OSTree::Select(int i, Operation op)
{
    if (root == nullptr || i < 1 || (unsigned int)i > root->size)
        throw new invalid_argument("i must be between 1 and the size of the tree");
    Node* node = Select(root, i, op);
    assert(node != nullptr);
    return node->data;
}

void OSTree::Delete(int i, Operation op)
{
    Node* node = Select(root, i, op);
    node = Delete(node, op);
    DecrementSizeOfAncestors(node, op);
    node->left = node->right = nullptr;
    delete node;
}

size_t OSTree::Size()
{
    return root == nullptr ? 0 : root->size;
}

OSTree::~OSTree()
{
    if (root != nullptr)
        delete root;
}

void OSTree::Print()
{
    cout << "Tree:\n";
    Print(root, 0);
    cout << "\n";
}

OSTree::Node* OSTree::BuildPBTFromRange(int range_min, int range_max, Operation op)
{
    op.count();
    if (range_min <= range_max)
    {
        int data = (range_min + range_max) / 2;
        Node* node = new Node(data);
        node->left = BuildPBTFromRange(range_min, data - 1, op);
        node->right = BuildPBTFromRange(data + 1, range_max, op);

        if (node->left != nullptr)
        {
            node->size += node->left->size;
            node->left->parent = node;
            op.count(2);
        }
        if (node->right != nullptr)
        {
            node->size += node->right->size;
            node->right->parent = node;
            op.count(2);
        }

        op.count(6);
        return node;
    }
    return nullptr;
}

OSTree::Node* OSTree::FindMin(Node* node, Operation op)
{
    op.count();
    if (node == nullptr)
        return nullptr;

    while (node->left != nullptr)
    {
        node = node->left;
        op.count();
    }
    op.count();
    return node;
}

OSTree::Node* OSTree::Select(OSTree::Node* root, int i, Operation op)
{
    op.count();
    if (root == nullptr)
        return nullptr;

    int r = 1;
    op.count(2);
    if (root->left != nullptr)
    {
        r += root->left->size;
        op.count();
    }

    op.count();
    if (i == r)
    {
        return root;
    }
    else if (i < r)
    {
        op.count();
        return Select(root->left, i, op);
    }
    op.count();
    return Select(root->right, i - r, op);
}

OSTree::Node* OSTree::Delete(Node* node, Operation op)
{
    op.count();
    if (node->left != nullptr && node->right != nullptr)
    {
        Node* successor = FindMin(node->right, op);
        swap(node->data, successor->data);
        node = successor;
        op.count(5);
    }

    Node* child;
    if (node->left != nullptr)
        child = node->left, op.count(2);
    else
        child = node->right, op.count();

    op.count();
    if (child != nullptr)
        child->parent = node->parent, op.count();;

    op.count();
    if (node->parent == nullptr)
    {
        root = child;
        op.count();
    }
    else
    {
        op.count(2);
        if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
    }
    return node;
}

void OSTree::DecrementSizeOfAncestors(Node* node, Operation op)
{
    op.count();
    if (node == nullptr)
        return;
    while (node->parent != nullptr)
    {
        node = node->parent;
        node->size--;
        op.count(3);
    }
    op.count();
}

void OSTree::Print(Node* node, int indent)
{
    cout << string(indent, ' ');
    if (node == nullptr)
    {
        cout << "null\n";
    }
    else {
        cout << node->data << "\n";
        Print(node->left, indent + 4);
        Print(node->right, indent + 4);
    }
}

OSTree::Node::Node(int data) : data(data), left(nullptr), right(nullptr), parent(nullptr), size(1)
{
}

OSTree::Node::~Node()
{
    if (left != nullptr)
        delete left;
    if (right != nullptr)
        delete right;
}

void Demo()
{
    const int size = 11;
    const int nrOfSearches = 3;

    Profiler profiler("Demo");
    Operation op = profiler.createOperation("dummy", 0);

    OSTree* os = OSTree::Build(size, op);

    os->Print();

    for (int i = 0; i < nrOfSearches; i++)
    {
        int index = rand() % (os->Size() - 1) + 1;
        cout << "Searching for \"" << index << "\". Result: \"" << os->Select(index, op) << "\"\n";
        os->Delete(index, op);
        os->Print();
    }

    delete os;
}

void Evaluate()
{
    const int range_min = 100;
    const int range_max = 10000;
    const int step_size = 100;
    const int nr_of_measurements = 5;

    Profiler profiler("Dynamic-Order-Statistics");

    for (int m = 1; m <= nr_of_measurements; m++) {
        for (int n = range_min; n <= range_max; n += step_size)
        {
            cout << "measurement: " << m << " size: " << n << "\n";
            Operation opBuild = profiler.createOperation("BUILD_TREE", n);
            Operation opSelect = profiler.createOperation("OS_SELECT", n);
            Operation opDelete = profiler.createOperation("OS_DELETE", n);

            OSTree* os = OSTree::Build(n, opBuild);

            for (int i = 0; i < n; i++)
            {
                int index = os->Size() == 1 ? 1 : rand() % (os->Size() - 1) + 1;
                os->Select(index, opSelect);
                os->Delete(index, opDelete);
            }
        }
    }
    profiler.divideValues("BUILD_TREE", nr_of_measurements);
    profiler.divideValues("OS_SELECT", nr_of_measurements);
    profiler.divideValues("OS_DELETE", nr_of_measurements);
    profiler.createGroup("ALL_OPERATIONS", "BUILD_TREE", "OS_SELECT", "OS_DELETE");

    profiler.addSeries("SELECT_AND_DELETE", "OS_SELECT", "OS_DELETE");
    profiler.addSeries("TOTAL", "SELECT_AND_DELETE", "BUILD_TREE");

    profiler.showReport();
}

int main()
{
    srand(time(NULL));
#ifdef DEMO
    Demo();
#else
    Evaluate();
#endif // DEMO
    return 0;
}