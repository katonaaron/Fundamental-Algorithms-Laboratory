/**
 * @author Katona Aron
 * @group 30423
 *
 * Problem specification:
 *	Implement correctly and efficiently an O(n * log k) method for merging k sorted sequences, where n is the total number of elements.
 *
 * Algorithm:
 *  Denote n the number of total elements, k the number of lists.
 *  For this algorithm a heap was used. The elements of the heap are a pair of values: the "list_index", which references to a list in the vector of lists, and "key", 
 *      which is the smallest key from that list. 
 *  1. From each given sorted lists, the key of the first element is inserted to a vector, paired with the index of the list.
 *  (1'. The constructor of the heap copies the vector to an internal vector.)
 *  2. The bottom up heap build procedure is used to create heap from the vector.
 *  3. While the heap is not empty, meaning that there are lists that weren't fully merged:
 *      3.1 Extract the smallest element from the heap.
 *      3.2 Transfer the corresponding node from the source list to the result list.
 *      3.3 If the list became empty after the removal, the smallest element from the heap is popped, meaning that from that list no element can be further added.
 *      3.4 If the list didn't become empty after the removal, the key of the smallest element of the heap is updated with the current smallest element of the list.
 *  4. The resulting list is returned.
 *
 * Efficiency:
 *  Step 1, 1'and 2 of the algorithm represents the creation of the heap from the first elements of the lists. Each step is O(k): iterating through the first elements of the lists twice, then
 *      using the bottom up heap build procedure, which is O(k) in every case. This means that the complexity of these three steps is 3 * O(k) == O(k).
 *  In step 3.1 an O(1) method was used to extract the smallest element of the heap. 
 *  In step 3.2, transferring a node from the beginning of a list to the end of another is O(1).
 *  In step 3.3, removing the root of a heap is done by putting the last element to the first position and applying heapify. So the complexity of this step is O(1) + O(h) == O(h) == O(log k),
 *      because the size of the heap can have maximum k elements, each representing a source list.
 *  In step 3.4, updating the root of the heap follows with the heapify procedure. So the complexity of this step is also O(1) + O(h) == O(h) == O(log k).
 *  Step 3 is a loop, which is repeated n times. Either step 3.3 or step 3.4 is processed, which are both O(log k) algorithms. So the total complexity of this 
 *      step is n * (O(1) + O(1) + O(log k)) == O(n * log k).
 *  In step 4, returning an element is O(1).

 *  The total complexity of the algorithm is: O(k) + O(n log k) + O(1) == O(n * log k).
 *
 *
 * Observation:
 *  In the evaluation phase two types of experiments were done. 
 *  When k is a fixed value (10, 20, 100) and n is variable, log k is a constant value, so the complexity of the algorithm is O(n). This can be seen on the charts, for each fixed key, 
 *      the running time is linear.
 *  When n is a fixed value (10000) and k is variable, n is a constant, thus the complexity of the algorithm is O(log k). This can be seen on the chart, the growth rate of the
 *      running time is logarithmic.
 */

#include <iostream>
#include <list>
#include <vector>
#include "Profiler.h"
#include <cassert>

using namespace std;

#define DEMO

enum SORTED_ORDER { RANDOM = 0, ASC, DESC };

#ifdef DEMO
const int RANGE_MIN = 1;
const int RANGE_MAX = 20;
#else
const int RANGE_MIN = 10;
const int RANGE_MAX = 50000;
#endif // DEMO


template <class T>
void PrintVector(vector<T> Vector, string Message = "")
{
    for (T& num : Vector)
    {
        cout << num << " ";
    }
    cout << "- " << Message << "\n";
}

void PrintList(list<int> List, string Message = "")
{
    for (int& num : List)
    {
        cout << num << " ";
    }
    cout << "- " << Message << "\n";
}

void PrintLists(vector<list<int>> Lists, string Message = "")
{
    cout << Message << "\n";
    int i = 0;
    for (list<int>& l : Lists)
    {
        cout << "list #" << i++ << ": ";
        PrintList(l);
    }
    cout << "\n";
}

struct Element {
    int key;
    int list_index;

