#include <string>

#include "HashTable.h"
#include "HashTableTools.h"

const double max_load = 0.7;

const double min_load = 0.2;

const double rebuild_coef = 2;

const int simple_hash_coef = 13;

int StringToHash(std::string str_key)
{
    int hash = 0, coef = 1;
    for(int i = 0; i < str_key.length(); i++)
    {
        hash += str_key[i] * coef;
        coef *= simple_hash_coef;
    }
    return hash;
}