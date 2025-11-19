#include "MessageBox.h"
#if true

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
        selected = true;
        cancel = false;
        active = false;
        set_timeout_state_transition(500, MENU_STATE_RENDER);
        break;
    case MENU_STATE_UP:
    case MENU_STATE_DOWN:
    case MENU_STATE_AVAILABLE:
        break;
    case MENU_STATE_RENDER:
        active = true;
        break;
    case MENU_STATE_BACK:
        if (!active)
            break;
        cancel = true;
        selected = false;
        state = MENU_STATE_CLOSE;
        break;
    case MENU_STATE_RIGHT:
        if (!active)
            break;
        selected = false;
        break;
    case MENU_STATE_LEFT:
        if (!active)
            break;
        selected = true;
        break;
    case MENU_STATE_SELECT:
        if (!active)
            break;
        if(on_select != nullptr){
            on_select(selected);
        }
        state = MENU_STATE_CLOSE;
        break;
    }
}

DisplayData MessageBox::getDisplayData(void)
{
    DisplayData data;
    uint8_t rows = display_max_lines;
    
    for (uint8_t i = 0; i < rows; i++)
        memset(display_lines[i], 0, display_line_width + 1); // Clear line

    uint8_t total_lines = (strlen(title) / display_line_width) + 1;
    // Si el mensaje es largo, dividir en varias lineas
    for (uint8_t i = 0; i < total_lines && i < rows - 1; i++)
        strncpy(display_lines[i], title + (i * display_line_width), display_line_width);

    if (active)
    {
        if (selected)
        strncpy(display_lines[rows - 1], "[ SI ]    NO  ", display_line_width);
        else
        strncpy(display_lines[rows - 1], "  SI    [ NO ]", display_line_width);
    }

    data.lines = display_lines;
    data.n_lines = rows;
    return data;
}

#endif