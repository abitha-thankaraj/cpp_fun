#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream file(__FILE__);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    std::cout << code;
    return 0;
}

