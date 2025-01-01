#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string read_file_contents(const std::string &filename);

int main(int argc, char *argv[]) {
  int ret_val = 0;
  // Disable output buffering
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  // You can use print statements as follows for debugging, they'll be visible
  // when running tests.
  std::cerr << "Logs from your program will appear here!" << std::endl;

  if (argc < 3) {
    std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
    return 1;
  }

  const std::string command = argv[1];

  if (command != "tokenize") {
    std::cerr << "Unknown command: " << command << std::endl;
    return 1;
  }

  std::string file_contents = read_file_contents(argv[2]);
  if (file_contents.empty()) {
    std::cout << "EOF  null" << std::endl;
    return 0;
  }
  int i = 0;
  while (i < file_contents.size()) {
    char c = file_contents[i];
    switch (c) {
      case '(':
        std::cout << "LEFT_PAREN ( null" << std::endl;
        i++;
        break;
      case ')':
        std::cout << "RIGHT_PAREN ) null" << std::endl;
        i++;
        break;
      case '{':
        std::cout << "LEFT_BRACE { null" << std::endl;
        i++;
        break;
      case '}':
        std::cout << "RIGHT_BRACE } null" << std::endl;
        i++;
        break;
      case '*':
        std::cout << "STAR * null" << std::endl;
        i++;
        break;
      case '.':
        std::cout << "DOT . null" << std::endl;
        i++;
        break;
      case ',':
        std::cout << "COMMA , null" << std::endl;
        i++;
        break;
      case '+':
        std::cout << "PLUS + null" << std::endl;
        i++;
        break;
      case '-':
        std::cout << "MINUS - null" << std::endl;
        i++;
        break;
      case ';':
        std::cout << "SEMICOLON ; null" << std::endl;
        i++;
        break;
      case '=':
        if (i == file_contents.size() - 1 || file_contents[i + 1] != '=') {
          std::cout << "EQUAL = null" << std::endl;
          i++;
        } else {
          std::cout << "EQUAL_EQUAL == null" << std::endl;
          i += 2;
        }
        break;
      default:
        std::cerr << "[line 1] Error: Unexpected character: " << c << std::endl;
        i++;
        ret_val = 65;
        break;
    }
  }
  std::cout << "EOF  null" << std::endl;
  return ret_val;
}

std::string read_file_contents(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error reading file: " << filename << std::endl;
    std::exit(1);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  return buffer.str();
}
