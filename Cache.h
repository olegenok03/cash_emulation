#ifndef CACHE
#define CACHE

#include <iostream>

#include "Pair.h"
#include "IDictionary.h"
#include "Sequence.h"

template <class TKey, class TElement>
class Object
{
public:
    Object()
    {
        getKey = NULL;
        references = 0;
        last_reference_time = 0;
    }

    Object(TElement element, TKey (*getKey)(TElement element))
    {
        this->element = element;
        this->getKey = getKey;
        this->references = 0;
        this->last_reference_time = 0;
    }

    Object(const Object<TKey, TElement> &obj)
    {
        this->element = obj.element;
        this->getKey = obj.getKey;
        this->references = obj.references;
        this->last_reference_time = obj.last_reference_time;
    }

    TElement GetElement() const
    {
        return element;
    }

    TKey GetKey() const
    {
        return getKey(element);
    }

    int GetReferences() const
    {
        return references;
    }

    int GetTime() const
    {
        return last_reference_time;
    }

    void SetElement(TElement element)
    {
        this->element = element;
    }

    void SetReferences(int references)
    {
        this->references = references;
    }

    void SetTime(int last_reference_time)
    {
        this->last_reference_time = last_reference_time;
    }

    void Use()
    {
        references++;
        last_reference_time = 0;
    }

    void Tick()
    {
        last_reference_time++;
    }

private:
    TElement element;
    int references, last_reference_time;

    TKey (*getKey)(TElement element);
};

template <class TKey, class TElement>
bool operator==(Object<TKey, TElement> obj1, Object<TKey, TElement> obj2)
{
    return (obj1.GetElement() == obj2.GetElement() && obj1.GetReferences() == obj2.GetReferences() &&
        obj1.GetTime() == obj2.GetTime());
}

template <class TKey, class TElement>
bool operator!=(Object<TKey, TElement> obj1, Object<TKey, TElement> obj2)
{
    return (obj1.GetElement() != obj2.GetElement() || obj1.GetReferences() != obj2.GetReferences() ||
        obj1.GetTime() != obj2.GetTime());
}

template <class TKey, class TElement>
class Storage
{
public:
    Storage()
    {
        dict = NULL;
    }

    Storage(IDictionary<TKey, Object<TKey, TElement>> *storage_sample, Sequence<TElement> *data, TKey (*getKey)(TElement element))
    {
        dict = storage_sample;
        for(int i = 0; i < data->GetLength(); i++)
        {
            dict->Add(getKey(data->Get(i)), Object<TKey, TElement>(data->Get(i), getKey));
        }
    }

    Storage(Sequence<Object<TKey, TElement>> *storage_seq, IDictionary<TKey, Object<TKey, TElement>> *storage_sample)
    {
        dict = storage_sample;
        for(int i = 0; i < storage_seq->GetLength(); i++)
        {
            dict->Add(storage_seq->Get(i).GetKey(), storage_seq->Get(i));
        }
    }

    Storage(const Storage<TKey, TElement> &storage)
    {
        dict = storage.dict->Copy();
    }

    int GetCount() const
    {
        return dict->GetCount();
    }

    Object<TKey, TElement> *GetPointer(TKey key) const
    {
        return dict->GetPointer(key);
    }

    Sequence<TKey> *GetStorageKeys() const
    {
        return dict->GetKeys();
    }

    Sequence<Object<TKey, TElement>> *GetStorage() const
    {
        return dict->GetElements();
    }

    bool ContainsKey(TKey key) const
    {
        return dict->ContainsKey(key);
    }

    Object<TKey, TElement> Refer(TKey key)
    {
        Sequence<TKey> *keys = GetStorageKeys();
        for(int i = 0; i < GetCount(); i++)
            GetPointer(keys->Get(i))->Tick();
        delete keys;
        
        Object<TKey, TElement> *obj = GetPointer(key);
        obj->Use();
        return *obj;
    }

    ~Storage()
    {
        delete dict;
    }

private:
    IDictionary<TKey, Object<TKey, TElement>> *dict;
};



template <class TKey, class TElement>
class Cache
{
public:
    Cache()
    {
        dict = NULL;
        system_storage = NULL;
        minor_obj = NULL;
    }

    Cache(IDictionary<TKey, Object<TKey, TElement> *> *cache_sample, Storage<TKey, TElement> *system_storage)
    {
        dict = cache_sample;
        this->system_storage = system_storage;
        minor_obj = NULL;
    }

    Cache(Sequence<Object<TKey, TElement>> *cache_seq, IDictionary<TKey, Object<TKey, TElement> *> *cache_sample,
        Storage<TKey, TElement> *system_storage)
    {
        dict = cache_sample;
        this->system_storage = system_storage;
        for(int i = 0; i < cache_seq->GetLength(); i++)
        {
            TKey key = cache_seq->Get(i).GetKey();
            dict->Add(key, system_storage->GetPointer(key));
        }
        minor_obj = FindMinorObj();
    }

    Cache(const Cache<TKey, TElement> &cache)
    {
        dict = cache.dict->Copy();
        system_storage = cache.system_storage;
        minor_obj = cache.minor_obj;
    }

