#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Sequence.h"
#include "DynamicArray.h"
#include "ArraySequence.h"
#include "LinkedList.h"
#include "ListSequence.h"

#include "Pair.h"
#include "IDictionary.h"
#include "HashTable.h"
#include "HashTableDictionary.h"
#include "BTree.h"
#include "BTreeDictionary.h"

#include "FInOut.h"
#include "Messages.h"
#include "Output.h"

#include "Person.h"
#include "HashTableTools.h"
#include "BTreeTools.h"

#include "Cache.h"

int main(int argc, char *argv[], bool debug_mode = 0)
{
    /*InitHashTableClass<int, std::string>();
    InitBTreeClass<int, std::string>();

    auto bTreeDict = new BTreeDictionary<int, std::string>();
    bTreeDict->Add(1, "abab");
    std::cout << *bTreeDict->GetElements() << std::endl << *bTreeDict->GetKeys() << std::endl;
    delete bTreeDict;

    auto hashTableDict = new HashTableDictionary<int, std::string>(10, [](int a){return a;});
    hashTableDict->Add(5, "cjkljlk");
    hashTableDict->Add(1, "aghjj");
    std::cout << hashTableDict->GetCount() << std::endl << hashTableDict->GetCapacity() << std::endl;
    std::cout << *hashTableDict->GetKeys() << std::endl << *hashTableDict->GetElements() << std::endl;
    delete hashTableDict;

    std::cout << "Успех\n";
    return 0;*/

    InitHashTableClass<std::string, Object<std::string, Person> *>();
    InitBTreeClass<std::string, Object<std::string, Person>>();
    int view_argc = 2, get_argc = 3, init_argc = 4;

    if (argc == 2 && !strcmp(argv[1], "-help") && !debug_mode) // help
    {
        Help();
    }

    else if (argc == init_argc && !strcmp(argv[1], "-r")) // !help && !get && !view; argv[2], argv[3] - check
    {
        int cache_cap = atoi(argv[3]);
        if (cache_cap == 0)
        {
            if(!debug_mode)
                Error2(); // Messages.h, некорректно введена вместимость кеша
            return 2;
        }

        int num_of_str = CountStrings(argv[2]); // FInOut.h
        if (num_of_str == -1)
        {
            if(!debug_mode)
                Error3(argv[2]); // Messages.h, файл отсутствует или поврежден
            return 3;
        }

        Sequence<Person> *data = new ArraySequence<Person>(num_of_str);
        int read_error = SeqOfPersonFromFile(data, argv[2]); // FInOut.h
        if (read_error == -2)
        {
            if(!debug_mode)
                Error4(argv[2]); // Messages.h, данные введены в файл некорректно
            return 4;
        }

        SystemMemory<std::string, Person> *system_memory =
            new SystemMemory<std::string, Person>(new HashTableDictionaryFixed<std::string, Object<std::string, Person> *>(cache_cap, StringToHash),
                                                  new BTreeDictionary<std::string, Object<std::string, Person>>(), data,
                                                  [](Person person)
                                                  { return person.GetSurname(); });
        delete data;

        Sequence<Object<std::string, Person>> *system_cache = system_memory->GetCache();
        Sequence<Object<std::string, Person>> *system_storage = system_memory->GetStorage();

        int write_error = SeqOfObjWithCapToFile(cache_cap, system_cache, "Cache.txt"); // FInOut.h
        if (write_error == -1)
        {
            if(!debug_mode)
                Error3("Cache.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }

        write_error = SeqOfObjWithCapToFile(num_of_str, system_storage, "Storage.txt"); // FInOut.h
        if (write_error == -1)
        {
            if(!debug_mode)
                Error3("Storage.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }

        delete system_cache;
        delete system_storage;

        delete system_memory;
    }

    else if (argc == 2 && !strcmp(argv[1], "-fformat") && !debug_mode) // fformat
    {
        ShowFFormat();
    }

    else if (argc == get_argc && !strcmp(argv[1], "-get")) // !help && !init && !view; argv[2] - check
    {
        int cache_size = (CountStrings("Cache.txt") - 1) / 3;

        int storage_size = CountStrings("Storage.txt");
        if (storage_size == 0)
        {
            if(!debug_mode)
                StorIsEmpty(); // Messages.h, память пуста
            return 6;
        }
        storage_size /= 3;

        Sequence<Object<std::string, Person>> *cache_seq = new ArraySequence<Object<std::string, Person>>(cache_size);
        Sequence<Object<std::string, Person>> *storage_seq = new ArraySequence<Object<std::string, Person>>(storage_size);

        int cache_cap = SeqOfObjWithCapFromFile(cache_seq, "Cache.txt"); // FInOut.h
        if (cache_cap == -1)
        {
            if(!debug_mode)
                Error3("Cache.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }
        if (cache_cap == -2)
        {
            if(!debug_mode)
                Error4("Cache.txt"); // Messages.h, данные введены в файл некорректно
            return 4;
        }

        int read_error = SeqOfObjWithCapFromFile(storage_seq, "Storage.txt"); // FInOut.h
        if (read_error == -1)
        {
            if(!debug_mode)
                Error3("Storage.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }
        if (read_error == -2)
        {
            if(!debug_mode)
                Error4("Storage.txt"); // Messages.h, данные введены в файл некорректно
            return 4;
        }

        SystemMemory<std::string, Person> *system_memory =
            new SystemMemory<std::string, Person>(cache_seq, storage_seq,
                                                  new HashTableDictionaryFixed<std::string, Object<std::string, Person> *>(cache_cap, StringToHash),
                                                  new BTreeDictionary<std::string, Object<std::string, Person>>());
        delete cache_seq;
        delete storage_seq;

        if (!system_memory->ContainsKey(std::string(argv[get_argc - 1])))
        {
            if(!debug_mode)
                Error5(); // Messages.h, такого ключа не существует
            return 5;
        }

        if (!debug_mode)
        {
            PersonOutput(system_memory->Get(std::string(argv[get_argc - 1])));
            std::cout << std::endl;
        }

        Sequence<Object<std::string, Person>> *system_cache = system_memory->GetCache();
        Sequence<Object<std::string, Person>> *system_storage = system_memory->GetStorage();

        int write_error = SeqOfObjWithCapToFile(cache_cap, system_cache, "Cache.txt"); // FInOut.h
        if (write_error == -1)
        {
            if(!debug_mode)
                Error3("Cache.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }

        write_error = SeqOfObjWithCapToFile(storage_size, system_storage, "Storage.txt"); // FInOut.h
        if (write_error == -1)
        {
            if(!debug_mode)
                Error3("Storage.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }

        delete system_cache;
        delete system_storage;

        delete system_memory;
    }

    else if (argc == view_argc && !strcmp(argv[1], "-getCache")) // getCache
    {
        int cache_size = CountStrings("Cache.txt") - 1;
        if (cache_size == 0 || cache_size == -1 || cache_size == -2)
        {
            if(!debug_mode)
                CacheIsEmpty(); // Messages.h, кеш пуст
            return 7;
        }

        int read_error = CacheOutput("Cache.txt");
        if (read_error == -1)
        {
            if(!debug_mode)
                Error3("Cache.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }
    }

    else if (argc == view_argc && !strcmp(argv[1], "-getStorage")) // getStorage
    {
        int storage_size = CountStrings("Storage.txt") - 1;
        if (storage_size == 0 || storage_size == -1 || storage_size == -2)
        {
            if(!debug_mode)
                StorIsEmpty(); // Messages.h, память пуста
            return 6;
        }

        int read_error = StorageOutput("Storage.txt");
        if (read_error == -1)
        {
            if(!debug_mode)
                Error3("Storage.txt"); // Messages.h, файл отсутствует или поврежден
            return 3;
        }
    }

    else
    {
        if(!debug_mode)
            Error1(); // Messages.h, команда введена некорректно
        return 1;
    }

    if(!debug_mode)
        std::cout << "Неужели без ошибок? (Прикинь)";
    return 0;
}