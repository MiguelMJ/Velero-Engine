#include "stringmanip.hpp"

namespace ge{
    void toLower(std::string& str){
        std::transform(str.begin(), str.end(), str.begin(), 
            [](unsigned char c){return std::tolower(c);}
        );
    }
    
    // https://stackoverflow.com/a/347974
    void ltrim(std::string& str, std::string totrim){
        size_t startpos = str.find_first_not_of(totrim);
        if( std::string::npos != startpos ){
            str = str.substr( startpos );
        }
    }
    // https://stackoverflow.com/a/347974
    void rtrim(std::string& str, std::string totrim){
        size_t endpos = str.find_last_not_of(totrim);
        size_t startpos = str.find_first_not_of(totrim);
        if( std::string::npos != endpos ){
            str = str.substr( 0, endpos+1 );
            str = str.substr( startpos );
        }
        else{
            str.erase(std::remove(std::begin(str), std::end(str), ' '), std::end(str));
        }
    }
    void trim(std::string& str, std::string totrim){
        ltrim(str, totrim);
        rtrim(str, totrim);
    }
    std::vector<std::string> tokenize(const std::string& str, char delim){
        std::vector<std::string> ret;
        ret.reserve(std::count(str.begin(), str.end(), delim)+1);
        std::stringstream ss(str);
        std::string token;
        while(getline(ss, token, delim)){
            ret.push_back(token);
        }
        return ret;
    }
    std::unordered_map<std::string, std::string> parseMap (const std::string& str, char pair_del, char kv_del, bool dotrim){
        std::unordered_map<std::string, std::string> ret;
        auto pairs = tokenize(str, pair_del);
        for(auto& pair: pairs){
            std::string key, val;
            std::stringstream ss(pair);
            getline(ss, key, kv_del);
            getline(ss, val);
            if(dotrim){
                trim(key);
                trim(val);
            }
            ret[key]=val;
        }
        return ret;
    }
}
