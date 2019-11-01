#include <iostream>
#include <list>
#include <vector>
#include "Profiler.h"

using namespace std;

enum SORTED_ORDER { RANDOM = 0, ASC, DESC };

const int RANGE_MIN = 1;
const int RANGE_MAX = 20;

template <class T>
void PrintVector(vector<T> Vector, string Message = "")
{
    for (T num : Vector)
    {
        cout << num << " ";
    }
    cout << "- " << Message << "\n";
}

void PrintList(list<int> List, string Message = "")
{
    for (int num : List)
    {
        cout << num << " ";
    }
    cout << "- " << Message << "\n";
}

void PrintLists(vector<list<int>> Lists)
{
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
        output << "Element{key=" << E.key << ", list_index=" << E.list_index << "}";
        return output;
    }
};

template <class T, class Compare = std::less<T>>
class Heap
{
    vector<T> data;
    Compare compare = Compare();
public:

    Heap() {};
    Heap(vector<T> Data);

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
Heap<T, Compare>::Heap(vector<T> Data)
{
    this->data = Data;
    BuildHeap();
}

template<class T, class Compare>
void Heap<T, Compare>::Push(T Element)
{
    data.push_back(Element);
    SiftUp(data.size() - 1);
}

template<class T, class Compare>
void Heap<T, Compare>::Pop()
{
    data.at(0) = data.at(data.size() - 1);
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
    //Op.count(2); //2x Comp

    if (largest != i)
    {
        swap(data[i], data[largest]);
        //Op.count(3);

        /*if (Debug)
            PrintHeap(A, HeapSize, to_string(A[largest]) + " swapped with " + to_string(A[i]));*/

        Heapify(largest);
    }
}

template<class T, class Compare>
void Heap<T, Compare>::SiftUp(int i)
{
    while (i > 0 && compare(data[Parent(i)], data[i]))
    {
        swap(data[Parent(i)], data[i]);
        /*if (Debug)
            PrintHeap(A, HeapSize, to_string(A[Parent(i)]) + " was swapped with " + to_string(A[i]));*/
        i = Parent(i);
        //Op.count(4); //comp + swap
    }
    //if (i > 0)
    //    Op.count(); //comp
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

list<int> MergeKSortedLists(vector<list<int>> Lists)
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

    PrintVector(data, "Before Build Heap");
    Heap<Element>heap(data);
    heap.Print("After BuildHeap");

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

int main()
{
    vector<list<int>> lists = GenerateKSortedLists(20, 4);
    PrintLists(lists);
    /*Merge(lists.at(0), lists.at(1));
    PrintLists(lists);*/

    list<int> result = MergeKSortedLists(lists);
    PrintList(result, "The result after merge");
    return 0;
}
