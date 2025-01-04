
#include "FileUtils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

// Reads the contents of a file and returns it as a string
std::string readFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cerr << "Error reading file: " << fileName << std::endl;
        std::exit(1);  // Exit if the file can't be opened
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
