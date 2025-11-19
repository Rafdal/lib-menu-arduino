#include "MenuBase.h"

const char* MenuBase::NULL_STR = "(null)";
menu_callback_t MenuBase::event_listener = NULL; // Callback lector de nuevos eventos
menu_callback_t MenuBase::update_display = NULL; // Callback para actualizar datos de interfaz de usuario

menu_display_type_t MenuBase::display_type = MENU_DISPLAY_TYPE_CUSTOM; // Tipo de display actual
char** MenuBase::display_lines = NULL; // Lineas de texto a mostrar en el display
uint8_t MenuBase::display_max_lines = 0; // Cantidad maxima de lineas del display
uint8_t MenuBase::display_line_width = 0; // Ancho de cada linea del display
bool MenuBase::display_show_title = true; // Bloquear la primera linea para el titulo del menu

menu_callback_t MenuBase::realTimeLoop = NULL; // Callback en tiempo real (OPCIONAL)

MenuBase *MenuBase::current_menu = NULL;    // Puntero al menu del contexto actual (en ejecucion)

bool MenuBase::debug_enabled = false; // Variable para habilitar/deshabilitar debug

void MenuBase::run(void){
    // Sin los callbacks event_listener y update_display, el menu no funciona!
    if(event_listener != NULL && update_display != NULL){
        state = MENU_STATE_OPEN; // Activar el menu

        while (state != MENU_STATE_CLOSE) // Mientras el menu no se haya cerrado o este cerrado
        {        
            event_listener();                   // escuchar cambios de estado (bajar,seleccionar,cerrar,etc)
            
            if (timeout_ms > 0)
            {
                if (millis() - timeout_start >= timeout_ms)
                {
                    state = next_state; // Cambiar al siguiente estado
                    timeout_ms = 0;     // Desactivar el timeout
                }
            }
            
            fsm();  // Implementacion definida por el usuario

            // Si estamos en un estado de transicion, actualizamos el display
            if(state != MENU_STATE_AVAILABLE && state != MENU_STATE_CLOSE){
                update_display();
                state = MENU_STATE_AVAILABLE;
            }

            if(realTimeLoop != NULL) // Si hay loop en tiempo real, ejecutar
                realTimeLoop();
        }
    }
}

void MenuBase::go_up(void){
    if (state != MENU_STATE_AVAILABLE)
    {
        if (debug_enabled){
            Serial.println(F("MenuBase::go_up() - MENU BUSY") );
            print_debug_info();
        }
        return;
    }
    state = MENU_STATE_UP;
}

void MenuBase::go_down(void){
    if (state != MENU_STATE_AVAILABLE)
    {
        if (debug_enabled){
            Serial.println(F("MenuBase::go_down() - MENU BUSY") );
            print_debug_info();
        }
        return;
    }
    state = MENU_STATE_DOWN;
}

void MenuBase::go_right(void){
    if (state != MENU_STATE_AVAILABLE)
    {
        if (debug_enabled){
            Serial.println(F("MenuBase::go_right() - MENU BUSY") );
            print_debug_info();
        }
        return;
    }
    state = MENU_STATE_RIGHT;
}

void MenuBase::go_left(void){
    if (state != MENU_STATE_AVAILABLE)
    {
        if (debug_enabled){
            Serial.println(F("MenuBase::go_left() - MENU BUSY") );
            print_debug_info();
        }
        return;
    }
    state = MENU_STATE_LEFT;
}

void MenuBase::go_select(void){
    if (state != MENU_STATE_AVAILABLE)
    {
        if (debug_enabled){
            Serial.println(F("MenuBase::go_select() - MENU BUSY") );
            print_debug_info();
        }
        return;
    }
    state = MENU_STATE_SELECT;
}

void MenuBase::go_back(void){
    if (state != MENU_STATE_AVAILABLE)
    {
        if (debug_enabled){
            Serial.println(F("MenuBase::go_back() - MENU BUSY") );
            print_debug_info();
        }
        return;
    }
    state = MENU_STATE_BACK;
}

