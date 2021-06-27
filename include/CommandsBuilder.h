#pragma once
#include <iostream>
#include "Args.h"
#include <unordered_map>
#include "TokenType.h"

using namespace std;

class CommandsBuilder
{
public:
    template<class T>
    explicit CommandsBuilder(T&& code) noexcept : code(forward<T>(code))
    {
    }

    vector<string> Build() noexcept
    {
        vector<string> res;

        Parse(Lexer());
        for(auto& [key, value] : lists)
        {
            cout << key << ":\n";
            for(auto& item : value)
            {
                cout << item << ", ";
            }
            cout << '\n';
        }
        for(auto& [key, value] : values)
        {
            cout << key << ": " << value << '\n';
        }

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
                //remove blank characters at the end of a line
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
                if(code[i] != ';' && code[i] != ',')
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
    void Parse(vector<string>&& lexems) noexcept
    {
        for(size_t i = 0; i < lexems.size(); ++i)
        {
            using enum TokenType;
            if(ToTokenType(lexems[i]) == NAME)
            {
                if(ToTokenType(lexems[i + 1]) == OPEN_BRACE)
                {
                    string key = move(lexems[i]);
                    i += 2;
                    while(ToTokenType(lexems[i]) != CLOSE_BRACE)
                    {
                        lists[key].push_back(move(lexems[i]));
                        ++i;
                    }
                }
                if(ToTokenType(lexems[i + 1]) == EQUAL)
                {
                    string key = move(lexems[i]);
                    i += 2;
                    values[key] = move(lexems[i]);
                }
            }
        }
    }
    static TokenType ToTokenType(const string& str) noexcept
    {
        if(str == "{") return TokenType::OPEN_BRACE;
        if(str == "}") return TokenType::CLOSE_BRACE;
        if(str == "=") return TokenType::EQUAL;
        return TokenType::NAME;
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
    unordered_map<string, string> values;
};