#ifndef HASHTABLETOOLS
#define HASHTABLETOOLS

#include <string>

#include "HashTable.h"


extern const double max_load;

extern const double min_load;

extern const double rebuild_coef;

int StringToHash(std::string str_key);

template <class T, class Y>
void InitHashTableClass()
{
    HashTable<T, Y>::InitClass(max_load, min_load, rebuild_coef);
}

#endif