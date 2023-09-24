#ifndef PAIR
#define PAIR

template <class T, class Y>
class Pair
{
public:

    Pair() {}

    Pair(const Pair<T, Y> &pair) = default;

    Pair(T first, Y second)
    {
        this->first = first;
        this->second = second;
    }

    T GetFirst()
    {
        return first;
    }

    Y GetSecond()
    {
        return second;
    }

    T *GetFirstPointer()
    {
        return &first;
    }

    Y *GetSecondPointer()
    {
        return &second;
    }

private:
    T first;
    Y second;
};

template <class T, class Y>
bool operator==(Pair<T, Y> pair1, Pair<T, Y> pair2)
{
    return (pair1.GetFirst() == pair2.GetFirst() && pair1.GetSecond() == pair2.GetSecond());
}

template <class T, class Y>
bool operator!=(Pair<T, Y> pair1, Pair<T, Y> pair2)
{
    return (pair1.GetFirst() != pair2.GetFirst() || pair1.GetSecond() != pair2.GetSecond());
}

#endif