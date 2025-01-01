#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

std::string read_file_contents(const std::string& filename);
void        processFileContents(const std::string& file_contents, int& ret_val);

int main(int argc, char* argv[])
{
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible
    // when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3)
    {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command != "tokenize")
    {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    std::string file_contents = read_file_contents(argv[2]);

    int ret_val = 0;

    processFileContents(file_contents, ret_val);

    std::cout << "EOF  null" << std::endl;

    return ret_val;
}

std::string read_file_contents(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void processFileContents(const std::string& file_contents, int& ret_val)
{
    // Define the lookup table for single-character tokens
    const std::unordered_map<char, std::string> token_map = {{'(', "LEFT_PAREN ( null"},
                                                             {')', "RIGHT_PAREN ) null"},
                                                             {'{', "LEFT_BRACE { null"},
                                                             {'}', "RIGHT_BRACE } null"},
                                                             {'*', "STAR * null"},
                                                             {'.', "DOT . null"},
                                                             {',', "COMMA , null"},
                                                             {'+', "PLUS + null"},
                                                             {'-', "MINUS - null"},
                                                             {';', "SEMICOLON ; null"},
                                                             {'=', "EQUAL = null"},
                                                             {'!', "BANG ! null"},
                                                             {'<', "LESS < null"},
                                                             {'>', "GREATER > null"}};

    // Define the lookup table for multi-character tokens
    const std::unordered_map<std::string, std::string> multi_token_map = {
        {"==", "EQUAL_EQUAL == null"},
        {"!=", "BANG_EQUAL != null"},
        {"<=", "LESS_EQUAL <= null"},
        {">=", "GREATER_EQUAL >= null"}};

    int i = 0;
    while (i < file_contents.size())
    {
        char c = file_contents[i];

        // Handle multi-character tokens
        if (i + 1 < file_contents.size())
        {
            std::string multi_token = std::string(1, c) + file_contents[i + 1];
            auto        multi_it    = multi_token_map.find(multi_token);
            if (multi_it != multi_token_map.end())
            {
                std::cout << multi_it->second << std::endl;
                i += 2;  // Skip the multi-character token
                continue;
            }
        }

        // Handle single-character tokens
        auto it = token_map.find(c);
        if (it != token_map.end())
        {
            std::cout << it->second << std::endl;
        }
        else
        {
            // Handle unexpected characters
            std::cerr << "[line 1] Error: Unexpected character: " << c << std::endl;
            ret_val = 65;
        }

        i++;
    }
}
