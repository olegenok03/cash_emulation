#ifndef IDICTIONARY
#define IDICTIONARY

#include "Sequence.h"

template <class TKey, class TElement>
class IDictionary
{
public:
    virtual int GetCount() const = 0;

    virtual int GetCapacity() const = 0;

    virtual TElement Get(TKey key) const = 0;

    virtual TElement *GetPointer(TKey key) const = 0;

    virtual Sequence<TKey> *GetKeys() const = 0;

    virtual Sequence<TElement> *GetElements() const = 0;
    
    virtual bool ContainsKey(TKey key) const = 0;

    virtual IDictionary<TKey, TElement> *Copy() const = 0;

    virtual IDictionary<TKey, TElement> *Empty() const = 0;

    virtual void Add(TKey key, TElement element) = 0;

    virtual void Remove(TKey key) = 0;

    virtual IDictionary<TKey, TElement> &operator=(const IDictionary<TKey, TElement> &dict) = 0;

    virtual ~IDictionary() = 0;
};

template <class TKey, class TElement>
IDictionary<TKey, TElement>::~IDictionary() {}

#endif