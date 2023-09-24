#ifndef SEQUENCE
#define SEQUENCE

#include <iostream>
#include <functional>

template <class T>
class Sequence
{
public:
    virtual int GetLength() const = 0;

    virtual int GetAllocatedSize() const = 0;

    virtual T GetFirst() const = 0;

    virtual T GetLast() const = 0;

    virtual T Get(int index) const = 0;

    virtual T *GetPointer(int index) const = 0;

    virtual int GetIndex(T value) const = 0;

    virtual int GetFirstIndexIf(std::function<bool(T value)> condition) const = 0;

    virtual int GetLastIndexIf(std::function<bool(T value)> condition) const = 0;

    virtual Sequence<T> *Reverse() const = 0;

    virtual Sequence<T> *GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T> *Copy() const = 0;

    virtual Sequence<T> *Empty() const = 0;

    virtual void Append(T value) = 0;

    virtual void Prepend(T value) = 0;

    virtual void InsertAt(int index, T value) = 0;

    virtual void RemoveLast() = 0;

    virtual void RemoveFirst() = 0;

    virtual void Remove(int index) = 0;

    virtual void RemoveValue(T value) = 0;

    virtual void Set(int index, T value) = 0;

    void Permutate(int firstIndex, int secondIndex)
    {
        if (firstIndex != secondIndex)
        {
            T bubble = Get(firstIndex);
            Set(firstIndex, Get(secondIndex));
            Set(secondIndex, bubble);
        }
    }

    virtual Sequence<T> *Concat(Sequence<T> *list) const = 0;

    virtual Sequence<T> *Split(int index) = 0;

    virtual Sequence<T> &operator=(const Sequence<T> &x) = 0;

    virtual ~Sequence() = 0;
};

template <class T>
Sequence<T>::~Sequence() {}

#endif