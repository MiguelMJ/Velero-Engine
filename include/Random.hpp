#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <cstdint>

namespace ge{
    namespace Random{
        double rndDouble(double min, double max);
        int rndInt(int min, int max);
        void seed(uint32_t);
    }
}

#endif
