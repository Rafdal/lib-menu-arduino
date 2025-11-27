#define SPLASHSCREEN_H
#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <Arduino.h>
#include <Print.h>
#include "MenuTypes.h"
#include "MenuBase.h"

class SplashScreen : public MenuBase
{
public:
    SplashScreen(const char* message) : MenuBase(message) {};
    ~SplashScreen(){}

    DisplayData getDisplayData() override;

private:
    void fsm() override;

private:
    using MenuBase::menu_debug;
    using MenuBase::menu_set_event_listener_display;
    using MenuBase::menu_set_real_time_loop;
    using MenuBase::menu_set_display_type;
    using MenuBase::menu_go_up;
    using MenuBase::menu_go_down;
    using MenuBase::menu_go_right;
    using MenuBase::menu_go_left;
    using MenuBase::menu_go_select;
    using MenuBase::menu_go_back;
    using MenuBase::menu_is_current_available;
    using MenuBase::menu_force_close_current;
    using MenuBase::menu_get_current_data;
    using MenuBase::menu_get_current_display_data;
    // using MenuBase::menu_get_current_display_data_legacy;
};

#endif // SPLASHSCREEN_H