    Element(int Key, int ListIndex) : key(Key), list_index(ListIndex) {

    }

    bool operator <(const Element& element) const
    {
        return this->key < element.key;
    }
    bool operator >(const Element& element) const
    {
        return this->key > element.key;
    }
    friend ostream& operator<< (ostream& output, const Element& E)
    {
        output << "{key=" << E.key << ", list_index=" << E.list_index << "}";
        return output;
    }
};

template <class T, class Compare = std::less<T>>
class Heap
{
    vector<T> data;
    Compare compare = Compare();
    //Profiler
    Operation op;
public:

    Heap(Operation Op) : op(Op) {};
    Heap(vector<T> Data, Operation Op);

    void Push(T Element);
    void Pop();
    const T& Top();
    size_t Size();
    bool Empty();
    void ReplaceTop(T Element);


    //Debug
    void Print(std::string Message = "")
    {
        PrintVector(data, Message);
    }
private:
    inline int Parent(int index)
    {
        return (index - 1) / 2;
    }

    inline int Left(int index)
    {
        return index * 2 + 1;
    }

    inline int Right(int index)
    {
        return index * 2 + 2;
    }

    void BuildHeap();
    void Heapify(int i);
    void SiftUp(int i);
};

template<class T, class Compare>
Heap<T, Compare>::Heap(vector<T> Data, Operation Op) : op(Op)
{
    this->data = Data;
    op.count(data.size());
    BuildHeap();
}

template<class T, class Compare>
void Heap<T, Compare>::Push(T Element)
{
    data.push_back(Element);
    op.count();
    SiftUp(data.size() - 1);
}

template<class T, class Compare>
void Heap<T, Compare>::Pop()
{
    data.at(0) = data.at(data.size() - 1);
    op.count(1);
    data.pop_back();
    Heapify(0);
}

template<class T, class Compare>
const T& Heap<T, Compare>::Top()
{
    return data.at(0);
}

template<class T, class Compare>
size_t Heap<T, Compare>::Size()
{
    return data.size();
}

template<class T, class Compare>
bool Heap<T, Compare>::Empty()
{
    return data.empty();
}

template<class T, class Compare>
void Heap<T, Compare>::ReplaceTop(T Element)
{
    data.at(0) = Element;
    op.count(1);
    Heapify(0);
}

template<class T, class Compare>
void Heap<T, Compare>::BuildHeap()
{
    for (int i = (data.size() - 1) / 2; i >= 0; i--)
    {
        Heapify(i);
    }
}

template<class T, class Compare>
void Heap<T, Compare>::Heapify(int i)
{
    int largest = i;
    unsigned left = Left(i);
    unsigned right = Right(i);


    if (left < data.size() && compare(data[left], data[largest]))
    {
        largest = left;
    }
    if (right < data.size() && compare(data[right], data[largest]))
    {
        largest = right;
    }
    op.count(2); //2x Comp

    if (largest != i)
    {
        swap(data[i], data[largest]);
        op.count(3);

        Heapify(largest);
    }
}

template<class T, class Compare>
void Heap<T, Compare>::SiftUp(int i)
{
    while (i > 0 && compare(data[Parent(i)], data[i]))
    {
        swap(data[Parent(i)], data[i]);
        i = Parent(i);
        op.count(4); //comp + swap
    }
    if (i > 0)
        op.count(); //comp
}

vector<list<int>> GenerateKSortedLists(int n, int k)
{
    vector<list<int>> result;
    vector<int> data(n / k + 1);

    int remainder = n % k, size;
    for (int i = 0; i < k; i++) {
        if (remainder > 0)
        {
            remainder--;
            size = n / k + 1;
        }
        else
        {
            size = n / k;
        }
        FillRandomArray(data.data(), size, RANGE_MIN, RANGE_MAX, false, SORTED_ORDER::ASC);
        result.push_back(list<int>(data.begin(), next(data.begin(), size)));
    }
    return result;
}

