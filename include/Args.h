#pragma once
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Args
{
public:
    static inline vector<string> args;

    template<class T>
    static auto Find(T&& argName)
    {
        return find(args.begin(), args.end(), forward<T>(argName));
    }
    template<class T>
    static bool Contains(T&& argName)
    {
        return Find(forward<T>(argName)) != args.end();
    }
};