// internedStrings.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <set>

class IdmString
{
public:
    static const std::string* Intern(const char*);
    static const std::string* Intern(const std::string&);

protected:
    IdmString() {}
    IdmString(const IdmString&) {}

    static std::set<std::string> strings;
};

const std::string* IdmString::Intern(const char* source)
{
    return Intern(std::string(source));
}

const std::string* IdmString::Intern(const std::string& source)
{
    std::string src(source);
    auto it = strings.find(src);
    if (it == strings.end())
    {
        it = strings.insert(src).first;
    }

    return &(*it);
}

std::set<std::string> IdmString::strings;

// typedef std::function<void(void)> fn;
// 
// void timeit(fn f, size_t loops = 1000000)
// {
//     auto t1 = std::chrono::high_resolution_clock::now();
//     for (size_t i = 0; i < loops; ++i)
//     {
//         f();
//     }
//     auto t2 = std::chrono::high_resolution_clock::now();
//     std::cout << uint64_t((t2 - t1).count()) * 1000 * double(std::chrono::high_resolution_clock::period::num) / double(std::chrono::high_resolution_clock::period::den) << std::endl;
// }

int main()
{
    auto engine = std::mt19937_64();
    auto dist = std::uniform_int<uint32_t>(0, 25);

    std::set<std::string> std_strings;
    std::set<const std::string*> interned_strings;

    for (size_t j = 0; j < 128; ++j)
    {
        char word[9];
        for (size_t i = 0; i < 8; ++i)
        {
            char c = 'a' + dist(engine);
            word[i] = c;
        }
        word[8] = 0;

        std_strings.insert(std::string(word));
        interned_strings.insert(IdmString::Intern(word));
    }

    std::string search;
    const std::string* interned;
    size_t i = 0;
    for (auto entry : std_strings)
    {
        search = entry;
        interned = IdmString::Intern(search);
        if (++i == 64) break;
    }

    for (size_t j = 0; j < 8; ++j)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        size_t count = 0;
        for (size_t i = 0; i < 1000000; ++i)
        {
            if (std_strings.find(search) != std_strings.end()) ++count;
        }
        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout << count << std::endl;
        std::cout << uint64_t((t2 - t1).count()) * 1000 * double(std::chrono::high_resolution_clock::period::num) / double(std::chrono::high_resolution_clock::period::den) << std::endl;
    }

    for (size_t j = 0; j < 8; ++j)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        size_t count = 0;
        for (size_t i = 0; i < 1000000; ++i)
        {
            if (interned_strings.find(interned) != interned_strings.end()) ++count;
        }
        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout << count << std::endl;
        std::cout << uint64_t((t2 - t1).count()) * 1000 * double(std::chrono::high_resolution_clock::period::num) / double(std::chrono::high_resolution_clock::period::den) << std::endl;
    }

//    size_t count = 0;
//    fn find_string = [&search, &std_strings, &count]() { if (std_strings.find(search) != std_strings.end()) ++count; };
//    timeit(find_string);
//    count = 0;
//    fn find_interned = [&interned, &interned_strings, &count]() { if (interned_strings.find(interned) != interned_strings.end()) ++count; };
//    timeit(find_interned);

    return 0;
}
