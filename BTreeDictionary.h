#ifndef BTREEDICTIONARY
#define BTREEDICTIONARY


#include "IDictionary.h"
#include "BTree.h"

#include "Sequence.h"

template <class TKey, class TElement>
class BTreeDictionary : public IDictionary<TKey, TElement>
{
public:
    BTreeDictionary()
    {
        tree = new BTree<TKey, TElement>();
    }

    BTreeDictionary(const BTreeDictionary<TKey, TElement> &dict)
    {
        tree = new BTree<TKey, TElement>(*dict.tree);
    }

    int GetCount() const override
    {
        return tree->GetCount();
    }

    int GetCapacity() const override
    {
        return tree->GetCapacity();
    }

    TElement Get(TKey key) const override
    {
        return tree->Get(key);
    }

    TElement *GetPointer(TKey key) const override
    {
        return tree->GetPointer(key);
    }

    Sequence<TKey> *GetKeys() const override
    {
        return tree->GetKeys();
    }

    Sequence<TElement> *GetElements() const override
    {
        return tree->GetElements();
    }

    bool ContainsKey(TKey key) const override
    {
        return tree->ContainsKey(key);
    }

    IDictionary<TKey, TElement> *Copy() const override
    {
        return new BTreeDictionary<TKey, TElement>(*this);
    }

    IDictionary<TKey, TElement> *Empty() const override
    {
        return new BTreeDictionary<TKey, TElement>();        
    }

    void Add(TKey key, TElement element) override
    {
        tree->Add(key, element);
    }

    void Remove(TKey key) override
    {
        tree->Remove(key);
    }

    IDictionary<TKey, TElement> &operator=(const IDictionary<TKey, TElement> &dict) override
    {
        BTreeDictionary<TKey, TElement> temp = *(BTreeDictionary<TKey, TElement> *)dict.Copy();
        Swap(temp);
        return *this;
    }

    ~BTreeDictionary() override
    {
        delete tree;
    }

private:
    BTree<TKey, TElement> *tree;

    void Swap(BTreeDictionary<TKey, TElement> &dict)
    {
        BTree<TKey, TElement> *bubble_tree = tree;
        tree = dict.tree;
        dict.tree = bubble_tree;
    }
};

#endif