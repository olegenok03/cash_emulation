#ifndef HASHTABLEDICTIONARY
#define HASHTABLEDICTIONARY

#include "IDictionary.h"
#include "HashTable.h"

#include "Sequence.h"

template <class TKey, class TElement>
class HashTableDictionary : public IDictionary<TKey, TElement>
{
public:
    HashTableDictionary()
    {
        table = new HashTable<TKey, TElement>();
    }

    HashTableDictionary(int capacity, int (*getHashCode)(TKey key))
    {
        table = new HashTable<TKey, TElement>(capacity, getHashCode);
    }

    HashTableDictionary(const HashTableDictionary<TKey, TElement> &dict)
    {
        table = new HashTable<TKey, TElement>(*dict.table);
    }

    int GetCount() const override
    {
        return table->GetCount();
    }

    int GetSize() const
    {
        return table->GetSize();
    }

    int GetCapacity() const override
    {
        return table->GetCapacity();
    }

    TElement Get(TKey key) const override
    {
        return table->Get(key);
    }

    TElement *GetPointer(TKey key) const override
    {
        return table->GetPointer(key);
    }

    Sequence<TKey> *GetKeys() const override
    {
        return table->GetKeys();
    }

    Sequence<TElement> *GetElements() const override
    {
        return table->GetElements();
    }

    bool ContainsKey(TKey key) const override
    {
        return table->ContainsKey(key);
    }

    IDictionary<TKey, TElement> *Copy() const override
    {
        return new HashTableDictionary<TKey, TElement>(*this);
    }

    IDictionary<TKey, TElement> *Empty() const override
    {
        HashTableDictionary<TKey, TElement> *res = new HashTableDictionary<TKey, TElement>();
        delete res->table;
        res->table = this->table->Empty();
        return res;
    }

    virtual void Add(TKey key, TElement element) override
    {
        table->Add(key, element);
        table->Rebuild();
    }

    virtual void Remove(TKey key) override
    {
        table->Remove(key);
        table->Rebuild();
    }

    IDictionary<TKey, TElement> &operator=(const IDictionary<TKey, TElement> &dict) override
    {
        HashTableDictionary<TKey, TElement> temp = *(HashTableDictionary<TKey, TElement> *)dict.Copy();
        Swap(temp);
        return *this;
    }

    virtual ~HashTableDictionary() override
    {
        delete table;
    }

protected:
    HashTable<TKey, TElement> *table;

private:
    void Swap(HashTableDictionary<TKey, TElement> &dict)
    {
        HashTable<TKey, TElement> *bubble_table = table;
        table = dict.table;
        dict.table = bubble_table;
    }
};

template <class TKey, class TElement>
class HashTableDictionaryFixed : public HashTableDictionary <TKey, TElement>
{
public:
    HashTableDictionaryFixed() : HashTableDictionary<TKey, TElement>() {}

    HashTableDictionaryFixed(int capacity, int (*getHashCode)(TKey key)) : HashTableDictionary<TKey, TElement>(capacity, getHashCode) {}

    HashTableDictionaryFixed(const HashTableDictionary<TKey, TElement> &dict) : HashTableDictionary<TKey, TElement>(dict) {}

    void Add(TKey key, TElement element) override
    {
        this->table->Add(key, element);
    }

    void Remove(TKey key) override
    {
        this->table->Remove(key);
    }

    ~HashTableDictionaryFixed()
    {
        delete this->table;
    }
};

#endif