#pragma once
#include <iostream>
#include "Args.h"
#include <unordered_map>

using namespace std;

class CommandsBuilder
{
public:
    template<class T>
    explicit CommandsBuilder(T&& code) noexcept : code(forward<T>(code))
    {
    }

    vector<string> Build() const noexcept
    {
        vector<string> res;

        Parse(Lexer());

        return res;
    }

private:
    vector<string> Lexer() const noexcept
    {
        vector<string> lexems;
        string temp;
        bool isValue = false;

        for(size_t i = 0; i < code.size(); ++i)
        {
            if(IsOp(code[i]))
            {
                //обрезание конца строки
                
                for(size_t j = temp.size() - 1; j >= 0; --j)
                {
                    if(IsSpace(temp[j]))
                    {
                        temp.pop_back();
                    }
                    else
                    {
                        break;
                    }
                }
                lexems.push_back(move(temp));
                if(code[i] != ';')
                    lexems.emplace_back(1, code[i]);
                isValue = false;
            }
            else if(IsSpace(code[i]))
            {
                if(isValue)
                {
                    temp.push_back(code[i]);
                }
            }
            else
            {
                isValue = true;
                temp.push_back(code[i]);
            }
        }
        return lexems;
    }
    void Parse(vector<string>&& lexems) const noexcept
    {
        copy(begin(lexems), end(lexems), ostream_iterator<string>{cout, ", "});
        
    }
    constexpr bool IsSpace(char c) const noexcept
    {
        return c == ' ' || c == '\n' || c == '\r' || c == '\t';
    }
    bool IsOp(char c) const noexcept
    {
        for(unsigned char i = 0; i < sizeOps; ++i)
        {
            if(ops[i] == c)return true;
        }
        return false;
    }

    string code;
    unsigned char sizeOps = 5;
    char ops[5]{'{', '}', '=', ',', ';'};
    unordered_map<string, vector<string>> lists;
    unordered_map<string, string> strings;
    unordered_map<string, bool> booleans;
};