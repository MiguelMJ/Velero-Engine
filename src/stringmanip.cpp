#include "stringmanip.hpp"

namespace ge{
    void toLower(std::string& str){
        std::transform(str.begin(), str.end(), str.begin(), 
            [](unsigned char c){return std::tolower(c);}
        );
    }
    
    // https://stackoverflow.com/a/347974
    void ltrim(std::string& str){
        size_t startpos = str.find_first_not_of(" \t\n");
        if( std::string::npos != startpos ){
            str = str.substr( startpos );
        }
    }
    // https://stackoverflow.com/a/347974
    void rtrim(std::string& str){
        size_t endpos = str.find_last_not_of(" \t\n");
        size_t startpos = str.find_first_not_of(" \t\n");
        if( std::string::npos != endpos ){
            str = str.substr( 0, endpos+1 );
            str = str.substr( startpos );
        }
        else{
            str.erase(std::remove(std::begin(str), std::end(str), ' '), std::end(str));
        }
    }
    void trim(std::string& str){
        ltrim(str);
        rtrim(str);
    }
}
