#ifndef MENU_TYPES_H
#define MENU_TYPES_H

#include <Arduino.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define STR_LINE __FILE__ ":" STR(__LINE__) " "

typedef void (*menu_callback_t)(void); // Void Callback

// Acciones del menu al ejecutar menu_go_back()
typedef enum {
    MENU_ACTION_DO_NOTHING, // No hacer nada al ir hacia atras (no salir)
    MENU_ACTION_JUST_EXIT,  // Salir del menu al ir hacia atras
    MENU_ACTION_CALLBACK,   // Ejecutar un callback al ir hacia atras
} menu_action_t;

// Tipos de display soportados
typedef enum {
    MENU_DISPLAY_TYPE_CUSTOM,   // Display personalizado (el usuario se encarga de mostrar los datos)
    MENU_DISPLAY_TYPE_LCD_16x2,  // Display LCD 16x2
    MENU_DISPLAY_TYPE_LCD_20x4,  // Display LCD 20x4
    MENU_DISPLAY_TYPE_SERIAL,   // Display Serial (por consola Serial)
} menu_display_type_t;

// Estados del menu
typedef enum {
    MENU_STATE_CLOSE,       // Cerrar menu | menu cerrado
    MENU_STATE_AVAILABLE,   // Menu activo en espera
    MENU_STATE_UP,      // Ir a la opcion superior (estado de transicion)
    MENU_STATE_DOWN,    // Ir a la opcion inferior (estado de transicion)
    MENU_STATE_LEFT,    // Ir a la opcion izquierda (estado de transicion)
    MENU_STATE_RIGHT,   // Ir a la opcion derecha (estado de transicion)
    MENU_STATE_BACK,    // Ir hacia atras (estado de transicion)
    MENU_STATE_SELECT,   // Seleccionar opcion (estado de transicion)
    MENU_STATE_OPEN,    // Abrir menu (estado de transicion)
} menu_state_t;

// Datos del menu para mostrar en el display
struct MenuData
{
    MenuData(){ n_options = 0; title = NULL; }
    char **option_titles;   // titulos de cada opcion    
    const char *title;      // Titulo del menu
    uint8_t n_options;      // cantidad de opciones
    uint8_t current_option; // opcion actual
    uint8_t display_offset; // offset de scroll del display
};

struct DisplayData
{
    DisplayData(){ n_lines = 0; lines = NULL; }
    char **lines;       // lineas de texto a mostrar
    uint8_t n_lines;    // cantidad de lineas
};

#endif // MENU_TYPES_H