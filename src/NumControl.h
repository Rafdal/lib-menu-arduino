#ifndef NUMCONTROL_H
#define NUMCONTROL_H

#include <Arduino.h>
#include "MenuTypes.h"
#include "MenuBase.h"

class NumControl : public MenuBase
{
public:
    NumControl(const char* name, int min_val, int max_val) : MenuBase(name), min_val(min_val), max_val(max_val) {};
    ~NumControl(){}

    void on_change(void (*cb)(int)){
        this->on_change_callback = cb;
    }

    void set_units(const char* u){
        units = const_cast<char*>(u);
    }

    inline int get_value() { return val; }
    inline void set_value(int v) { if(v >= min_val && v <= max_val) val = v; }

    DisplayData getDisplayData() override;

    void set_option_labels(const char* opt1, const char* opt2, const char* opt3) {
        options[0] = const_cast<char*>(opt1);
        options[1] = const_cast<char*>(opt2);
        options[2] = const_cast<char*>(opt3);
    }

    void set_option_multipliers(int mul1, int mul2, int mul3) {
        option_multipliers[0] = mul1;
        option_multipliers[1] = mul2;
        option_multipliers[2] = mul3;
    }

private:
    void fsm() override;
    char* units = nullptr;
    int min_val = 0;
    int max_val = 0;
    void (*on_change_callback)(int) = nullptr;
    int val = 0;
    uint8_t current_option = 0;
    uint8_t n_options = 3;
    // const char* options[3] = {"001", "010", "100"};
    // const int option_multipliers[3] = {1, 10, 100};
    char* options[3] = {nullptr, nullptr, nullptr};
    int option_multipliers[3] = {1, 10, 100};

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

#endif // NUMCONTROL_H