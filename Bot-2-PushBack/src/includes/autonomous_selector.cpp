#include "autonomous_selector.hpp"

button::button(double x, double y, double x2, double y2, std::string nextPage) : x(x), y(y), x2(x2), y2(y2), nextPage(nextPage) {}

bool button::isPressed(pros::screen_touch_status_s_t touch)
{
    if (touch.x >= x && touch.x <= x2 && touch.y >= y && touch.y <= y2)
    {
        return true;
    }
    return false;
}

void page::addElement(screenElement element)
{
    elements.push_back(element);
}

void page::draw()
{
    {
        pros::screen::erase();
        for (auto &element : elements)
        {
            pros::screen::set_pen(element.color);
            if (element.name == "lineRect")
            {
                pros::screen::draw_rect(element.x, element.y, element.x2, element.y2);
            }
            else if (element.name == "lineCircle")
            {
                pros::screen::draw_circle(element.x, element.y, element.radius);
            }
            else if (element.name == "fillRect")
            {
                pros::screen::fill_rect(element.x, element.y, element.x2, element.y2);
            }
            else if (element.name == "fillCircle")
            {
                pros::screen::fill_circle(element.x, element.y, element.radius);
            }
            else if (element.name == "text")
            {
                pros::screen::print(element.textFormat, element.x, element.y, "%s", element.text.c_str());
            }
            else if (element.name == "line")
            {
                pros::screen::draw_line(element.x, element.y, element.x2, element.y2);
            }
            else if (element.name == "erase")
            {
                pros::screen::erase_rect(element.x, element.y, element.x2, element.y2);
            }
        }
    }
}

std::string page::checkButtons(pros::screen_touch_status_s_t touch)
{
    for (auto &button : buttons)
    {
        if (button.isPressed(touch))
        {
            return button.nextPage;
        }
    }
    return "none";
}

