#include "main.h"

#ifndef PISTON_CPP
#define PISTON_CPP

class Piston {
    private:
        pros::ADIDigitalOut *piston;
        bool state;
    public:
        Piston(pros::ADIDigitalOut *piston)
        :  piston(piston),
            state(false)
        {}
        void init();
        void toggle();
        void setState(bool Istate);
        bool getState() const;

};

#endif