#include <iostream>
#include <string>
#include <fstream>
#include "Args.h"
#include "CommandsBuilder.h"
#include <filesystem>
using namespace std;

int main(int argc, char* argv[])
{
    string filename{"Emakefile"};
    copy(argv, argv + argc, back_inserter(Args::args));

    ifstream file{filename};
    if(!file.is_open())
        cerr << "could not open file!" << endl;
    
    size_t filesize = filesystem::file_size(filesystem::path{filename});
    string code;
    code.resize(filesize);
    file.read(code.data(), filesize);
    CommandsBuilder cb{move(code)};
    cb.Build();
    
    return 0;
}