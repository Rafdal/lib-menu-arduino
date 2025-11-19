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
void Menu::run(){
    // Sin los callbacks event_listener y update_display, el menu no funciona!
    if(event_listener != NULL && update_display != NULL){

        state = MENU_STATE_OPEN; // Activar el menu

        while (state != MENU_STATE_CLOSE) // Mientras el menu no se haya cerrado o este cerrado
        {        
            event_listener();                   // escuchar cambios de estado (bajar,seleccionar,cerrar,etc)

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

            if(realTimeLoop != NULL) // Si hay loop en tiempo real, ejecutar
                realTimeLoop();

            // Si estamos en un estado de transicion, actualizamos el display
            if(state != MENU_STATE_AVAILABLE && state != MENU_STATE_CLOSE){
                state = MENU_STATE_AVAILABLE;

                if (display_offset > current_option)
                    display_offset = current_option;
                else if (current_option >= display_offset + display_max_lines - (display_show_title ? 1 : 0))
                    display_offset = current_option - display_max_lines + 1 + (display_show_title ? 1 : 0);

                update_display();
            }
        }
        // Serial.println(F("saliendo del menu\n"));
    }
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