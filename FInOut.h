#ifndef FINOUT
#define FINOUT

#include <fstream>
#include <string>

#include "Person.h"
#include "Cache.h"
#include "Sequence.h"

int SeqOfObjWithCapFromFile(Sequence<Object<std::string, Person>> *seq, const char *filepath);

int SeqOfObjWithCapToFile(int capacity, const Sequence<Object<std::string, Person>> *seq, const char *filepath);

int SeqOfPersonFromFile(Sequence<Person> *seq, const char *filepath);

int CountStrings(const char *filepath);

#endif