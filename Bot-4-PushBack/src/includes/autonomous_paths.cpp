#include "autonomous_paths.hpp"

void moveTo(double x, double y, int t1, int t2, bool b1, bool b2) {
	chassis.turnToPoint(x, y, t1, {.forwards=b1});
	chassis.moveToPoint(x, y, t2, {.forwards=b2});
}

void left() {
}
void left2() {}
void soloAWP() {}
void right() {
    chassis.setPose(-48, -17, 90);


}
void right2() {}
void skills() {}

void test() {
    chassis.setPose(0, 0, 0);
    moveTo(0, 48, 3000, 3000, true, true);
    moveTo(-48, 48, 3000, 3000, true, true);
    moveTo(-48, 0, 3000, 3000, true, true);
    moveTo(0, 0, 3000, 3000, true, true);    
}