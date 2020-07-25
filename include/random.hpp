#include "Random.hpp"

namespace ge{
    // https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_ProcGen_Universe.cpp
    // lines 168 - 192
    uint32_t nProcGen = 0;
    
    uint32_t rnd(){
        nProcGen += 0xe120fc15;
        uint64_t tmp;
        tmp = (uint64_t)nProcGen * 0x4a39b70d;
        uint32_t m1 = (tmp >> 32) ^ tmp;
        tmp = (uint64_t)m1 * 0x12fad5c9;
        uint32_t m2 = (tmp >> 32) ^ tmp;
        return m2;
    }

    double rndDouble(double min, double max){
        return ((double)rnd() / (double)(0xFFFFFFFF)) * (max - min) + min;
    }
    int rndInt(int min, int max){
        return (rnd() % (max - min)) + min;
    }
    void seed(uint32_t s){
        nProcGen = s;
    }
}
