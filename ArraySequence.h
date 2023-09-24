#ifndef ARRAYSEQUENCE
#define ARRAYSEQUENCE

#include <iostream>
#include <cstring>
#include <functional>

#include "DynamicArray.h"
#include "Sequence.h"

template <class T>
class ArraySequence : public Sequence<T>
{
public:
    ArraySequence()
    {
        buffer = new DynamicArray<T>();
        allocated_size = 0;
    }

    ArraySequence(T *items, int count)
    {
        buffer = new DynamicArray<T>(items, count);
        allocated_size = count;
    }

    ArraySequence(int count)
    {
        buffer = new DynamicArray<T>(count);
        allocated_size = count;
    }

    ArraySequence(T value, int count)
    {
        buffer = new DynamicArray<T>(value, count);
        allocated_size = count;
    }

    ArraySequence(const DynamicArray<T> &dynamicArray)
    {
        buffer = new DynamicArray<T>(dynamicArray);
        allocated_size = dynamicArray.GetSize();
    }

    ArraySequence(const ArraySequence<T> &arraySequence)
    {
        buffer = new DynamicArray<T>(arraySequence.GetAllocatedSize());
        allocated_size = arraySequence.GetAllocatedSize();
        for (int i = 0; i < arraySequence.GetLength(); i++)
        {
            Set(i, arraySequence.Get(i));
        }
    }

    int GetLength() const override
    {
        return buffer->GetSize();
    }

    int GetAllocatedSize() const override
    {
        return allocated_size;
    }

    T GetFirst() const override
    {
        return buffer->Get(0);
    }

    T GetLast() const override
    {
        return buffer->Get(buffer->GetSize() - 1);
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
        for (int i = 0; i < GetLength(); i++)
        {
            if(condition(Get(i)))
                return i;
        }
        return GetLength();
    }

    int GetLastIndexIf(std::function<bool(T value)> condition) const override
    {
        int index = -1;
        for (int i = 0; i < GetLength(); i++)
        {
            if(condition(Get(i)))
                index = i;
        }
        return index;
    }

    Sequence<T> *Reverse() const override
    {
        DynamicArray<T> *reversedArray = buffer->Reverse();
        Sequence<T> *res = new ArraySequence<T>(*reversedArray);
        delete reversedArray;
        return res;
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        DynamicArray<T> *sub_buf = new DynamicArray<T>(buffer->GetPointer(startIndex), endIndex - startIndex);
        ArraySequence<T> *res = new ArraySequence<T>(*sub_buf);
        delete sub_buf;
        return res;
    }

    Sequence<T> *Copy() const override
    {
        return new ArraySequence<T>(*this);
    }

    Sequence<T> *Empty() const override
    {
        return new ArraySequence<T>();
    }

    void Append(T value) override
    {
        if (!allocated_size)
        {
            Resize(1);
        }
        else if (buffer->GetSize() == allocated_size)
        {
            Resize(buffer->GetSize() * 2);
        }
        buffer->InsertAt(buffer->GetSize(), value);
    }

    void Prepend(T value) override
    {
        if (!allocated_size)
        {
            Resize(1);
        }
        else if (buffer->GetSize() == allocated_size)
        {
            Resize(buffer->GetSize() * 2);
        }
        buffer->InsertAt(0, value);
    }

    void InsertAt(int index, T value) override
    {
        if (!allocated_size)
        {
            Resize(1);
        }
        else if (buffer->GetSize() == allocated_size)
        {
            Resize(buffer->GetSize() * 2);
        }
        buffer->InsertAt(index, value);
    }

    void RemoveLast() override
    {
        buffer->RemoveLast();
        allocated_size = buffer->GetSize();
    }

    void RemoveFirst() override
    {
        buffer->RemoveFirst();
        allocated_size = buffer->GetSize();
    }

    void Remove(int index) override
    {
        buffer->Remove(index);
        allocated_size = buffer->GetSize();
    }

    void RemoveValue(T value) override
    {
        buffer->RemoveValue(value);
        allocated_size = buffer->GetSize();
    }

    void Set(int index, T value) override
    {
        buffer->Set(index, value);
    }

    Sequence<T> *Concat(Sequence<T> *list) const override
    {
        ArraySequence<T> *res = new ArraySequence<T>(*buffer);
        res->Resize(res->GetLength() + list->GetLength());
        for (int i = 0; i < list->GetLength(); i++)
        {
            res->Set(this->GetLength() + i, list->Get(i));
        }
        return res;
    }

    Sequence<T> *Split(int index) override
    {
        Sequence<T> *res = GetSubsequence(index, GetLength());
        Resize(index);
        return res;
    }

    Sequence<T> &operator=(const Sequence<T> &x) override
    {
        ArraySequence<T> temp = *(ArraySequence<T> *)x.Copy();
        Swap(temp);
        return *this;
    }

    ~ArraySequence() override
    {
        delete buffer;
    }

private:
    DynamicArray<T> *buffer;
    int allocated_size;

    void Resize(int newSize)
    {
        buffer->Resize(newSize);
        allocated_size = newSize;
    }

    void Swap(ArraySequence<T> &array)
    {
        DynamicArray<T> *bubble_buffer = buffer;
        int bubble_allocated_size = allocated_size;
        buffer = array.buffer;
        allocated_size = array.allocated_size;
        array.buffer = bubble_buffer;
        array.allocated_size = bubble_allocated_size;
    }
};

#endif