#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#include "Args.h"
#include "CommandsBuilder.h"

using namespace std;


int main(int argc, char* argv[]) noexcept
{
    string filename{"Emakefile"};
    copy(argv, argv + argc, back_inserter(Args::args));

    ifstream file{filename};
    if(!file.is_open())
        cerr << "could not open file!" << endl;
    
    // Read all file
    size_t filesize = filesystem::file_size(filesystem::path{filename});
    string code;
    code.resize(filesize);
    file.read(code.data(), filesize);

    CommandsBuilder cb{move(code)};
    for(auto& item: cb.Build())
    {
        // Execute commands
        system(item.c_str());
    }
    
    return 0;
}