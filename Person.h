#ifndef PERSON
#define PERSON

#include <string>
#include <iostream>

class Person
{
public:
    Person();

    Person(const Person &p) = default;

    Person(std::string name, std::string surname, int age);

    std::string GetName() const;

    std::string GetSurname() const;

    int GetAge() const;

    void SetName(std::string name);

    void SetSurname(std::string name);

    void SetAge(int age);

private:
    std::string name, surname;
    int age;
};

bool operator==(Person p1, Person p2);

bool operator!=(Person p1, Person p2);

#endif