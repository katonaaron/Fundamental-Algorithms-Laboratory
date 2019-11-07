#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

class UniversalHash {
    static const long long p = (long long)INT_MAX + 1;
    const int bucketSize;

    long long a;
    long long b;

public:
    UniversalHash(const int bucketSize) : bucketSize(bucketSize)
    {
        srand(time(0));
        a = rand() % (p - 1) + 1;
        b = rand() % p;
    };

    int operator() (int key)
    {
        return ((a * key + b) % p) % bucketSize;
    }
};

struct Entry {
    static const int nameSize = 30;
    int id;
    char name[nameSize];
};

template <class Hash>
class HashTable {
    static const int bucketSize = 10007;

    Entry* data[bucketSize];
    const double fillingFactor;
    Hash auxiliaryHash;

public:
    HashTable(double fillingFactor) : fillingFactor(fillingFactor), data(), auxiliaryHash(bucketSize) {}
    void Insert(int key, const char* name);
    const char* Search(int key);

private:
    int hash(int auxiliaryHashValue, int i);

};

template<class Hash>
int HashTable<Hash>::hash(int auxiliaryHashValue, int i)
{
    const int c1 = 13;
    const int c2 = 17;
    return auxiliaryHashValue + c1 * i + c2 * (i << 1);
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

void Demo()
{
    HashTable<UniversalHash> ht(0.8);
    ht.Insert(1, "name1");
    cout << ht.Search(1);
}

int main()
{
    Demo();
    return 0;
}