    Cache(const Cache<TKey, TElement> &cache, Storage<TKey, TElement> *system_storage)
    {
        Sequence<Object<TKey, TElement>> *cache_seq = cache.GetCache();
        IDictionary<TKey, Object<TKey, TElement> *> *cache_sample = cache.dict->Empty();

        dict = cache_sample;
        for(int i = 0; i < cache_seq->GetLength(); i++)
        {
            TKey key = cache_seq->Get(i).GetKey();
            dict->Add(key, system_storage->GetPointer(key));
        }

        delete cache_seq;

        minor_obj = FindMinorObj();
    }

    int GetCount() const
    {
        return dict->GetCount();
    }

    int GetCapacity() const
    {
        return dict->GetCapacity();
    }

    Object<TKey, TElement> Get(TKey key) const
    {
        return *dict->Get(key);
    }

    Sequence<TKey> *GetCacheKeys() const
    {
        return dict->GetKeys();
    }

    Sequence<Object<TKey, TElement>> *GetCache() const //костылики( тип seq тот же, что у storage
    {
        Sequence<Object<TKey, TElement>> *storage_seq = system_storage->GetStorage();
        Sequence<Object<TKey, TElement>> *res = storage_seq->Empty();
        delete storage_seq;

        Sequence<Object<TKey, TElement> *> *object_pointers = dict->GetElements();
        for(int i = 0; i < GetCount(); i++)
        {
            res->Append(*object_pointers->Get(i));
        }
        delete object_pointers;
        return res;
    }

    bool ContainsKey(TKey key) const
    {
        return dict->ContainsKey(key);
    }

    Object<TKey, TElement> Refer(TKey key)
    {
        Object<TKey, TElement> obj = system_storage->Refer(key);
        minor_obj = FindMinorObj();
        return obj;
    }

    void Rebuild(Object<TKey, TElement> *used_obj)
    {
        if(GetCount() == GetCapacity())
        {
            dict->Remove(minor_obj->GetKey());
        }
        dict->Add(used_obj->GetKey(), used_obj);
        minor_obj = FindMinorObj();
    }

    ~Cache()
    {
        delete dict;
    }

private:
    IDictionary<TKey, Object<TKey, TElement> *> *dict;
    Storage<TKey, TElement> *system_storage;

    Object <TKey, TElement> *minor_obj;
    
    Object <TKey, TElement> *FindMinorObj()
    {
        if(!GetCount())
        {
            return NULL;
        }
        Sequence<Object<TKey, TElement> *> *object_pointers = dict->GetElements();
        Object <TKey, TElement> *obj = object_pointers->Get(0);
        int min_references = obj->GetReferences();
        int max_time = obj->GetTime();
        for(int i = 1; i < GetCount(); i++)
        {
            if((object_pointers->Get(i)->GetReferences() < min_references) ||
                ((object_pointers->Get(i)->GetReferences() == min_references) && (object_pointers->Get(i)->GetTime() > max_time)))
            {
                obj = object_pointers->Get(i);
                min_references = obj->GetReferences();
                max_time = obj->GetTime();
            }
        }
        return obj;
    }
};



template <class TKey, class TElement>
class SystemMemory
{
public:
    SystemMemory()
    {
        system_storage = NULL;
        system_cache = NULL;
    }

    SystemMemory(IDictionary<TKey, Object<TKey, TElement> *> *cache_sample, IDictionary<TKey, Object<TKey, TElement>> *storage_sample,
        Sequence<TElement> *data, TKey (*getKey)(TElement element))
    {
        system_storage = new Storage<TKey, TElement>(storage_sample, data, getKey);
        system_cache = new Cache<TKey, TElement>(cache_sample, system_storage);
    }

    SystemMemory(Sequence<Object<TKey, TElement>> *cache_seq, Sequence<Object<TKey, TElement>> *storage_seq, 
        IDictionary<TKey, Object<TKey, TElement> *> *cache_sample, IDictionary<TKey, Object<TKey, TElement>> *storage_sample)
    {
        system_storage = new Storage<TKey, TElement>(storage_seq, storage_sample);
        system_cache = new Cache<TKey, TElement>(cache_seq, cache_sample, system_storage);        
    }

    SystemMemory(SystemMemory<TKey, TElement> &memory)
    {
        system_storage = new Storage<TKey, TElement>(*memory.system_storage);
        system_cache = new Cache<TKey, TElement>(*memory.system_cache, system_storage);
    }

    int GetCount() const
    {
        return system_storage->GetCount();
    }

    int GetCacheCapacity() const
    {
        return system_cache->GetCapacity();
    }

    int GetCacheCount() const
    {
        return system_cache->GetCount();
    }

    TElement Get(TKey key)
    {
        if(system_cache->ContainsKey(key))
            return system_cache->Refer(key).GetElement();

        TElement element = system_storage->Refer(key).GetElement();
        system_cache->Rebuild(system_storage->GetPointer(key));
        return element;
    }

    bool CacheContainsKey(TKey key) const
    {
        return system_cache->ContainsKey(key);
    }

    bool ContainsKey(TKey key) const
    {
        return system_storage->ContainsKey(key);
    }

    Sequence<Object<TKey, TElement>> *GetCache() const
    {
        return system_cache->GetCache();
    }

    Sequence<Object<TKey, TElement>> *GetStorage() const
    {
        return system_storage->GetStorage();
    }

    ~SystemMemory()
    {
        delete system_cache;
        delete system_storage;
    }

private:
    Cache<TKey, TElement> *system_cache;
    Storage<TKey, TElement> *system_storage;
    
};

#endif