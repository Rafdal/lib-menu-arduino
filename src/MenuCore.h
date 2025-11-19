#ifndef MENU_CORE_H
#define MENU_CORE_H

#include <Arduino.h>

// namespace MenuCore {
//     typedef void (*menu_callback_t)(void); // Void Callback

//     extern bool debug_enabled;
//     extern const char* NULL_STR;

//     /**
//      * @brief Configurar escucha de eventos de control y funciones de visualizacion del menu
//      * - EJEMPLO:     menu_set_event_listener_display(keyboard_read, display_update);
//      * 
//      * @param ev_listener funcion que se ejecuta en TIEMPO REAL para escuchar nuevos eventos (ej: clicks de teclado)
//      *                      Colocar por ejemplo, un callback de lectura del joystick
//      * @param update_display funcion que se ejecuta cada vez que se deberia actualizar el display que muestra el menu.
//      *                      Dentro de esta funcion se deberia llamar menu_get_current_menu_data() para obtener
//      *                      los datos del menu actual y asi poder mostrarlos al usuario.
//      */
//     void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t update_display);

//     void menu_debug(bool enable); // Habilitar debug por Serial

//     /**
//      * @brief (Opcional) Setear un loop en tiempo real
//      * 
//      * @param f funcion callback
//      */
//     void menu_set_real_time_loop(menu_callback_t f);

//     /**
//      * @brief Setear el tipo de display que se usara para mostrar el menu
//      */
//     void menu_set_display_type(menu_display_type_t display_type);

//     // (3) FUNCIONES PARA NAVEGAR EN EL MENU (colocar en un callback asociado a un evento de control. ej: Teclado, Joystick)
//     void menu_go_up(void);     // Ir a opcion superior
//     void menu_go_down(void);   // Ir a opcion inferior
//     void menu_go_select(void); // Seleccionar opcion
//     void menu_go_back(void);   // Ir hacia atras


//     // (4) FUNCIONES PARA MANIPULAR EL MENU EN EJECUCION ACTUAL
//     bool menu_is_current_available(void);      // Retorna true si el menu actual esta activo
//     void menu_force_close_current(void);       // fuerza el cierre del menu actual
//     MenuData menu_get_current_data(void);   // Retorna una estructura con datos del menu actual (para mostrar en una UI, etc)
//     DisplayData menu_get_current_display_data(void); // Retorna una estructura con datos para mostrar en pantalla
//     DisplayData menu_get_current_display_data_legacy(void); // Retorna una estructura con datos para mostrar en pantalla
// }

#endif // MENU_CORE_H