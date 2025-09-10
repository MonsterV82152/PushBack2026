#ifndef LIMELIB_PID_HPP
#define LIMELIB_PID_HPP

#include "limelib/types.hpp"

namespace limelib
{
    template <typename T>
    constexpr T sgn(T value) { return value < 0 ? -1 : 1; }
    class PID
    {
    public:
        PID(real_t kP, real_t kI, real_t kD, real_t windupRange = 0, bool signFlipReset = false);
        real_t update(real_t error);
        void reset();

    protected:
        const real_t kP;
        const real_t kI;
        const real_t kD;

        const real_t windupRange;
        const bool signFlipReset;

        real_t integral = 0;
        real_t prevError = 0;
    };
}

#endif
