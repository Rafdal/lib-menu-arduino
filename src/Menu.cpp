#include "Menu.h"

// Inicializar un menu
Menu::Menu(uint8_t options, const char* title, menu_callback_t on_exit, menu_action_t action) : MenuBase(title)
{
    // Asignar memoria a los arreglos de informacion de las opciones
    option_titles = new char*[options];
    option_callbacks = new menu_callback_t[options];

    // Asignar los datos
    current_option = 0;
    n_options = options;
    this->on_exit = on_exit;
    exit_action = action;
}

Menu::~Menu()
{
    delete[] option_titles;
    delete[] option_callbacks;
}

void Menu::set_option(uint8_t option_id, const char* title, menu_callback_t callback){
    if (option_id >= n_options)
    {
        if (debug_enabled) {
            Serial.println(F("Menu::set_option() - Opcion invalida"));
            print_debug_info();
        }
        return; // Opcion invalida
    }
    if (title == NULL)
        option_titles[option_id] = const_cast<char*>(NULL_STR);
    else
        option_titles[option_id] = const_cast<char*>(title);
    option_callbacks[option_id] = callback;
}

// ABRIR Y EJECUTAR UN MENU
void Menu::fsm(){
    switch (state)
    {
    case MENU_STATE_OPEN:
        current_menu = this;            // Setear el menu como menu actual (en foco)
        current_option=0;               // Focusear la primera opcion por defecto
        break;
    case MENU_STATE_LEFT:
    case MENU_STATE_RIGHT:
        // No implementado en este menu
        break;
    case MENU_STATE_AVAILABLE:
        /* do nothing */
        break;

    case MENU_STATE_DOWN: // Ir a la opcion inferior
        if(current_option < n_options-1)
            current_option++;
        else if(current_option == (n_options-1) && enable_option_roll)
            current_option = 0;
        break;

    case MENU_STATE_UP: // Ir a la opcion superior
        if(current_option >0)
            current_option--;
        else if( current_option == 0 && enable_option_roll)
            current_option = n_options - 1;
        break;

    case MENU_STATE_SELECT: // Seleccionar la opcion actual
        if(option_callbacks[current_option] != NULL){
            option_callbacks[current_option]();
            current_menu = this; // retomo el control (en caso de que se haya perdido)
        }
        else if(debug_enabled){
            Serial.println(F("Menu::run() - NULL Callback") );
            print_debug_info();
        }
        break;

    case MENU_STATE_BACK:
        switch (exit_action)
        {
        case MENU_ACTION_CALLBACK:
            if(on_exit != NULL)
                on_exit();                 // ejecuto la funcion de salida
            break;
        
        case MENU_ACTION_DO_NOTHING:
            break; // do nothing

        case MENU_ACTION_JUST_EXIT:
        default:
            state = MENU_STATE_CLOSE;    // cierro el menu (por defecto)
            break;
        }
        break;

    default:
        if (debug_enabled){
            Serial.println(F("Menu::run() - INVALID STATE"));
            print_debug_info();
        }
        break;
    }

    if (display_offset > current_option)
        display_offset = current_option;
    else if (current_option >= display_offset + display_max_lines - (display_show_title ? 1 : 0))
        display_offset = current_option - display_max_lines + 1 + (display_show_title ? 1 : 0);
}

void Menu::print_debug_info(){
    Serial.print(F("\ttitle: \'"));
    Serial.print(title);
    Serial.print(F("\' | n_options: "));
    Serial.print(n_options);
    Serial.print(F(" | current_option: "));
    Serial.print(current_option);
    Serial.print(F("=\'"));
    Serial.print(option_titles[current_option]);
    Serial.print(F("\' | state: "));
    Serial.println(state);
    Serial.print(F("\tdisplay_offset: "));
    Serial.println(display_offset);
}


MenuData Menu::getData()
{
    MenuData data;
    data.current_option = current_option;
    data.n_options = n_options;
    data.option_titles = option_titles;
    data.title = title;
    data.display_offset = display_offset;
    return data;
}



DisplayData Menu::getDisplayData(void)
{
    DisplayData displayData;
    uint8_t rows = display_max_lines;
    switch(display_type) {
        case MENU_DISPLAY_TYPE_LCD_16x2:
        case MENU_DISPLAY_TYPE_LCD_20x4:
            break;
        case MENU_DISPLAY_TYPE_CUSTOM:
        case MENU_DISPLAY_TYPE_SERIAL:
        default:
            if (debug_enabled) {
                Serial.println(F("menu_get_current_display_data() - Unsupported display type for automatic display data generation"));
            }
            return DisplayData(); // Unknown type
    }

    for (uint8_t i = 0; i < rows; i++)
        memset(display_lines[i], 0, display_line_width + 1); // Clear line

    if (display_show_title)
        strncpy(display_lines[0], title, display_line_width);

    if (debug_enabled) {
        Serial.println(title);
    }
    uint8_t title_offset = (display_show_title) ? 1 : 0;

    for (uint8_t i = title_offset; i < rows; i++) {
        uint8_t option_index = i - title_offset + display_offset;
        strcat(display_lines[i], (option_index == current_option) ? ">" : " "); // Leading space for selection indicator
        strncat(display_lines[i], option_titles[option_index], display_line_width - 1);
        if (debug_enabled) {
            Serial.println(display_lines[i]);
        }
    }
    displayData.lines = display_lines;
    displayData.n_lines = rows;

    return displayData;
}

/* DisplayData Menu::menu_get_current_display_data_legacy(void)
{
    DisplayData displayData;
    if(current_menu != NULL) {
        MenuData data = current_menu->getData();
        uint8_t rows = display_max_lines;

        switch(display_type) {
            case MENU_DISPLAY_TYPE_LCD_16x2:
            case MENU_DISPLAY_TYPE_LCD_20x4:
                break;
            case MENU_DISPLAY_TYPE_CUSTOM:
            case MENU_DISPLAY_TYPE_SERIAL:
            default:
                if (debug_enabled) {
                    Serial.println(F("menu_get_current_display_data() - Unsupported display type for automatic display data generation"));
                }
                return DisplayData(); // Unknown type
        }

        for (uint8_t i = 0; i < rows; i++)
            memset(display_lines[i], 0, display_line_width + 1); // Clear line

        uint8_t title_offset = (display_show_title) ? 1 : 0;
        if (display_show_title)
            strncpy(display_lines[0], data.title, display_line_width);

        uint8_t offset = (data.current_option > (rows - 1 - title_offset)) ? (data.current_option - (rows - 1 - title_offset)) : 0;
        uint8_t end_option = (data.n_options > (rows - title_offset)) ? (rows - title_offset) : data.n_options;

        if (debug_enabled) {
            Serial.println(data.title);
            Serial.print(F("\toffset: ")); Serial.print(offset);
            Serial.print(F(" | end_option: ")); Serial.print(end_option);
            Serial.print(F(" | current_option: ")); Serial.print(data.current_option);
            Serial.print(F(" | n_options: ")); Serial.println(data.n_options);
            Serial.print(F("\tdisplay_offset: ")); Serial.println(data.display_offset);
        }

        for (uint8_t i = 0; i < end_option; i++) {
            strcat(display_lines[i + title_offset], (i + offset == data.current_option) ? ">" : " "); // Leading space for selection indicator
            strncat(display_lines[i + title_offset], data.option_titles[i + offset], display_line_width - 1);
            if (debug_enabled) {
                Serial.println(display_lines[i + title_offset]);
            }
        }
        displayData.lines = display_lines;
        displayData.n_lines = rows;
    }
    return displayData;
} */