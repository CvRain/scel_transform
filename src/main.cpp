#include <iostream>
#include "scel_transformer.hpp"

int main(const int argc, const char** argv) {
    if(argc <= 1) {
        std::cout << "The input parameters are too few. Need at least two parameters" << std::endl;
        std::cout << "scel_format <source_dir | source_file> <dist_dir>" << std::endl;
    }
    ScelTransformer transformer{argv[1], argv[2]};

    return 0;
}