AutonSelector::AutonSelector()
{
    pages = {
        page(
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
                button(250, 15, 455, 220, "match")}),
        page(
            "skills",
            {true, true},
            std::vector<screenElement>{
                screenElement{"lineRect", pros::Color::white, 20, 15, 225, 220},
                screenElement{"lineRect", pros::Color::white, 25, 20, 230, 225},
                screenElement{"lineRect", pros::Color::white, 250, 15, 455, 220},
                screenElement{"lineRect", pros::Color::white, 255, 20, 460, 225},
                screenElement{"erase", pros::Color::black, 180, 182, 300, 222},
                screenElement{"lineRect", pros::Color::white, 180, 182, 300, 222},
                screenElement{"text", pros::Color::white, 220, 195, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"},
                screenElement{"text", pros::Color::white, 80, 105, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Auton"},
                screenElement{"text", pros::Color::white, 302, 105, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Driver"}},
            std::vector<button>{
                button(180, 180, 300, 220, "home"),
                button(20, 15, 225, 220, "autonomousSkills"),
                button(250, 15, 455, 220, "driverSkills")}),
        page(
            "match",
            {true, false},
            std::vector<screenElement>{
                screenElement{"fillRect", pros::Color::dark_red, 0, 0, 240, 240},
                screenElement{"fillRect", pros::Color::dark_blue, 240, 0, 480, 240},
                screenElement{"erase", pros::Color::black, 180, 180, 300, 220},
                screenElement{"lineRect", pros::Color::white, 180, 180, 300, 220},
                screenElement{"text", pros::Color::white, 220, 193, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"},
                screenElement{"text", pros::Color::white, 90, 105, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Red"},
                screenElement{"text", pros::Color::white, 322, 105, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Blue"}},
            std::vector<button>{
                button(180, 180, 300, 220, "home"),
                button(0, 0, 240, 240, "redAutonPage"),
                button(240, 0, 480, 240, "blueAutonPage")}),
        page(
            "redAutonPage",
            {true, false},
            std::vector<screenElement>{
                screenElement{"lineRect", pros::Color::white, 10, 10, 235, 77},
                screenElement{"lineRect", pros::Color::white, 10, 87, 235, 153},
                screenElement{"lineRect", pros::Color::white, 10, 163, 235, 230},
                screenElement{"lineRect", pros::Color::white, 245, 10, 470, 77},
                screenElement{"lineRect", pros::Color::white, 245, 87, 470, 153},
                // screenElement{"lineRect", pros::Color::white, 245, 163, 480, 230},
                screenElement{"lineRect", pros::Color::white, 245, 163, 394, 230},
                screenElement{"fillCircle", pros::Color::red, 434, 196, 0, 0, 33},
                screenElement{"text", pros::Color::white, 37, 37, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 37, 114, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 37, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 272, 37, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 272, 114, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 272, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"}},
            std::vector<button>{
                button(245, 163, 394, 230, "match"),
                button(10, 10, 235, 77, ""),
                button(10, 87, 235, 153, ""),
                button(10, 163, 235, 230, ""),
                button(245, 10, 470, 77, ""),
                button(245, 87, 470, 153, ""),
            }),
        page(
            "blueAutonPage",
            {false, false},
            std::vector<screenElement>{
                screenElement{"lineRect", pros::Color::white, 10, 10, 235, 77},
                screenElement{"lineRect", pros::Color::white, 10, 87, 235, 153},
                screenElement{"lineRect", pros::Color::white, 10, 163, 235, 230},
                screenElement{"lineRect", pros::Color::white, 245, 10, 470, 77},
                screenElement{"lineRect", pros::Color::white, 245, 87, 470, 153},
                // screenElement{"lineRect", pros::Color::white, 245, 163, 480, 230},
                screenElement{"lineRect", pros::Color::white, 245, 163, 394, 230},
                screenElement{"fillCircle", pros::Color::blue, 434, 196, 0, 0, 33},
                screenElement{"text", pros::Color::white, 37, 37, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 37, 114, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 37, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 272, 37, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 272, 114, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, ""},
                screenElement{"text", pros::Color::white, 272, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"}},
            std::vector<button>{
                button(245, 163, 394, 230, "match"),
                button(10, 10, 235, 77, ""),
                button(10, 87, 235, 153, ""),
                button(10, 163, 235, 230, ""),
                button(245, 10, 470, 77, ""),
                button(245, 87, 470, 153, ""),
            }),
        page(
            "driverSkills",
            {true, true},
            std::vector<screenElement>{
                screenElement{"lineRect", pros::Color::white, 10, 163, 470, 230},
                screenElement{"text", pros::Color::white, 40, 40, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, "Driver Skills"},
                screenElement{"text", pros::Color::white, 220, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"}},
            std::vector<button>{
                button(10, 163, 470, 230, "skills"),
            })};
}

void AutonSelector::start()
{
    pros::Task autonSelectorTask([&]()
                                 { autonSelectorF(); });
}

void AutonSelector::runAuton()
{
    if (currentPage.name == "autonomousSkills")
    {
        autonSkills.autonFunction();
        return;
    }
    for (autonomousRoute &route : autonomousRoutes)
    {
        if (route.name + "blue" == currentPage.name || route.name + "red" == currentPage.name)
        {
            route.autonFunction();
            return;
        }
    }
}

void AutonSelector::setSkillsAuton(autonomousRoute auton)
{
    autonSkills = auton;
    pages.push_back(page(
        "autonomousSkills",
        {true, true},
        std::vector<screenElement>{
            screenElement{"lineRect", pros::Color::white, 10, 163, 470, 230},
            screenElement{"text", pros::Color::white, 20, 20, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, auton.name},
            screenElement{"text", pros::Color::white, 20, 60, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, auton.description},
            screenElement{"text", pros::Color::white, 220, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"}},
        std::vector<button>{
            button(10, 163, 470, 230, "skills"),
        }));
}

void AutonSelector::setAutons(std::vector<autonomousRoute> autons)
{
    autonomousRoutes = autons;
    double reds = 0;
    double blues = 0;
    for (auto &auton : autons)
    {
        if (auton.teamColor == "red")
        {
            pages[3].elements[7 + reds].text = auton.name;
            pages[3].buttons[reds + 1].nextPage = auton.name + "red";
            reds++;
            pages.push_back(
                page(
                    auton.name + "red",
                    {true, false},
                    std::vector<screenElement>{
                        screenElement{"text", pros::Color::white, 20, 20, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, auton.name},
                        screenElement{"text", pros::Color::white, 20, 60, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, auton.description},
                        screenElement{"fillCircle", pros::Color::red, 434, 196, 0, 0, 33},
                        screenElement{"lineRect", pros::Color::white, 10, 163, 390, 230},
                        screenElement{"text", pros::Color::white, 190, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"}},
                    std::vector<button>{
                        button(10, 163, 394, 230, "redAutonPage")}));
        }
        else if (auton.teamColor == "blue")
        {
            pages[4].elements[7 + blues].text = auton.name;
            pages[4].buttons[blues + 1].nextPage = auton.name + "blue";
            blues++;
            pages.push_back(
                page(
                    auton.name + "blue",
                    {false, false},
                    std::vector<screenElement>{
                        screenElement{"text", pros::Color::white, 20, 20, 0, 0, 0, pros::text_format_e_t::E_TEXT_LARGE_CENTER, auton.name},
                        screenElement{"text", pros::Color::white, 20, 60, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, auton.description},
                        screenElement{"fillCircle", pros::Color::blue, 434, 196, 0, 0, 33},
                        screenElement{"lineRect", pros::Color::white, 10, 163, 390, 230},
                        screenElement{"text", pros::Color::white, 190, 190, 0, 0, 0, pros::text_format_e_t::E_TEXT_MEDIUM_CENTER, "Back"}},
                    std::vector<button>{
                        button(10, 163, 394, 230, "blueAutonPage")}));
        }
    }
}

bool AutonSelector::isRedTeam() const
{
    return currentPage.traits[0];
}

bool AutonSelector::isSkills() const
{
    return currentPage.traits[1];
}

void AutonSelector::exit()
{
    exitCode = true;
}

void AutonSelector::drawPage(std::string pageName)
{
    for (page &screen : pages)
    {
        if (screen.name == pageName)
        {
            screen.draw();
            currentPage = screen;
            return;
        }
    }
}

void AutonSelector::handleTouch(pros::screen_touch_status_s_t touch)
{
    for (auto &screen : pages)
    {
        if (screen.name == currentPage.name)
        {
            std::string nextPage = screen.checkButtons(touch);
            if (nextPage != "none")
            {
                drawPage(nextPage);
                return;
            }
        }
    }
}

void AutonSelector::autonSelectorF()
{
    drawPage("home");
    pros::screen_touch_status_s_t touch;
    bool newTouch = true;
    while (true)
    {
        touch = pros::screen::touch_status();
        if (newTouch && touch.touch_status == pros::E_TOUCH_PRESSED)
        {
            handleTouch(touch);
            newTouch = false;
        }
        else if (touch.touch_status == pros::E_TOUCH_RELEASED)
        {
            newTouch = true;
        }
        if (exitCode)
        {
            break;
        }
        pros::delay(20); // Prevents the task from running too fast
    }
}

