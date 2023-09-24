#include <fstream>
#include <string>

#include "FInOut.h"

#include "Person.h"
#include "Cache.h"
#include "Sequence.h"

int SeqOfObjWithCapFromFile(Sequence<Object<std::string, Person>> *seq, const char *filepath)
{
    std::ifstream in;
    in.open(filepath, std::ios_base::in);

    if(in.fail())
        return -1;

    int capacity = 0;
    in >> capacity;
    
    for(int i = 0; i < seq->GetLength(); i++)
    {
        std::string name, surname; 
        int age = 0, references = 0, time = 0;
        in >> name >> surname >> age;
        in >> references;
        in >> time;
        Object<std::string, Person> obj = Object<std::string, Person>(Person(name, surname, age), 
            [](Person person){return person.GetSurname();});
        obj.SetReferences(references);
        obj.SetTime(time);
        seq->Set(i, obj);
    }

    if(!in.eof())
        return -2;

    in.close();
    return capacity;
}

int SeqOfObjWithCapToFile(int capacity, const Sequence<Object<std::string, Person>> *seq, const char *filepath)
{
    std::ofstream out;
    out.open(filepath, std::ios_base::out);

    if(out.fail())
        return -1;

    out << capacity;
    
    for(int i = 0; i < seq->GetLength(); i++)
    {
        Object<std::string, Person> obj = seq->Get(i);
        out << std::endl << obj.GetElement().GetName() << " " << obj.GetElement().GetSurname() << " " << obj.GetElement().GetAge() << std::endl;
        out << obj.GetReferences() << std::endl;
        out << obj.GetTime();
    }

    out.close();
    return 0;    
}

int SeqOfPersonFromFile(Sequence<Person> *seq, const char *filepath)
{
    std::ifstream in;
    in.open(filepath, std::ios_base::in);

    if(in.fail())
        return -1;
    
    for(int i = 0; i < seq->GetLength(); i++)
    {
        std::string name, surname; 
        int age = 0;
        in >> name >> surname >> age;
        seq->Set(i, Person(name, surname, age));
    }

    if(!in.eof())
        return -2;

    in.close();
    return 0;
}

int CountStrings(const char *filepath)
{
    std::ifstream in;
    in.open(filepath, std::ios_base::in);

    if(in.fail())
    {
        return -1;
    }

    std::string str;
    int count = 0;
    while (std::getline(in, str, '\n'))
    {
        count++;
    }

    in.close();
    return count;    
}