void Merge(list<int>& List1, list<int>& List2)
{
    if (List2.size() == 0)
        return;
    if (List1.size() == 0)
    {
        List1 = List2;
        return;
    }

    list<int>::iterator it1 = List1.begin();

    while (it1 != List1.end() && !List2.empty())
    {
        if (*it1 <= List2.front())
        {
            it1++;
        }
        else
        {
            List1.splice(it1, List2, List2.begin());
        }
    }

    List1.splice(it1, List2);
}

list<int> MergeKSortedLists(vector<list<int>> Lists, Operation Op)
{
    list<int> result;

    vector<Element> data;
    for (unsigned i = 0; i < Lists.size(); i++)
    {
        if (!Lists.at(i).empty())
        {
            data.emplace_back(Lists.at(i).front(), i);
        }
    }
    Op.count(data.size());

#ifdef DEMO
    PrintVector(data, "Before Build Heap");
#endif // DEMO

    Heap<Element>heap(data, Op);

#ifdef DEMO
    heap.Print("After BuildHeap");
#endif // DEMO


    while (!heap.Empty())
    {
        const Element& element = heap.Top();
        list<int>& l = Lists.at(element.list_index);

        result.splice(result.end(), l, l.begin());

        if (l.empty())
        {
            heap.Pop();
        }
        else
        {
            heap.ReplaceTop(Element(l.front(), element.list_index));
        }
    }

    return result;
}

void Evaluate()
{
    Profiler profiler("Merge-K-Sorted-Lists");
    vector<int> k_vector = { 5, 10, 100 };
    const int n_range_min = 100;
    const int n_range_max = 10000;
    const int n_increment = 100;
    const int k_range_min = 10;
    const int k_range_max = 500;
    const int k_increment = 10;
    const int k_fixed_n = 10000;
    const int nr_of_measurements = 5;

    for (int m = 1; m <= nr_of_measurements; m++)
    {
        for (int& k : k_vector)
        {
            for (int n = n_range_min; n <= n_range_max; n += n_increment)
            {
                vector<list<int>> lists = GenerateKSortedLists(n, k);
                Operation op = profiler.createOperation(("k_" + to_string(k)).c_str(), n);

                list<int> result = MergeKSortedLists(lists, op);
                assert(result.size() == n);
            }
        }
    }

    for (int m = 1; m <= nr_of_measurements; m++)
    {
        for (int k = k_range_min; k <= k_range_max; k += k_increment)
        {
            vector<list<int>> lists = GenerateKSortedLists(k_fixed_n, k);
            Operation op = profiler.createOperation(("n_" + to_string(k_fixed_n)).c_str(), k);

            list<int> result = MergeKSortedLists(lists, op);
        }
    }

    profiler.divideValues(("k_" + to_string(k_vector.at(0))).c_str(), nr_of_measurements);
    profiler.divideValues(("k_" + to_string(k_vector.at(1))).c_str(), nr_of_measurements);
    profiler.divideValues(("k_" + to_string(k_vector.at(2))).c_str(), nr_of_measurements);
    profiler.divideValues(("n_" + to_string(k_fixed_n)).c_str(), nr_of_measurements);

    profiler.createGroup("merge_for_fixed_k",
        ("k_" + to_string(k_vector.at(0))).c_str(),
        ("k_" + to_string(k_vector.at(1))).c_str(),
        ("k_" + to_string(k_vector.at(2))).c_str()
    );
    profiler.createGroup("merge_for_fixed_n",
        ("n_" + to_string(k_fixed_n)).c_str()
    );
    profiler.showReport();
}

void Demo()
{
    vector<list<int>> lists = GenerateKSortedLists(9, 2);
    PrintLists(lists, "Initial vector of lists");
    Merge(lists.at(0), lists.at(1));
    PrintLists(lists, "The result after merging two lists");

    Profiler profiler("Demo");
    Operation op = profiler.createOperation("dummy", 0);
    lists = GenerateKSortedLists(20, 4);
    PrintLists(lists, "------------------\n\nInitial vector of lists");
    list<int> result = MergeKSortedLists(lists, op);
    PrintList(result, "The result after merging all the lists");
}

int main()
{
#ifdef DEMO
    Demo();
#else
    Evaluate();
#endif // DEMO

    return 0;
}
