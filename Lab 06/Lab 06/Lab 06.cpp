/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *	Implement correctly and efficiently the management operations of an order statistics tree. A balanced, augmented Binary Search Tree
 *  has to be used. Each node in the tree holds, besides the necessary information, also the size field (i.e. the size of the sub-tree
 *  rooted at the node).
 *
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
 *      O( log n) running time. If the tree is unbalanced, then the running time will be O(h).
 *
 *  OS_DELETE:
 *      The OS_DELETE first searches the ith node by using OS_SELECT. The size of each visited node is decremented, to keep the OSTree property
 *      after the deletion. The node returned by OS_SELECT is deleted using the standard BST deletion, with the exception, that if the node 
 *      has two children, when we are searching for the inorder successor, the size of each node between the parent and the child is decremented.
 *      These constant, O(1) operations doesn't change the asymptotic running time of these algorithms.
 *      OS_SELECT is O(h) and BST deletion is also O(h). Thus the overall complexity is O(h). 
 *      We created a perfectly balanced tree, thus the initial height is log n, and after each deletion, the height of the root is smaller than or
 *      equal with the one before deletion. By deleting random nodes from the tree, the height of the tree will always be proportional with log n.
 *      If an AVL or Red-Black tree is used, the O(log n) running time will be guaranteed for any input data.
 *
 *  Chart interpretation:
 *      The chart shows that if the nodes to be searched and deleted are randomly selected, even if the tree is not self balancing, the expected
 *      average case running time of OS_SELECT and OS_DELETE is O(log n). These are called n times thus the overall complexity is O(n log n).
 *      We can see the difference between these O(n log n) operations and the linear BUILD_TREE.
 *      The curves of OS_SELECT and OS_DELETE are really close to each other, meaning that the deletion of the node (and searching for a successor)
 *      doesn't have much impact on the overall performance.
 *      We can also see the curve of the total operations (BUILD_TREE + OS_SELECT + OS_DELETE) which is O(n) + 2 * O(n log n) = O(n log n).
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

    Node* FindMin(Node* node, Operation op, bool decrement_size = false);
    Node* Select(Node* root, int i, Operation op, bool decrement_size = false);
    Node* Delete(Node* node, Operation op);

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
    Node* node = Select(root, i, op, true);
    node = Delete(node, op);
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

OSTree::Node* OSTree::FindMin(Node* node, Operation op, bool decrement_size)
{
    op.count();
    if (node == nullptr)
        return nullptr;

    while (node->left != nullptr)
    {
        if (decrement_size)
            node->size--;
        node = node->left;
        op.count();
    }
    op.count();
    return node;
}

OSTree::Node* OSTree::Select(OSTree::Node* root, int i, Operation op, bool decrement_size)
{
    op.count();
    if (root == nullptr)
        return nullptr;

    if (decrement_size)
        root->size--;

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
        return Select(root->left, i, op, decrement_size);
    }
    op.count();
    return Select(root->right, i - r, op, decrement_size);
}

OSTree::Node* OSTree::Delete(Node* node, Operation op)
{
    op.count();
    if (node->left != nullptr && node->right != nullptr)
    {
        Node* successor = FindMin(node->right, op, true);
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

void OSTree::Print(Node* node, int indent)
{
    cout << string(indent, ' ');
    if (node == nullptr)
    {
        cout << "null\n";
    }
    else {
        cout << node->data << "(" << node->size << ")" << "\n";
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
        int index = rand() % os->Size() + 1;
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
                int index = rand() % os->Size() + 1;
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