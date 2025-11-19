#include "MessageBox.h"
#if false

// Constructor
bool MessageBox::MessageBox(const char* message) : MenuBase(message)
{
    return exec();
}

bool MessageBox::exec()
{
    run();
    return selected;
}

void MessageBox::fsm()
{
    switch (state)
    {
    case MENU_STATE_OPEN:
        current_menu = this;            // Setear el menu como menu actual (en foco)
        selected = false;
        break;
    case MENU_STATE_UP:
    case MENU_STATE_DOWN:
    case MENU_STATE_AVAILABLE:
    case MENU_STATE_BACK:
        break;
    case MENU_STATE_RIGHT:
        selected = true;
        break;
    case MENU_STATE_LEFT:
        selected = false;
        break;
    case MENU_STATE_SELECT:
        if(on_select != nullptr){
            on_select(selected);
        }
        break;
    }
}

#endif