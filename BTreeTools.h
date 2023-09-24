#ifndef BTREETOOLS
#define BTREETOOLS

#include "BTree.h"


extern const int t_degree;

template <class T, class Y>
void InitBTreeClass()
{
    BTree<T, Y>::InitClass(t_degree);
}

#endif