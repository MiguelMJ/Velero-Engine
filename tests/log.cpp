#include "loguru.hpp"

int main(int argc, char** argv){
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    LOG_F(INFO, "Hello log file!");
    return 0;;
}
