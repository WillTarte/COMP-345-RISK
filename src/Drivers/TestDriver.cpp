//
// Created by Admin on 11/5/2019.
//
#include <string>
#include <experimental/filesystem>
#include <iostream>
namespace fs = std::experimental::filesystem;

int main() {

    const std::string MAPDIR = "../../references/";
    std::cout << fs::current_path() << std::endl;
    for (const auto & entry : fs::directory_iterator(fs::current_path())) {
        std::cout << entry.path() << std::endl;
    }
     std::cout  << "Hello World";
}
