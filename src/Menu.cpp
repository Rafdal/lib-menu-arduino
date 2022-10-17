#include "Menu.h"

static menu_callback_t event_listener = NULL; // Callback lector de nuevos eventos
static menu_callback_t update_display = NULL; // Callback para actualizar datos de interfaz de usuario

static menu_callback_t realTimeLoop = NULL; // Callback en tiempo real (OPCIONAL)

static Menu *current_menu = NULL;    // Puntero al menu del contexto actual (en ejecucion)


// Inicializar un menu
Menu::Menu(uint8_t options, const char* title, menu_callback_t on_exit, menu_action_t action) : title(title)
{
    // Asignar memoria a los arreglos de informacion de las opciones
    option_titles = (char**) calloc(options, sizeof(char*));
    option_callbacks = (menu_callback_t*) calloc(options, sizeof(menu_callback_t));

    // FIXME: Sacar los callocs y pasar a todo a new
    // Si ocurre un error liberar todo (ESTO NO DEBERIA PASAR EN C++)
    if(option_titles == NULL || option_callbacks == NULL){
        free(option_titles);
        free(option_callbacks);
    }else{
        // Asignar los datos
        current_option = 0;
        n_options = options;
        this->on_exit = on_exit;
        exit_action = action;
    }
}

Menu::~Menu()
{
    free(option_titles);
    free(option_callbacks);
}

void Menu::set_option(uint8_t option_id, const char* title, menu_callback_t callback){
    option_titles[option_id] = const_cast<char*>(title);
    option_callbacks[option_id] = callback;
}

// ABRIR Y EJECUTAR UN MENU
void Menu::run(){
    // Sin los callbacks event_listener y update_display, el menu no funciona!
    if(event_listener != NULL && update_display != NULL){

        state = MENU_STATE_AVAILABLE; // Activar el menu
        current_option=0;             // Focusear la primera opcion por defecto

        current_menu = this;                // Setear el menu como menu actual (en foco)
        update_display();                   // Renderizar el menu

        while (state != MENU_STATE_CLOSE) // Mientras el menu no se haya cerrado o este cerrado
        {        
            event_listener();                   // escuchar cambios de estado (bajar,seleccionar,cerrar,etc)

            switch (state)
            {
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
                else{
                    // printf("null Callback!\n");
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
                // printf("Menu Event Error\n");
                break;
            }

            if(realTimeLoop != NULL) // Si hay loop en tiempo real, ejecutar
                realTimeLoop();

            // Si estamos en un estado de transicion, actualizamos el display
            if(state != MENU_STATE_AVAILABLE && state != MENU_STATE_CLOSE){
                state = MENU_STATE_AVAILABLE;
                update_display();
            }
        }
        // Serial.println(F("saliendo del menu\n"));
    }
}

MenuData Menu::getData()
{
    MenuData data;
    data.current_option = current_option;
    data.n_options = n_options;
    data.option_titles = option_titles;
    data.title = title;
    return data;
}

bool Menu::is_available(){
    return (state == MENU_STATE_AVAILABLE); // En los estados intermedios (SELECT, UP, etc) se considera que el menu esta ocupado
}

void Menu::go_up(void){
    state = MENU_STATE_UP;
}

void Menu::go_down(void){
    state = MENU_STATE_DOWN;
}

void Menu::go_select(void){
    state = MENU_STATE_SELECT;
}

void Menu::go_back(void){
    state = MENU_STATE_BACK;
}

void Menu::force_close(){
    state = MENU_STATE_CLOSE;
}

void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display){
    event_listener = ev_listener;
    update_display = _update_display;
}

// OPCIONAL. Loop en tiempo real 
void menu_set_real_time_loop(menu_callback_t f)
{
    realTimeLoop = f;
}

void menu_go_up(void){
    current_menu->go_up();
}

void menu_go_down(void){
    current_menu->go_down();
}

void menu_go_select(void){
    current_menu->go_select();
}

void menu_go_back(void){
    current_menu->go_back();
}

MenuData menu_get_current_data(void){
    if(current_menu != NULL){
        return current_menu->getData();
    }
    return MenuData();
}

bool menu_is_current_available(void){
    return current_menu->is_available();
}

void menu_force_close_current(void){
    current_menu->force_close();
}
