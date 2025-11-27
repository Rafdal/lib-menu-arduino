#include "SplashScreen.h"
#if false

void SplashScreen::fsm()
{
    switch (state)
    {
    case MENU_STATE_OPEN:
        current_menu = this;            // Setear el menu como menu actual (en foco)
        set_timeout_state_transition(500, MENU_STATE_RENDER);
        break;
    case MENU_STATE_AVAILABLE:
        break;
    case MENU_STATE_RENDER:
        active = true;
        break;

    case MENU_STATE_UP:
    case MENU_STATE_DOWN:
    case MENU_STATE_BACK:
    case MENU_STATE_RIGHT:
    case MENU_STATE_LEFT:
    case MENU_STATE_SELECT:
        state = MENU_STATE_CLOSE;
        break;
    }
}

DisplayData SplashScreen::getDisplayData(void)
{
    DisplayData data;
    uint8_t rows = display_max_lines;
    
    for (uint8_t i = 0; i < rows; i++)
        memset(display_lines[i], 0, display_line_width + 1); // Clear line

    uint8_t total_lines = (strlen(title) / display_line_width) + 1;
    // Si el mensaje es largo, dividir en varias lineas
    for (uint8_t i = 0; i < total_lines && i < rows - 1; i++)
        strncpy(display_lines[i], title + (i * display_line_width), display_line_width);

    data.lines = display_lines;
    data.n_lines = rows;
    return data;
}

#endif