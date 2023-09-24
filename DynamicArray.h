#ifndef DYNAMICARRAY
#define DYNAMICARRAY

#include <iostream>
#include <algorithm>

template <class T>
class ArraySequence;

template <class T>
class DynamicArray
{
    // friend class ArraySequence<T>;

public:
    DynamicArray()
    {
        items = NULL;
        this->count = 0;
    }

    DynamicArray(int count)
    {
        if (!count)
        {
            items = NULL;
            this->count = 0;
        }
        else
        {
            items = new T[count];
            this->count = count;
        }
    }

    DynamicArray(const T *items, int count) : DynamicArray(count)
    {
        if (count)
        {
            std::copy(items, items + count, this->items);
        }
    }

    DynamicArray(T value, int count) : DynamicArray(count)
    {
        for (int i = 0; i < count; i++)
        {
            items[i] = value;
        }
    }

    DynamicArray(const DynamicArray<T> &dynamicArray) : DynamicArray(dynamicArray.items, dynamicArray.count) {}

    int GetSize() const
    {
        return count;
    }

    T Get(int index) const
    {
        return items[index];
    }

    T *GetPointer(int index) const
    {
        return items + index;
    }

    int GetIndex(T value) const
    {
        for (int i = 0; i < count; i++)
            if (items[i] == value)
                return i;
        return -1;
    }

    DynamicArray<T> *Reverse() const
    {
        DynamicArray<T> *res = new DynamicArray(count);
        for (int i = 0; i < count; i++)
        {
            res->Set(i, Get(count - i - 1));
        }
        return res;
    }

    void Set(int index, T value)
    {
        items[index] = value;
    }

    void Resize(int newSize)
    {
        if (newSize != count)
        {
            if(newSize < count)
                count = newSize;
            T *newItems = new T[newSize];
            
            for (int i = 0; i < count; i++)
            {
                newItems[i] = items[i];
            }
            delete[] items;
            items = newItems;
        }
    }

    void InsertAt(int index, T value)
    { // считаем, что память выделили извне
        for (int i = count - 1; i >= index; i--)
        {
            items[i + 1] = items[i];
        }
        items[index] = value;
        count++;
    }

    void RemoveLast()
    {
        Resize(--count);
    }

    void RemoveFirst()
    {
        for (int i = 0; i < count - 1; i++)
        {
            items[i] = items[i + 1];
        }
        Resize(--count);
    }

    void Remove(int index)
    {
        for (int i = index; i < count - 1; i++)
        {
            items[i] = items[i + 1];
        }
        Resize(--count);
    }

    void RemoveValue(T value)
    {
        for (int i = 0; i < count; i++)
            if (Get(i) == value)
            {
                Remove(i);
                break;
            }
    }

    ~DynamicArray()
    {
        if (items)
        {
            delete[] items;
        }
    }

private:
    T *items;
    int count;
};

#endif