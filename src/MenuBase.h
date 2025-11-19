#ifndef MENU_BASE_H
#define MENU_BASE_H

#include <Arduino.h>

#include "MenuTypes.h"

class MenuBase
{
public:
    MenuBase(const char* title) : title(title) {}
    virtual ~MenuBase() {}

    virtual void run() = 0;

    /**
     * @brief Cerrar el menu indicado
     */
    inline void force_close() { state = MENU_STATE_CLOSE; }

    /**
     * @brief Consultar si el menu esta activo
     * 
     * @retval true - ACTIVO
     * @retval false - INACTIVO
     */
    inline bool is_available() { return (state == MENU_STATE_AVAILABLE); }

    /**
     * @brief Get the menu state (Ignore this)
     * 
     * @return uint8_t menu state
     */
    inline uint8_t getState() { return state; }

    virtual MenuData getData() = 0;

    /**
     * @brief Imprimir informacion de debug del menu por Serial
     */
    virtual void print_debug_info() = 0;

    // Controles del menu
    void go_up();
    void go_down();
    void go_right();
    void go_left();
    void go_select();
    void go_back();

protected:
    uint8_t state = MENU_STATE_CLOSE;                   // evento (estado) actual
    const char *title;                                  // Titulo del menu

protected:
    static const char* NULL_STR;
    static menu_callback_t event_listener; // Callback lector de nuevos eventos
    static menu_callback_t update_display; // Callback para actualizar datos de interfaz de usuario

    static menu_display_type_t display_type; // Tipo de display actual
    static char** display_lines; // Lineas de texto a mostrar en el display
    static uint8_t display_max_lines; // Cantidad maxima de lineas del display
    static uint8_t display_line_width; // Ancho de cada linea del display
    static bool display_show_title; // Bloquear la primera linea para el titulo del menu

    static menu_callback_t realTimeLoop; // Callback en tiempo real (OPCIONAL)

    static MenuBase *current_menu;    // Puntero al menu del contexto actual (en ejecucion)

    static bool debug_enabled; // Variable para habilitar/deshabilitar debug

public:
    
    static void menu_set_event_listener_display(menu_callback_t ev_listener, menu_callback_t update_display);
    static void menu_debug(bool enable);
    static void menu_set_real_time_loop(menu_callback_t f);
    static void menu_set_display_type(menu_display_type_t display_type);
    static void menu_go_up(void);     // Ir a opcion superior
    static void menu_go_down(void);   // Ir a opcion inferior
    static void menu_go_select(void); // Seleccionar opcion
    static void menu_go_back(void);   // Ir hacia atras
    static void menu_go_right(void);  // Ir a la opcion derecha
    static void menu_go_left(void);   // Ir a la opcion izquierda
    static bool menu_is_current_available(void);      // Retorna true si el menu actual esta activo
    static void menu_force_close_current(void);       // fuerza el cierre del menu actual
    static MenuData menu_get_current_data(void);   // Retorna una estructura con datos del menu actual (para mostrar en una UI, etc)
    static DisplayData menu_get_current_display_data(void); // Retorna una estructura con datos para mostrar en pantalla
    static DisplayData menu_get_current_display_data_legacy(void); // Retorna una estructura con datos para mostrar en pantalla
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

void menu_debug(bool enable); // Habilitar debug por Serial

/**
 * @brief (Opcional) Setear un loop en tiempo real
 * 
 * @param f funcion callback
 */
void menu_set_real_time_loop(menu_callback_t f);

/**
 * @brief Setear el tipo de display que se usara para mostrar el menu
 */
void menu_set_display_type(menu_display_type_t display_type);

// (3) FUNCIONES PARA NAVEGAR EN EL MENU (colocar en un callback asociado a un evento de control. ej: Teclado, Joystick)
void menu_go_up(void);     // Ir a opcion superior
void menu_go_down(void);   // Ir a opcion inferior
void menu_go_select(void); // Seleccionar opcion
void menu_go_back(void);   // Ir hacia atras
void menu_go_right(void);  // Ir a la opcion derecha
void menu_go_left(void);   // Ir a la opcion izquierda

// (4) FUNCIONES PARA MANIPULAR EL MENU EN EJECUCION ACTUAL
bool menu_is_current_available(void);      // Retorna true si el menu actual esta activo
void menu_force_close_current(void);       // fuerza el cierre del menu actual
MenuData menu_get_current_data(void);   // Retorna una estructura con datos del menu actual (para mostrar en una UI, etc)
DisplayData menu_get_current_display_data(void); // Retorna una estructura con datos para mostrar en pantalla
DisplayData menu_get_current_display_data_legacy(void); // Retorna una estructura con datos para mostrar en pantalla

#endif // MENU_BASE_H