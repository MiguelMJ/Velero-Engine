#include "stringmanip.hpp"

namespace ge::util{
    void toLower(std::string& str){
        std::transform(str.begin(), str.end(), str.begin(), 
            [](unsigned char c){return std::tolower(c);}
        );
    }
}
