#pragma once
#include <iostream>
#include "Args.h"
#include <unordered_map>
#include "TokenType.h"

using namespace std;

template<class T>
vector<T> operator+(vector<T> one, const vector<T>& two)
{
    one.assign(two.begin(), two.end());
    return one;
}

class CommandsBuilder
{
public:
    template<class T>
    explicit CommandsBuilder(T&& code) noexcept : code(forward<T>(code))
    {
    }

    vector<string> Build() noexcept
    {
        vector<string> commands;
        string temp;

        Parse(Lexer());

        DefaultValues();

        vector<string> objects;
        
        for(auto& item : lists["src"])
        {
            temp = values["compiler"];
            temp += " -std=" + values["std"] + ((values["moduleSupport"] == "true") ? " -fmodules-ts" : "") +" -c " + item + " -o " + values["binary"] + "/";
            string obj = string(item.begin(), item.begin() + item.find_last_of('.')) + ".o ";
            obj = string(obj.begin() + obj.find_last_of('/') + 1, obj.end() - 1);
            temp += obj + values["objArgs"];
            objects.push_back(move(obj));
            commands.push_back(move(temp));
        }

        temp = values["compiler"];
        for(auto& item : objects)
        {
            temp += " " + values["binary"] + "/" + item;
        }
        temp += " -o " + values["binary"] + "/" + values["buildname"];

        commands.push_back(move(temp));
        return commands;
    }

private:
    vector<string> Lexer() const noexcept
    {
        vector<string> lexems;
        string temp;
        bool isValue = false, isOpenQuotes = false, isBackslash = false;

        for(size_t i = 0; i < code.size(); ++i)
        {
            if(IsOp(code[i]) && !isBackslash && !isOpenQuotes)
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
                isBackslash = false;
            }
            else if(IsSpace(code[i]))
            {
                if(isValue)
                {
                    temp.push_back(code[i]);
                }
                isBackslash = false;
            }
            else if(code[i] == '"' && !isBackslash)
            {
                isOpenQuotes = !isOpenQuotes;
                isBackslash = false;
            }
            else if(code[i] == '\\' && !isBackslash && !isOpenQuotes)
            {
                isBackslash = true;
            }
            else
            {
                isValue = true;
                temp.push_back(code[i]);
                isBackslash = false;
            }
        }
        if(!temp.empty())
            lexems.push_back(move(temp));
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
                else if(ToTokenType(lexems[i + 1]) == EQUAL)
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
    void DefaultValues()
    {
        if(!values.contains("compiler"))
            values["compiler"] = "g++";
        if(!values.contains("release"))
            values["release"] = "false";
        if(!values.contains("args"))
            values["args"] = "";
        if(!values.contains("buildname"))
            values["buildname"] = "program";
        if(!values.contains("binary"))
            values["binary"] = "bin";
        if(!values.contains("moduleSupport"))
            values["moduleSupport"] = "false";
        if(!values.contains("std"))
            values["std"] = "c++20";
        if(!values.contains("objArgs"))
            values["objArgs"] = "";

        if(!lists.contains("src"))
            lists["src"] = {};
        if(!lists.contains("include"))
            lists["include"] = {};
    }

private:
    string code;
    unsigned char sizeOps = 5;
    char ops[5]{'{', '}', '=', ',', ';'};
    unordered_map<string, vector<string>> lists;
    unordered_map<string, string> values;
};