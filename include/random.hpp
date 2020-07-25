#ifndef __GE_RANDOM_HPP__
#define __GE_RANDOM_HPP__

#include <stdint.h>

namespace ge{
    /**
     * @brief Generate a random double between two values.
     * @param min Minimum value, included.
     * @param max Maximum value, excluded.
     */
    double rndDouble(double min, double max);
    
    /**
     * @brief Generate a random integer between two values.
     * @param min Minimum value, included.
     * @param max Maximum value, excluded.
     */
    int rndInt(int min, int max);
    
    /**
     * @brief Seed @ref rndDouble and @ref rndInt.
     * @param s The seed.
     */
    void seed(uint32_t s);
}

#endif
