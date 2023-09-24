#ifndef HASHTABLE
#define HASHTABLE

#include "ArraySequence.h"
#include "ListSequence.h"
#include "Sequence.h"
#include "Pair.h"

template <class TKey, class TElement>
class HashTable
{
public:
    HashTable()
    {
        this->size = 0;
        this->keys = NULL;
        this->lists = NULL;
        this->getHashCode = NULL;
    }

    HashTable(int capacity, int (*getHashCode)(TKey key))
    {
        this->size = (capacity + 1) / maxLoad;
        this->keys = new ListSequence<TKey>();
        this->lists = new ArraySequence<ListSequence<Pair<TKey, TElement>> *>(size);
        for (int i = 0; i < size; i++)
        {
            this->lists->Set(i, new ListSequence<Pair<TKey, TElement>>());
        }
        this->getHashCode = getHashCode;
    }

    HashTable(HashTable<TKey, TElement> &hashTable)
    {
        this->size = hashTable.size;
        this->keys = dynamic_cast<ListSequence<TKey> *>(hashTable.keys->Copy());
        this->lists = dynamic_cast<ArraySequence<ListSequence<Pair<TKey, TElement>> *> *>(hashTable.lists->Copy());
        for (int i = 0; i < size; i++)
        {
            if (lists->Get(i))
            {
                this->lists->Set(i, dynamic_cast<ListSequence<Pair<TKey, TElement>> *>(hashTable.lists->Get(i)->Copy()));
            }
        }
        this->getHashCode = hashTable.getHashCode;
    }

    static void InitClass(double newMaxLoad, double newMinLoad, double newRebuildCoef)
    {
        maxLoad = newMaxLoad;
        minLoad = newMinLoad;
        rebuildCoef = newRebuildCoef;
    }

    HashTable<TKey, TElement> *Copy() const
    {
        return new HashTable<TKey, TElement>(*this);
    }

    HashTable<TKey, TElement> *Empty() const
    {
        return new HashTable<TKey, TElement>(size * maxLoad, getHashCode);
    }

    int GetCount() const
    {
        return keys->GetLength();
    }

    int GetSize() const
    {
        return size;
    }

    int GetCapacity() const
    {
        int capacity = size * maxLoad;
        if(!capacity)
            capacity++;
        return capacity;
    }

    TElement Get(TKey key) const
    {
        ListSequence<Pair<TKey, TElement>> *list = lists->Get(GetHash(key));
        for (int j = 0; j < list->GetLength(); j++)
            if (list->Get(j).GetFirst() == key)
                return list->Get(j).GetSecond();
    }

    TElement *GetPointer(TKey key) const
    {
        ListSequence<Pair<TKey, TElement>> *list = lists->Get(GetHash(key));
        for (int j = 0; j < list->GetLength(); j++)
            if (list->GetPointer(j)->GetFirst() == key)
                return list->GetPointer(j)->GetSecondPointer();
    }

    Sequence<TKey> *GetKeys() const
    {
        return keys->Copy();
    }

    Sequence<TElement> *GetElements() const
    {
        ArraySequence<TElement> *elements = new ArraySequence<TElement>(GetCount());
        for(int i = 0; i < GetCount(); i++)
        {
            elements->Set(i, Get(keys->Get(i)));
        }
        return elements;
    }

    bool ContainsKey(TKey key) const
    {
        ListSequence<Pair<TKey, TElement>> *list = lists->Get(GetHash(key));
        for (int j = 0; j < list->GetLength(); j++)
            if (list->Get(j).GetFirst() == key)
                return true;
        return false;
    }

    void Add(TKey key, TElement element)
    {
        keys->Append(key);
        ListSequence<Pair<TKey, TElement>> *list = lists->Get(GetHash(key));
        list->Append(Pair(key, element));
    }

    void Remove(TKey key)
    {
        ListSequence<Pair<TKey, TElement>> *list = lists->Get(GetHash(key));
        for (int j = 0; j < list->GetLength(); j++)
            if (list->Get(j).GetFirst() == key)
            {
                list->Remove(j);
                break;
            }
        keys->RemoveValue(key);
    }

    virtual void Rebuild()
    {
        if (GetCount() >= GetCapacity())
        {
            int newSize = size * rebuildCoef;
            HashTable<TKey, TElement> *newTable = new HashTable<TKey, TElement>(newSize * maxLoad + 1, getHashCode);
            for(int i = 0; i < keys->GetLength(); i++)
            {
                TKey key = keys->Get(i);
                newTable->Add(key, Get(key));
            }
            Swap(*newTable);
            delete newTable;
        }

        else if (GetCount() <= GetCapacity() * minLoad)
        {
            int newSize = size / rebuildCoef;
            HashTable<TKey, TElement> *newTable = new HashTable<TKey, TElement>(newSize * maxLoad, getHashCode);
            for(int i = 0; i < keys->GetLength(); i++)
            {
                TKey key = keys->Get(i);
                newTable->Add(key, Get(key));
            }
            Swap(*newTable);
            delete newTable;
        }

    }

    HashTable<TKey, TElement> &operator=(const HashTable<TKey, TElement> &table)
    {
        HashTable<TKey, TElement> temp = *(HashTable<TKey, TElement> *)table.Copy();
        Swap();
        return *this;
    }

    virtual ~HashTable()
    {
        delete keys;
        for (int i = 0; i < size; i++)
            delete lists->Get(i);
        delete lists;
    }

protected:

    static double maxLoad;

    int size;

    ListSequence<TKey> *keys;

    ArraySequence<ListSequence<Pair<TKey, TElement>> *> *lists;

    int (*getHashCode)(TKey key);

private:
    static double rebuildCoef, minLoad;

    int GetHash(TKey Key) const
    {
        return (getHashCode(Key) % size + size) % size;
    }

    void Swap(HashTable<TKey, TElement> &table)
    {
        ListSequence<TKey> *bubble_keys = keys;
        ArraySequence<ListSequence<Pair<TKey, TElement>> *> *bubble_lists = lists;
        int bubble_size = size;

        keys = table.keys;
        lists = table.lists;
        size = table.size;

        table.keys = bubble_keys;
        table.lists = bubble_lists;
        table.size = bubble_size;
    }
};

/*template <class TKey, class TElement>
class HashTableFixed : public HashTable <TKey, TElement>
{
public:
    HashTableFixed():HashTable<TKey, TElement>(){}

    HashTableFixed(int capacity, int (*getHashCode)(TKey key)) : HashTable<TKey, TElement>(capacity, getHashCode){}

    HashTableFixed(HashTable<TKey, TElement> &hashTable) : HashTable<TKey, TElement>(hashTable) {}

    void Rebuild() override {}

    ~HashTableFixed()
    {
        delete this->keys;
        for (int i = 0; i < this->size; i++)
            delete this->lists->Get(i);
        delete this->lists;
    }
};*/

template <class TKey, class TElement>
double HashTable<TKey, TElement>::rebuildCoef = 0;

template <class TKey, class TElement>
double HashTable<TKey, TElement>::minLoad = 0;

template <class TKey, class TElement>
double HashTable<TKey, TElement>::maxLoad = 0;

#endif