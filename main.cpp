#include "Context.hpp"

using namespace ge;

int main(int argc, char** argv){
    
    M_AS::addPath("assets");
    M_AS::loadRecursively("assets");
    
    launch();
    return 0;
}
