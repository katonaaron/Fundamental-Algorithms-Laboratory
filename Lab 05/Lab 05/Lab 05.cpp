#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>
#include <cstdlib>

#include "Profiler.h"

using namespace std;

//#define DEMO

long lrand()
{
    if (sizeof(int) < sizeof(long))
        return (static_cast<long>(rand()) << (sizeof(int) * 7)) |
        rand();

    return rand();
}

class UniversalHash {
    static const long long p = 2147483659;
    const size_t bucketSize;

    long long a;
    long long b;

public:
    UniversalHash(const size_t bucketSize) : bucketSize(bucketSize)
    {
        a = lrand() % (p - 1) + 1;
        b = lrand() % p;
        assert(a > 0);
        assert(b >= 0);
    };

    int operator() (int key)
    {
        return abs(((a * key + b) % p) % bucketSize);
    }
};

template <class Hash>
class HashTable {
    struct Entry {
        static const int nameSize = 30;
        int id;
        char name[nameSize];
    };

    const size_t bucketSize;
    Entry** data;
    Hash auxiliaryHash;
    size_t size = 0;
public:

    HashTable(const size_t bucketSize);
    void Insert(int key, const char* name);
    const char* Search(int key, int& effort);
    size_t Size();
    size_t BucketSize();
    ~HashTable();
private:
    int hash(int auxiliaryHashValue, int i);

};

template<class Hash>
int HashTable<Hash>::hash(int auxiliaryHashValue, int i)
{
    const int c1 = 13;
    const int c2 = 17;
    return (auxiliaryHashValue + c1 * i + c2 * (i << 1)) % bucketSize;
}

template<class Hash>
HashTable<Hash>::HashTable(const size_t bucketSize) : bucketSize(bucketSize), auxiliaryHash(bucketSize)
{
    data = new Entry * [bucketSize] {};
}

template<class Hash>
void HashTable<Hash>::Insert(int key, const char* name)
{
    unsigned int i = 0;
    int auxiliaryHashValue = auxiliaryHash(key);
    int hashValue;
    do {
        hashValue = hash(auxiliaryHashValue, i);
        if (data[hashValue] == nullptr)
        {
            data[hashValue] = new Entry();
            data[hashValue]->id = key;
            strcpy_s(data[hashValue]->name, name);
            size++;
            return;
        }
        else if (data[hashValue]->id == key)
        {
            strcpy_s(data[hashValue]->name, name);
            return;
        }
        i++;
    } while (i < bucketSize);
    throw new length_error("hash table overflow");
}

template<class Hash>
const char* HashTable<Hash>::Search(int key, int& effort)
{
    unsigned int i = 0;
    int auxiliaryHashValue = auxiliaryHash(key);
    int hashValue;
    do {
        hashValue = hash(auxiliaryHashValue, i);
        if (data[hashValue] == nullptr)
        {
            effort = i + 1;
            return nullptr;
        }
        if (data[hashValue]->id == key)
        {
            effort = i + 1;
            return data[hashValue]->name;
        }
        i++;
    } while (i < bucketSize);
    return nullptr;
}

template<class Hash>
size_t HashTable<Hash>::Size()
{
    return size;
}

template<class Hash>
size_t HashTable<Hash>::BucketSize()
{
    return bucketSize;
}

template<class Hash>
HashTable<Hash>::~HashTable()
{
    for (unsigned int i = 0; i < bucketSize; i++)
    {
        delete data[i];
    }
    delete data;
}



void Demo()
{
    const size_t bucketSize = 4;
    int effort;

    HashTable<UniversalHash> ht(bucketSize);
    ht.Insert(1, "name1");
    ht.Insert(2, "name2");
    ht.Insert(INT_MIN, "nameINT_MIN");
    ht.Insert(INT_MAX, "nameINT_MAX");
    ht.Insert(2, "name22");
    cout << ht.Search(1, effort) << "\n";
    cout << ht.Search(2, effort) << "\n";
    cout << ht.Search(INT_MIN, effort) << "\n";
    cout << ht.Search(INT_MAX, effort) << "\n";
}

void RandomPermutate(int a[], size_t size)
{
    int index;

    while (size > 1)
    {
        index = rand() % size;
        swap(a[index], a[--size]);
    }
}

