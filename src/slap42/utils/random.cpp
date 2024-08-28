#include "random.hpp"

#include <cstdlib>

namespace Slap42 {

float RandMToN(float m, float n) {
    return m + (rand() / (RAND_MAX / (n - m)));
}

float RandZeroToOne() {
    return RandMToN(0.0f, 1.0f);
}

}
