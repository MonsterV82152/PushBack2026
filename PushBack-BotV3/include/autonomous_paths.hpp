#ifndef AUTON_HPP
#define AUTON_HPP

#include "globals.hpp"
#include "defines.hpp"

/**
 * @brief Autonomous path for left side starting position
 * Scores in long and top middle goal - 4 + 6 + 3 blocks
 */
void left();
/**
 * @brief Autonomous path for left side starting position
 * Scores in long goal only - 7 blocks
 */
void left2();
/**
 * @brief Autonomous path for left side starting position
 */
void left3();


/**
 * @brief Autonomous path for right side starting position
 * Scores in long and top middle goal - 4 + 6 + 3 blocks
 */
void soloAWP();

/**
 * @brief Autonomous path for right side starting position
 * Scores in long and bottom middle goal - 4 + 3 blocks
 */
void right();
/**
 * @brief Autonomous path for right side starting position
 * Scores in long goal only - 7 blocks
 */
void right2();
/**
 * @brief Autonomous path for right side starting position
 */
void right3();

void move(int speed);

/**
 * @brief Autonomous path for skills challenge
 * Safe route that gets ~80 points
 */
void skills();

/**
 * @brief Autonomous path for skills challenge
 * Aggressive route that gets ~100 points
 */
void skills2();

/**
 * @brief Drives forward for 500ms at speed 20
 */
void test();

#endif