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
 * @brief Autonomous path for right side starting position
 * Scores in long and top middle goal - 4 + 6 + 3 blocks
 */
void soloAWP();

/**
 * @brief Autonomous path for right side starting position
 * Scores in long and top middle goal - 4 + 6 + 3 blocks
 */
void halfSAWPRight();

/**
 * @brief Autonomous path for right side starting position
 * Scores in long and top middle goal - 4 + 6 + 3 blocks
 */
void halfSAWPLeft();

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
 * @brief Autonomous path for skills challenge
 * Safe route that gets ~80 points
 */
void skills();

/**
 * @brief Drives forward for 500ms at speed 20
 */
void test();

/**
 * @brief Risky autonomous path for skills challenge
 * Risky path that gets ~80-90 points
 */
void riskySkills();

/**
 * @brief Risky autonomous path for skills challenge
 * Risky path that gets ~90-100 points
 */
void riskySkillsV2();

#endif