void Evaluate()
{
    const size_t bucketSize = 10007;
    const unsigned int nrOfSearches = 3000;
    const unsigned int nrOfMeasurements = 5;
    const int range_min = 0;
    const int range_max = 50000;
    const double loadFactors[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };

    const int nrLoadFactors = sizeof(loadFactors) / sizeof(loadFactors[0]);
    double effortAvgFound[nrLoadFactors] = {};
    double effortAvgNotFound[nrLoadFactors] = {};
    double effortMaxFound[nrLoadFactors] = {};
    double effortMaxNotFound[nrLoadFactors] = {};
    static int data[bucketSize];

    for (unsigned int i = 0; i < nrOfMeasurements; i++)
    {
        for (int loadFactorIndex = 0; loadFactorIndex < nrLoadFactors; loadFactorIndex++)
        {
            HashTable<UniversalHash> ht(bucketSize);
            int size = (int)(loadFactors[loadFactorIndex] * bucketSize);
            int effort, totalEffort, maxEffort;

            FillRandomArray(data, size, range_min, range_max, true, 0);

            //Insert elements
            for (int j = 0; j < size; j++)
            {
                ht.Insert(data[j], ("name" + to_string(data[j])).c_str());
            }

            //Check for correct behaviour
            if (size != ht.Size())
            {
                assert(size == ht.Size());
            }
            for (int j = 0; j < size; j++)
            {
                const char* name = ht.Search(data[j], effort);
                assert(name != nullptr);
                assert(strcmp(name, ("name" + to_string(data[j])).c_str()) == 0);
            }

            //Evaluate search time     
            //Searching for existing elements
            RandomPermutate(data, size);
            totalEffort = 0;
            maxEffort = -1;
            for (int j = 0; j < nrOfSearches / 2; j++)
            {
                //const char* name = ht.Search(data[j], effort);
                int ind = rand() % size;
                const char* name = ht.Search(data[ind], effort);
                totalEffort += effort;
                maxEffort = max(effort, maxEffort);
                assert(name != nullptr);
                //assert(strcmp(name, ("name" + to_string(data[j])).c_str()) == 0);
                assert(strcmp(name, ("name" + to_string(data[ind])).c_str()) == 0);
            }
            effortAvgFound[loadFactorIndex] += totalEffort / (nrOfSearches / 2);
            effortMaxFound[loadFactorIndex] += maxEffort;

            //Searching for non-existing elements
            totalEffort = 0;
            maxEffort = -1;
            for (int j = 1; j <= (nrOfSearches + 1) / 2; j++)
            {
                const char* name = ht.Search(range_max + j, effort);
                totalEffort += effort;
                maxEffort = max(effort, maxEffort);
                assert(name == nullptr);
            }
            effortAvgNotFound[loadFactorIndex] += totalEffort / ((nrOfSearches + 1) / 2);
            effortMaxNotFound[loadFactorIndex] += maxEffort;
        }
    }
    for (unsigned int i = 0; i < nrLoadFactors; i++)
    {
        effortAvgFound[i] /= nrOfMeasurements;
        effortMaxFound[i] /= nrOfMeasurements;
        effortAvgNotFound[i] /= nrOfMeasurements;
        effortMaxNotFound[i] /= nrOfMeasurements;
    }

    ofstream g("output.html");
    g << "<!DOCTYPE html><html><head><style>table {  font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;  border-collapse: collapse;  width: 100%;}table td, table th {  border: 1px solid #ddd;  padding: 8px;}table tr:nth-child(even){background-color: #f2f2f2;}table tr:hover {background-color: #ddd;}table th {  padding-top: 12px;  padding-bottom: 12px;  text-align: left;  background-color: #4CAF50;  color: white;}</style></head><body>";
    g << "<table><tr><th>Filling factor</th><th>Avg. Effort found</th><th>Max. Effort found</th><th>Avg. Effort not-found</th><th>Max. Effort not-found</th></tr>";
    for (unsigned int i = 0; i < nrLoadFactors; i++)
    {
        g << "<tr><td>" << loadFactors[i] << "</td><td>"
            << effortAvgFound[i] << "</td><td>"
            << effortMaxFound[i] << "</td><td>"
            << effortAvgNotFound[i] << "</td><td>"
            << effortMaxNotFound[i] << "</td></tr>";
    }
    g << "</table></body></html>";
    g.close();
    system("output.html");
}

int main()
{
    srand(time(NULL));
#ifdef DEMO
    Demo();
#else
    Evaluate();
#endif // DEMO    

    /* int a[] = { 1, 2, 3, 4 };
     const int size = sizeof(a) / sizeof(a[0]);
     RandomPermutate(a, size);
     for (int i = 0; i < size; i++)
     {
         cout << a[i] << " ";
     }
     cout << "\n";*/
    return 0;
}
