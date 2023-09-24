#include <iostream>
#include <string>

#include "Messages.h"

void Help()
{
    std::cout << "ИМИТАЦИЯ РАБОТЫ КЕШ-ПАМЯТИ" << std::endl
              << std::endl;

    std::cout << "Команды:" << std::endl;
    std::cout << "      CacheModel -help = вывести на экран описание всех поддерживаемых команд" << std::endl;
    std::cout << "      CacheModel -r [имя файла] [размер кеша] = инициализировать кеш и основное хранилище данными из файла" << std::endl;
    std::cout << "      CacheModel -fformat = вывести на экран описание формата файла с входными данными" << std::endl;
    std::cout << "      CacheModel -get [значение ключа] = найти элемент по ключу и вывести его значение на экран" << std::endl;
    std::cout << "      CacheModel -getCache = вывести данные кеша" << std::endl;
    std::cout << "      CacheModel -getStorage = вывести данные хранилища" << std::endl
              << std::endl;
}

void ShowFFormat()
{
    std::cout << "ВХОДНЫЕ ДАННЫЕ" << std::endl
              << std::endl;

    std::cout << "      Файл должен содержать произвольное количество строк с описанием объектов типа Person." << std::endl;
    std::cout << "      Строки должны быть представлены в формате \"[имя] [фамилия] [возраст]\"" << std::endl
              << std::endl;
    std::cout << "      Файлы \"Cash.txt\" и \"Storage.txt\" должны содержать вместимость" << std::endl;
    std::cout << "      (соответственно, кеша и хранилища) и дополнительно кратное трем и, в случае с \"Cash.txt\" -" << std::endl;
    std::cout << "      меньшее утроенной вместимости, а в случае со \"Storage.txt\" - равное утроенной вместимости," << std::endl;
    std::cout << "      количество строк с описанием объектов типа Object, содержащий Person." << std::endl;
    std::cout << "      Строки должны быть представлены в формате: \"[имя] [фамилия] [возраст]\"" << std::endl;
    std::cout << "                                                 \"[кол-во обращений]\"" << std::endl;
    std::cout << "                                                 \"[время, прошедшее после последнего обращения]\"" << std::endl
              << std::endl;
}

void Error1()
{
    std::cout << "  ОШИБКА: Команда введена некорректно. Воспользуйтесь командой \"CacheModel -help\" для ознакомления" << std::endl
              << "        со списком поддерживаемых команд" << std::endl;
}

void Error2()
{
    std::cout << "  ОШИБКА: Некорректно введена вместимость кеша. Она должна быть задана натуральным числом, не превышающим INT_MAX" << std::endl;
}

void Error3(const char *filepath)
{
    std::cout << "  ОШИБКА: Файл \"" << filepath << "\" отсутствует, поврежден или открыт в другой программе" << std::endl;
}

void Error4(const char *filepath)
{
    std::cout << "  ОШИБКА: Данные, введенные в файл \"" << filepath << "\" некорректны. Воспользуйтесь командой \"CacheModel -fformat\"" << std::endl
              << "       для ознакомления с требуемым форматом входных данных" << std::endl;
}

void Error5()
{
    std::cout << "  ОШИБКА: Ключ отсутсвует" << std::endl;
}

void StorIsEmpty()
{
    std::cout << "  ОШИБКА: Хранилище пусто. Воспользуйтесь командой \"CacheModel -r [имя файла] [размер кеша]\" для того," << std::endl
              << "      чтобы его инициализировать" << std::endl;
}

void CacheIsEmpty()
{
    std::cout << "  ОШИБКА: Кеш пуст. Воспользуйтесь командой \"CacheModel -r [имя файла] [размер кеша]\" для того, чтобы" << std::endl
              << "      инициализировать хранилище, и командой \"CacheModel -get [значение ключа]\", чтобы заполнить кеш" << std::endl;
}