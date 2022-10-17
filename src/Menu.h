#ifndef _MENU_H_
#define _MENU_H_

#include <Arduino.h>

/********************************************************************************
 *                              LIBERIA MENU                                    *
 * Esta libreria permite crear diferentes menues y poder controlarlos           *
 * facilmente, navegar entre sus opciones, crear sub-menues y estructuras       *
 * complejas de menues dentro de otros menues con total libertad                *
 *                                                                              *
 * Tambien permite acceder con facilidad al los datos del menu actual para poder*
 * mostrarlos por pantalla (lista de opciones, opcion seleccionada, titulo, etc)*
 *                                                                              *
 * Esta libreria NO cumple funciones de frontend, el programador decide como    *
 * conectar esta libreria con el frontend y que interfaz va a usar              *
 ********************************************************************************
 */

// Acciones del menu al ejecutar menu_go_back()
typedef enum {
    MENU_ACTION_DO_NOTHING, // No hacer nada al ir hacia atras (no salir)
    MENU_ACTION_JUST_EXIT,  // Salir del menu al ir hacia atras
    MENU_ACTION_CALLBACK,   // Ejecutar un callback al ir hacia atras
} menu_action_t;

// Estados del menu
typedef enum {
    MENU_STATE_CLOSE,       // Cerrar menu | menu cerrado
    MENU_STATE_AVAILABLE,   // Menu activo en espera
    MENU_STATE_UP,      // Ir a la opcion superior (estado de transicion)
    MENU_STATE_DOWN,    // Ir a la opcion inferior (estado de transicion)
    MENU_STATE_BACK,    // Ir hacia atras (estado de transicion)
    MENU_STATE_SELECT   // Seleccionar opcion (estado de transicion)
} menu_state_t;

typedef void (*menu_callback_t)(void); // Void Callback

// Datos del menu para mostrar en el display
struct MenuData
{
    MenuData(){ n_options = 0; title = NULL; }
    char **option_titles;   // titulos de cada opcion    
    const char *title;      // Titulo del menu
    uint8_t n_options;      // cantidad de opciones
    uint8_t current_option; // opcion actual
};


class Menu
{
public:
    /**
     * @brief Instanciar menu. Se ejecuta una sola vez al principio del programa.
     * 
     * @param options cantidad de opciones que tendra el menu (numero entero > 0)
     * @param title titulo del menu (c-string)
     * @param on_exit (Opcional) funcion callback que se ejecuta al salir del menu.
     * @param action (Opcional) accion que realiza el menu al ejecutar la funcion "menu_back()"
     * 
     */
    Menu(uint8_t options, const char* title, menu_callback_t on_exit = NULL, 
         menu_action_t action = MENU_ACTION_DO_NOTHING);
    ~Menu();

    /**
     * @brief Configurar una opcion de un menu
     * - EJEMPLO:     menu.set_option(0, "Start", start_callback);
     * 
     * @param option_id numero de opcion que desea configurar (entre 0 y la cant. de opciones que tiene dicho menu)
     * @param title titulo de la opcion (c-string)
     * @param callback funcion callback que se ejecuta al seleccionar la opcion
     */
    void set_option(uint8_t option_id, const char* title, menu_callback_t callback);


// (2) FUNCIONES DE MENU

    /**
     * @brief BLOQUEANTE - ejecuta y abre un menu. El programa queda atrapado dentro del mismo hasta que se cierre
     */
    virtual void run();

    /**
     * @brief Cerrar el menu indicado
     */
    void force_close();

    /**
     * @brief Consultar si el menu esta activo
     * 
     * @retval true - ACTIVO
     * @retval false - INACTIVO
     */
    bool is_available();

    /**
     * @brief Get the menu state (Ignore this)
     * 
     * @return uint8_t menu state
     */
    uint8_t getState() { return state; }

    /**
     * @brief Get the Menu Data (useful for display or frontend)
     * 
     * @return MenuData struct with menu title, option titles, option_id, etc
     */
    MenuData getData();

    // Controles del menu
    void go_up();
    void go_down();
    void go_select();
    void go_back();

    bool enable_option_roll = false; // Al llegar a la opcion final y avanzar, vuelve a la primera

protected:
    char **option_titles = NULL;                        // titulos de cada opcion

private:
    menu_callback_t *option_callbacks = NULL;           // callbacks de cada opcion
    uint8_t current_option = 0;                         // opcion actual
    uint8_t state = MENU_STATE_CLOSE;                   // evento (estado) actual
    const char *title;                                  // Titulo del menu
    uint8_t n_options = 0;                              // cantidad de opciones
    menu_action_t exit_action = MENU_ACTION_DO_NOTHING; // accion de salida
    void (*on_exit)(void) = NULL;                       // callback de salida

};


/**
 * @brief Configurar escucha de eventos de control y funciones de visualizacion del menu
 * - EJEMPLO:     menu_set_event_listener_display(keyboard_read, display_update);
 * 
 * @param ev_listener funcion que se ejecuta en TIEMPO REAL para escuchar nuevos eventos (ej: clicks de teclado)
 *                      Colocar por ejemplo, un callback de lectura del joystick
 * @param update_display funcion que se ejecuta cada vez que se deberia actualizar el display que muestra el menu.
 *                      Dentro de esta funcion se deberia llamar menu_get_current_menu_data() para obtener
 *                      los datos del menu actual y asi poder mostrarlos al usuario.
 */
void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t update_display);

/**
 * @brief (Opcional) Setear un loop en tiempo real
 * 
 * @param f funcion callback
 */
void menu_set_real_time_loop(menu_callback_t f);

// (3) FUNCIONES PARA NAVEGAR EN EL MENU (colocar en un callback asociado a un evento de control. ej: Teclado, Joystick)
void menu_go_up(void);     // Ir a opcion superior
void menu_go_down(void);   // Ir a opcion inferior
void menu_go_select(void); // Seleccionar opcion
void menu_go_back(void);   // Ir hacia atras


// (4) FUNCIONES PARA MANIPULAR EL MENU EN EJECUCION ACTUAL
bool menu_is_current_available(void);      // Retorna true si el menu actual esta activo
void menu_force_close_current(void);       // fuerza el cierre del menu actual
MenuData menu_get_current_data(void);   // Retorna una estructura con datos del menu actual (para mostrar en una UI, etc)

#endif // _MENU_H_
