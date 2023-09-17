#include <iostream>
#include <string>
#include <fstream>
#include "Kronos.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <log_file>" << std::endl;
        return 1;
    }

    std::string log_file = argv[1];

    std::string startRegex = R"(Boot Start)";
    std::string completeRegex = R"(Boot Complete)";
    Kronos kronos(startRegex, completeRegex);

    std::ofstream output_file("output.txt");
    std::streambuf* original_cout_buffer = std::cout.rdbuf(output_file.rdbuf());

    kronos.initLog(log_file);
    kronos.analyzeLog();

    std::cout.rdbuf(original_cout_buffer);

    return 0;
}
