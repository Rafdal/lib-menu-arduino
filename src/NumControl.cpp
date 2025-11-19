#include "NumControl.h"


void NumControl::fsm()
{
    switch (state)
    {
    case MENU_STATE_OPEN:
        current_menu = this;            // Setear el menu como menu actual (en foco)
        break;
    case MENU_STATE_AVAILABLE:
        /* do nothing */
        break;

    case MENU_STATE_RIGHT: // Incrementar valor
        if(val < max_val){
            val += option_multipliers[current_option];
            val = (val > max_val) ? max_val : val;
            val = (val < min_val) ? min_val : val;
            if(on_change_callback != nullptr){
                on_change_callback(val);
            }
        }
        break;
    case MENU_STATE_LEFT: // Decrementar valor
        if(val > min_val){
            val -= option_multipliers[current_option];
            val = (val < min_val) ? min_val : val;
            val = (val > max_val) ? max_val : val;
            if(on_change_callback != nullptr){
                on_change_callback(val);
            }
        }
        break;
    case MENU_STATE_UP:
        if(current_option >0)
            current_option--;
        break;
    case MENU_STATE_DOWN:
        if(current_option < n_options - 1)
            current_option++;
        break;
    case MENU_STATE_SELECT:
        // No implementado en este menu
        break;
    case MENU_STATE_BACK:
        // Cerrar el control numerico
        state = MENU_STATE_CLOSE;
        break;
    }
}

DisplayData NumControl::getDisplayData(void)
{
    DisplayData data;
    uint8_t rows = display_max_lines;
    
    for (uint8_t i = 0; i < rows; i++)
        memset(display_lines[i], 0, display_line_width + 1); // Initialize to empty string

    snprintf(display_lines[0], display_line_width + 1, "%s=%d%s", title, val, (units != nullptr) ? units : "");
    
    for(uint8_t i=0; i<n_options; i++)
    {
        if(current_option == i)
            snprintf(display_lines[i+1], display_line_width + 1, "[-]< [ %s% 2s] >[+]", options[i], (units != nullptr) ? units : "");
        else
            snprintf(display_lines[i+1], display_line_width + 1, "       %s% 2s      ", options[i], (units != nullptr) ? units : "");
    }
    
    data.lines = display_lines;
    data.n_lines = rows;
    return data;
}
