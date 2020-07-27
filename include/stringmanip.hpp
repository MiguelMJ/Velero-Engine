#ifndef __STRINGMANIP_HPP__
#define __STRINGMANIP_HPP__

#include <string>
#include <algorithm>

namespace ge{ 
    void toLower(std::string& str);
    void ltrim(std::string& str);
    void rtrim(std::string& str);
    void trim(std::string& str);
}
#endif
