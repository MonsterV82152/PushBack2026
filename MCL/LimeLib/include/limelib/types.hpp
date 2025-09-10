#pragma once

#ifndef LIMELIB_TYPES_HPP
#define LIMELIB_TYPES_HPP
#include <vector>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <random>
#include <memory>

#define M_PI 3.1415926535
#define MM_TO_IN 0.0393701

namespace limelib
{
    using real_t = float; // float for memory-constrained systems
    real_t getRandomReal_t(real_t lower, real_t upper); // Declaration only
}

#endif