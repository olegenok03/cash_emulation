#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "Person.h"

Person::Person()
{
    this->name = "";
    this->surname = "";
    this->age = 0;
}

Person::Person(std::string name, std::string surname, int age)
{
    this->name = name;
    this->surname = surname;
    this->age = age;
}

std::string Person::GetName() const
{
    return name;
}

std::string Person::GetSurname() const
{
    return surname;
}

int Person::GetAge() const
{
    return age;
}

void Person::SetName(std::string name)
{
    this->name = name;
}

void Person::SetSurname(std::string name)
{
    this->surname = surname;
}

void Person::SetAge(int age)
{
    this->age = age;
}

bool operator==(Person p1, Person p2)
{
    return (p1.GetAge() == p2.GetAge() && p1.GetName() == p2.GetName() && p1.GetSurname() == p2.GetSurname());
}

bool operator!=(Person p1, Person p2)
{
    return (p1.GetAge() != p2.GetAge() || p1.GetName() != p2.GetName() || p1.GetSurname() != p2.GetSurname());
}