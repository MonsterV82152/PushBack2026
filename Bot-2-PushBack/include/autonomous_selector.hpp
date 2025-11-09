#ifndef AUTONSELECTOR_HPP
#define AUTONSELECTOR_HPP

#include "main.h"

class button
{
public:
    double x;
    double y;
    double x2;
    double y2;
    std::string nextPage = "none";
    button(double x, double y, double x2, double y2, std::string nextPage = "none");
    bool isPressed(pros::screen_touch_status_s_t touch);
};

struct screenElement
{
    std::string name;
    pros::Color color = pros::Color::black;
    double x;
    double y;
    double x2 = 0;
    double y2 = 0;
    double radius = 0;
    pros::text_format_e_t textFormat = pros::text_format_e_t::E_TEXT_MEDIUM_CENTER;
    std::string text = "";
};

struct autonomousRoute
{
    std::string teamColor;
    std::string name;
    std::string description;
    void (*autonFunction)();
};

class page
{
public:
    std::string name;
    std::vector<screenElement> elements;
    std::vector<button> buttons;
    std::vector<bool> traits;
    page(std::string name, std::vector<bool> traits, std::vector<screenElement> elements = {}, std::vector<button> buttons = {}) : name(name), traits(traits), elements(elements), buttons(buttons) {}
    void addElement(screenElement element);
    void draw();
    std::string checkButtons(pros::screen_touch_status_s_t touch);
};

class AutonSelector
{
public:
    AutonSelector();

    void start();
    void runAuton();
    void setSkillsAuton(autonomousRoute auton);
    void setAutons(std::vector<autonomousRoute> autons);
    bool isRedTeam() const;
    bool isSkills() const;
    void exit();

private:
    std::vector<autonomousRoute> autonomousRoutes;
    bool exitCode = false;
    std::vector<page> pages;
    autonomousRoute autonSkills;
    page currentPage = page(
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

    void drawPage(std::string pageName);
    void handleTouch(pros::screen_touch_status_s_t touch);
    void autonSelectorF();
};

#endif