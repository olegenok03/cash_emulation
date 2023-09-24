#ifndef LISTSEQUENCE
#define LISTSEQUENCE

#include <iostream>
#include <functional>

#include "LinkedList.h"
#include "Sequence.h"

template <class T>
class ListSequence : public Sequence<T>
{
public:
    ListSequence()
    {
        buffer = new LinkedList<T>();
    }

    ListSequence(T *items, int count)
    {
        buffer = new LinkedList<T>(items, count);
    }

    ListSequence(int count)
    {
        buffer = new LinkedList<T>(count);
    }

    ListSequence(T value, int count)
    {
        buffer = new LinkedList<T>(value, count);
    }

    ListSequence(const LinkedList<T> &list)
    {
        buffer = new LinkedList<T>(list);
    }

    ListSequence(const ListSequence<T> &listSequence) : ListSequence(*listSequence.buffer)
    {
        /*buffer = new LinkedList<T>();
        for (int i = 0; i < sequence.GetLength(); i++)
        {
            Append(sequence.Get(i));
        }*/
    }

    int GetLength() const override
    {
        return buffer->GetLength();
    }

    int GetAllocatedSize() const override
    {
        return buffer->GetLength();
    }

    T GetFirst() const override
    {
        return buffer->GetFirst();
    }

    T GetLast() const override
    {
        return buffer->GetLast();
    }

    T Get(int index) const override
    {
        return buffer->Get(index);
    }

    T *GetPointer(int index) const override
    {
        return buffer->GetPointer(index);
    }

    int GetIndex(T value) const override
    {
        return buffer->GetIndex(value);
    }

    int GetFirstIndexIf(std::function<bool(T value)> condition) const override
    {
        return buffer->GetFirstIndexIf(condition);
    }

    int GetLastIndexIf(std::function<bool(T value)> condition) const override
    {
        return buffer->GetLastIndexIf(condition);
    }

    Sequence<T> *Reverse() const override
    {
        LinkedList<T> *reversedList = buffer->Reverse();
        Sequence<T> *res = new ListSequence(*reversedList);
        delete reversedList;
        return res;
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        LinkedList<T> *subList = buffer->GetSubList(startIndex, endIndex);
        Sequence<T> *res = new ListSequence(*subList);
        delete subList;
        return res;
    }

    Sequence<T> *Copy() const override
    {
        return new ListSequence(*this);
    }

    Sequence<T> *Empty() const override
    {
        return new ListSequence();
    }

    void Append(T value) override
    {
        buffer->Append(value);
    }

    void Prepend(T value) override
    {
        buffer->Prepend(value);
    }

    void InsertAt(int index, T value) override
    {
        buffer->InsertAt(index, value);
    }

    void RemoveLast() override
    {
        buffer->RemoveLast();
    }

    void RemoveFirst() override
    {
        buffer->RemoveFirst();
    }

    void Remove(int index) override
    {
        buffer->Remove(index);
    }

    void RemoveValue(T value) override
    {
        buffer->RemoveValue(value);
    }

    void Set(int index, T value) override
    {
        buffer->Set(index, value);
    }

    Sequence<T> *Concat(Sequence<T> *list) const override
    {
        Sequence<T> *res = new ListSequence<T>(*buffer);
        for (int i = 0; i < list->GetLength(); i++)
        {
            res->Append(list->Get(i));
        }
        return res;
    }

    Sequence<T> *Split(int index) override
    {
        Sequence<T> *res = GetSubsequence(index, GetLength());
        for (int i = index; i < index + res->GetLength(); i++)
        {
            RemoveLast();
        }
        return res;
    }

    Sequence<T> &operator=(const Sequence<T> &x) override
    {
        ListSequence<T> temp = *(ListSequence<T> *)x.Copy();
        Swap(temp);
        return *this;
    }

    ~ListSequence() override
    {
        delete buffer;
    }

private:
    LinkedList<T> *buffer;

    void Swap(ListSequence<T> &list)
    {
        LinkedList<T> *bubble_buffer = buffer;
        buffer = list.buffer;
        list.buffer = bubble_buffer;
    }
};

#endif