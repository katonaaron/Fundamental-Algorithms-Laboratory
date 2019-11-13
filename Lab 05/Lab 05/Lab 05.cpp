#include <iostream>
#include <fstream>
#include <time.h>
#include <cassert>

using namespace std;

#define DEMO

class UniversalHash {
    static const long long p = 2147483659;
    const size_t bucketSize;

    long long a;
    long long b;

public:
    UniversalHash(const size_t bucketSize) : bucketSize(bucketSize)
    {
        srand(time(NULL));
        a = rand() % (p - 1) + 1;
        b = rand() % p;
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
    const char* Search(int key);
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
    int i = 0;
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
const char* HashTable<Hash>::Search(int key)
{
    int i = 0;
    int auxiliaryHashValue = auxiliaryHash(key);
    int hashValue;
    do {
        hashValue = hash(auxiliaryHashValue, i);
        if (data[hashValue] == nullptr)
        {
            return nullptr;
        }
        if (data[hashValue]->id == key)
        {
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
    for (int i = 0; i < bucketSize; i++)
    {
        delete data[i];
    }
    delete data;
}



void Demo()
{
    const size_t bucketSize = 4;

    HashTable<UniversalHash> ht(bucketSize);
    ht.Insert(1, "name1");
    ht.Insert(2, "name2");
    ht.Insert(INT_MIN, "nameINT_MIN");
    ht.Insert(INT_MAX, "nameINT_MAX");
    ht.Insert(2, "name22");
    cout << ht.Search(1) << "\n";
    cout << ht.Search(2) << "\n";
    cout << ht.Search(INT_MIN) << "\n";
    cout << ht.Search(INT_MAX) << "\n";
}


void GenerateInput(int A[], int Size)
{
    for (int i = 0; i < Size; i++)
    {
        A[i] = i;
    }
}

void Evaluate()
{
    const size_t bucketSize = 10007;
    static int data[bucketSize];
    const double loadFactors[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };

    for (int i = 0; i < sizeof(loadFactors) / sizeof(loadFactors[0]); i++)
    {
        HashTable<UniversalHash> ht(bucketSize);
        int size = loadFactors[i] * bucketSize;

        FillRandomArray(data, size, 0, 50000, true, 0);
        for (int j = 0; j < size; j++)
        {
            ht.Insert(data[j], ("name" + to_string(data[j])).c_str());
        }

        if (size != ht.Size())
        {
            assert(size == ht.Size());
        }
        for (int j = 0; j < size; j++)
        {
            const char* name = ht.Search(data[j]);
            assert(name != nullptr);
            assert(strcmp(name, ("name" + to_string(data[j])).c_str()) == 0);
        }
        //profiler.divideValues(("load_factor_" + to_string(loadFactors[i])).c_str(), size);

    }
    for (int i = 0; i < sizeof(loadFactors) / sizeof(loadFactors[0]); i++)
    {
        //profiler.divideValues(("load_factor_" + to_string(loadFactors[i])).c_str(), 1);
    }
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
