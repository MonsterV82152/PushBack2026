/**
 * @file autonomous_selector.hpp
 * @brief Header file for the VEX robotics autonomous routine selector
 * This file defines the classes and structures used to create a touchscreen-based
 * autonomous selector interface for VEX V5 Brain. The selector allows drivers to
 * choose autonomous routines before a match or skills run through an intuitive
 * graphical interface.
 *
 * Key components:
 * - button: Defines interactive touchscreen buttons
 * - screenElement: Visual UI elements (shapes, text, etc.)
 * - autonomousRoute: Structure defining an autonomous routine
 * - page: A complete screen with UI elements and buttons
 * - AutonSelector: Main class managing the selection interface
 */

#ifndef AUTONSELECTOR_HPP
#define AUTONSELECTOR_HPP

#include "main.h"

/**
 * @class button
 * @brief Represents an interactive touchscreen button with rectangular boundaries
 *
 * A button is defined by two coordinate pairs (top-left and bottom-right) and
 * contains a reference to the next page to navigate to when pressed.
 */
class button
{
public:
    double x;                      ///< Top-left X coordinate of the button
    double y;                      ///< Top-left Y coordinate of the button
    double x2;                     ///< Bottom-right X coordinate of the button
    double y2;                     ///< Bottom-right Y coordinate of the button
    std::string nextPage = "none"; ///< Name of the page to navigate to when pressed

    /**
     * @brief Constructor for button
     * @param x Top-left X coordinate
     * @param y Top-left Y coordinate
     * @param x2 Bottom-right X coordinate
     * @param y2 Bottom-right Y coordinate
     * @param nextPage Name of the next page (default: "none")
     */
    button(double x, double y, double x2, double y2, std::string nextPage = "none");

    /**
     * @brief Checks if the button was pressed based on touch coordinates
     * @param touch PROS touch status structure containing touch coordinates
     * @return true if touch is within button boundaries, false otherwise
     */
    bool isPressed(pros::screen_touch_status_s_t touch);
};

/**
 * @struct screenElement
 * @brief Defines a visual element to be drawn on the screen
 *
 * Screen elements can be various shapes and text. Supported types include:
 * - "lineRect": Outlined rectangle
 * - "fillRect": Filled rectangle
 * - "lineCircle": Outlined circle
 * - "fillCircle": Filled circle
 * - "line": Straight line
 * - "text": Text string
 * - "erase": Erase rectangular area
 */
struct screenElement
{
    std::string name;                                                               ///< Type of element ("lineRect", "fillRect", "text", etc.)
    pros::Color color = pros::Color::black;                                         ///< Color of the element
    double x;                                                                       ///< X coordinate (or center X for circles)
    double y;                                                                       ///< Y coordinate (or center Y for circles)
    double x2 = 0;                                                                  ///< Second X coordinate (for rectangles/lines)
    double y2 = 0;                                                                  ///< Second Y coordinate (for rectangles/lines)
    double radius = 0;                                                              ///< Radius (for circles)
    pros::text_format_e_t textFormat = pros::text_format_e_t::E_TEXT_MEDIUM_CENTER; ///< Text formatting options
    std::string text = "";                                                          ///< Text content (for text elements)
};

/**
 * @struct autonomousRoute
 * @brief Defines a single autonomous routine with metadata
 *
 * Contains all information needed to identify, describe, and execute
 * an autonomous routine for competition.
 */
struct autonomousRoute
{
    std::string teamColor;   ///< Alliance color ("red" or "blue")
    std::string name;        ///< Short name of the routine
    std::string description; ///< Detailed description of what the routine does
    void (*autonFunction)(); ///< Function pointer to the autonomous routine
};

/**
 * @class page
 * @brief Represents a single screen/page in the selector interface
 *
 * A page consists of visual elements (rectangles, circles, text) and interactive
 * buttons. Each page also has traits that identify its properties (e.g., red team,
 * skills mode).
 */
class page
{
public:
    std::string name;                    ///< Unique identifier for the page
    std::vector<screenElement> elements; ///< Visual elements to be drawn
    std::vector<button> buttons;         ///< Interactive buttons on the page
    std::vector<bool> traits;            ///< Page properties [isRed, isSkills]

