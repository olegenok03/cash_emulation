#include <iostream>
#include <fstream>
#include <string>

#include "Output.h"

#include "Person.h"

int CacheOutput(const char *filepath)
{
    std::ifstream in;
    in.open(filepath, std::ios_base::in);

    if(in.fail())
        return -1;

    std::cout << "Содержимое кеша (файл \"" << filepath << "\"):" << std::endl;
    int count = 1, capacity = 0;

    in >> capacity;
    std::cout << "      Кол-во записей: " << capacity << std::endl << std::endl;

    while (!in.eof())
    {
        std::string name, surname;
        int age, ref_num = 0, last_ref_time = 0;
        in >> name >> surname >> age >> ref_num >> last_ref_time;

        std::cout << "   " << count << ") Имя: " << name << std::endl;
        std::cout << "      Фамилия: " << surname << std::endl;
        std::cout << "      Возраст: " << age << std::endl;
        std::cout << "      Кол-во обращений: " << ref_num << std::endl;
        std::cout << "      Время, прошедшее с момента последнего обращения: " << last_ref_time << std::endl << std::endl;

        count++;
    }

    in.close();
    return 0;
}


int StorageOutput(const char *filepath)
{
    std::ifstream in;
    in.open(filepath, std::ios_base::in);

    if(in.fail())
        return -1;

    std::cout << "Содержимое хранилища (файл \"" << filepath << "\"):" << std::endl;
    int count = 1, capacity = 0;

    in >> capacity;
    std::cout << "      Кол-во записей: " << capacity << std::endl << std::endl;

    while (!in.eof())
    {
        std::string name, surname;
        int age, ref_num = 0, last_ref_time = 0;
        in >> name >> surname >> age >> ref_num >> last_ref_time;

        std::cout << "   " << count << ") Имя: " << name << std::endl;
        std::cout << "      Фамилия: " << surname << std::endl;
        std::cout << "      Возраст: " << age << std::endl;
        std::cout << "      Кол-во обращений: " << ref_num << std::endl;
        std::cout << "      Время, прошедшее с момента последнего обращения: " << last_ref_time << std::endl << std::endl;

        count++;
    }

    in.close();
    return 0;
}

void PersonOutput(Person person)
{
    std::cout << "      Искомый элемент:    " << person.GetName() << " " << person.GetSurname() << ",   возраст: " << person.GetAge() << std::endl;

} 