void MenuBase::menu_debug(bool enable) {
    debug_enabled = enable;
}

void MenuBase::print_debug_info() {
    Serial.print(F("MenuBase Debug Info - title: \'"));
    Serial.print(title);
    Serial.print(F("\' | state: "));
    Serial.println(state);
}

void MenuBase::menu_set_display_type(menu_display_type_t _display_type)
{
    display_type = _display_type;
    switch (display_type)
    {
    case MENU_DISPLAY_TYPE_CUSTOM:
        break;
    case MENU_DISPLAY_TYPE_LCD_16x2:
        display_max_lines = 2;
        display_line_width = 16;
        display_lines = new char*[2];
        display_show_title = false;
        for (int i = 0; i < 2; i++) {
            display_lines[i] = new char[17]; // 16 characters + null terminator
            memset(display_lines[i], 0, 17); // Initialize to empty string
        }
        break;
    case MENU_DISPLAY_TYPE_LCD_20x4:
        display_max_lines = 4;
        display_line_width = 20;
        display_lines = new char*[4];
        display_show_title = true;
        for (int i = 0; i < 4; i++) {
            display_lines[i] = new char[21]; // 20 characters + null terminator
            memset(display_lines[i], 0, 21); // Initialize to empty string
        }
        break;
    case MENU_DISPLAY_TYPE_SERIAL:
        break;
    default:
        break;
    }
}

void MenuBase::menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t _update_display){
    event_listener = ev_listener;
    update_display = _update_display;
}

// OPCIONAL. Loop en tiempo real 
void MenuBase::menu_set_real_time_loop(menu_callback_t f)
{
    realTimeLoop = f;
}

void MenuBase::menu_go_up(void){
    current_menu->go_up();
}

void MenuBase::menu_go_down(void){
    current_menu->go_down();
}

void MenuBase::menu_go_right(void){
    current_menu->go_right();
}

void MenuBase::menu_go_left(void){
    current_menu->go_left();
}

void MenuBase::menu_go_select(void){
    current_menu->go_select();
}

void MenuBase::menu_go_back(void){
    current_menu->go_back();
}

MenuData MenuBase::menu_get_current_data(void){
    if(current_menu != NULL){
        return current_menu->getData();
    }
    return MenuData();
}

DisplayData MenuBase::menu_get_current_display_data(void){
    return current_menu->getDisplayData();
}

bool MenuBase::menu_is_current_available(void){
    return current_menu->is_available();
}

void MenuBase::menu_force_close_current(void){
    current_menu->force_close();
}





void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t update_display)
{
    MenuBase::menu_set_event_listener_display(ev_listener, update_display);
}
void menu_debug(bool enable)
{
    MenuBase::menu_debug(enable);
}
void menu_set_real_time_loop(menu_callback_t f)
{
    MenuBase::menu_set_real_time_loop(f);
}
void menu_set_display_type(menu_display_type_t display_type)
{
    MenuBase::menu_set_display_type(display_type);
}
void menu_go_up(void)
{
    MenuBase::menu_go_up();
}
void menu_go_down(void)
{
    MenuBase::menu_go_down();
}
void menu_go_select(void)
{
    MenuBase::menu_go_select();
}
void menu_go_back(void)
{
    MenuBase::menu_go_back();
}
void menu_go_right(void)
{
    MenuBase::menu_go_right();
}
void menu_go_left(void)
{
    MenuBase::menu_go_left();
}
bool menu_is_current_available(void)
{
    return MenuBase::menu_is_current_available();
}
void menu_force_close_current(void)
{
    MenuBase::menu_force_close_current();
}
MenuData menu_get_current_data(void)
{
    return MenuBase::menu_get_current_data();
}
DisplayData menu_get_current_display_data(void)
{
    return MenuBase::menu_get_current_display_data();
}
// DisplayData menu_get_current_display_data_legacy(void)
// {
//     return MenuBase::menu_get_current_display_data_legacy();
// }