    /**
     * @brief Constructor for page
     * @param name Unique identifier for the page
     * @param traits Vector of boolean traits [isRedTeam, isSkills]
     * @param elements Vector of screen elements to display (default: empty)
     * @param buttons Vector of interactive buttons (default: empty)
     */
    page(std::string name, std::vector<bool> traits, std::vector<screenElement> elements = {}, std::vector<button> buttons = {}) : name(name), traits(traits), elements(elements), buttons(buttons) {}

    /**
     * @brief Adds a screen element to the page
     * @param element The screenElement to add
     */
    void addElement(screenElement element);

    /**
     * @brief Draws all elements on this page to the VEX brain screen
     */
    void draw();

    /**
     * @brief Checks if any button was pressed and returns next page name
     * @param touch PROS touch status structure
     * @return Name of next page to navigate to, or "none" if no button pressed
     */
    std::string checkButtons(pros::screen_touch_status_s_t touch);
};

/**
 * @class AutonSelector
 * @brief Main class for managing the autonomous selector interface
 *
 * This class handles the entire autonomous selection system, including:
 * - Creating and managing multiple pages (home, skills, match, etc.)
 * - Handling touchscreen input and navigation
 * - Storing and executing autonomous routines
 * - Tracking current selection state (team color, skills/match mode)
 *
 * Usage:
 * 1. Create AutonSelector instance
 * 2. Register autonomous routines with setAutons() and setSkillsAuton()
 * 3. Call start() to launch the selector interface
 * 4. Call runAuton() at the start of autonomous period
 */
class AutonSelector
{
public:
    /**
     * @brief Constructor - initializes all pages in the selector interface
     */
    AutonSelector();

    /**
     * @brief Starts the selector as a background PROS task
     *
     * Launches the selector interface in a separate task, allowing it to run
     * in parallel with other initialization code.
     */
    void start();

    /**
     * @brief Executes the selected autonomous routine
     *
     * Should be called at the start of the autonomous period. Determines which
     * routine was selected and runs it.
     */
    void runAuton();

    /**
     * @brief Sets the autonomous routine for skills mode
     * @param auton The autonomousRoute structure for skills autonomous
     */
    void setSkillsAuton(autonomousRoute auton);

    /**
     * @brief Registers all match autonomous routines
     * @param autons Vector of autonomousRoute structures for red and blue alliance routines
     */
    void setAutons(std::vector<autonomousRoute> autons);

    /**
     * @brief Checks if red alliance is selected
     * @return true if red team, false if blue team
     */
    bool isRedTeam() const;

    /**
     * @brief Checks if skills mode is selected
     * @return true if in skills mode, false if in match mode
     */
    bool isSkills() const;

    /**
     * @brief Signals the selector task to exit
     *
     * Sets the exit flag to terminate the selector's main loop.
     */
    void exit();

private:
    std::vector<autonomousRoute> autonomousRoutes; ///< All registered match autonomous routines
    bool exitCode = false;                         ///< Flag to signal task termination
    std::vector<page> pages;                       ///< All pages in the selector interface
    autonomousRoute autonSkills;                   ///< The skills autonomous routine
    page currentPage = page(                       ///< Currently displayed page (defaults to home)
        "home",
        {true, true},
        std::vector<screenElement>{
            screenElement{"lineRect", pros::Color::white, 20, 15, 225, 220},
            screenElement{"lineRect", pros::Color::white, 25, 20, 230, 225},
            screenElement{"lineRect", pros::Color::white, 250, 15, 455, 220},
            screenElement{"lineRect", pros::Color::white, 255, 20, 460, 225},
            screenElement{"text", pros::Color::white, 70, 105, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Skills"},
            screenElement{"text", pros::Color::white, 312, 105, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Match"}},
        std::vector<button>{
            button(20, 15, 225, 220, "skills"),
            button(250, 15, 455, 220, "match")});

    /**
     * @brief Draws a specific page by name and updates currentPage
     * @param pageName Name identifier of the page to draw
     */
    void drawPage(std::string pageName);

    /**
     * @brief Handles touch input and navigates to next page if button pressed
     * @param touch PROS touch status structure
     */
    void handleTouch(pros::screen_touch_status_s_t touch);

    /**
     * @brief Main loop function for the selector interface
     *
     * Continuously polls for touch input, handles navigation, and updates
     * the display. Runs until exit() is called.
     */
    void autonSelectorF();
};

#endif