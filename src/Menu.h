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

#include "MenuTypes.h"
#include "MenuBase.h"

class Menu : public MenuBase
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
    void run() override;

    /**
     * @brief Imprimir informacion de debug del menu por Serial
     */
    void print_debug_info() override;

    /**
     * @brief Get the Menu Data (useful for display or frontend)
     * 
     * @return MenuData struct with menu title, option titles, option_id, etc
     */
    MenuData getData() override;

    bool enable_option_roll = false; // Al llegar a la opcion final y avanzar, vuelve a la primera

protected:
    char **option_titles = NULL;                        // titulos de cada opcion

private:
    menu_callback_t *option_callbacks = NULL;           // callbacks de cada opcion
    uint8_t current_option = 0;                         // opcion actual
    uint8_t n_options = 0;                              // cantidad de opciones
    menu_action_t exit_action = MENU_ACTION_DO_NOTHING; // accion de salida
    void (*on_exit)(void) = NULL;                       // callback de salida

    uint8_t display_offset = 0;                            // offset de scroll del display

private:
    using MenuBase::menu_debug;
    using MenuBase::menu_set_event_listener_display;
    using MenuBase::menu_set_real_time_loop;
    using MenuBase::menu_set_display_type;
    using MenuBase::menu_go_up;
    using MenuBase::menu_go_down;
    using MenuBase::menu_go_right;
    using MenuBase::menu_go_left;
    using MenuBase::menu_go_select;
    using MenuBase::menu_go_back;
    using MenuBase::menu_is_current_available;
    using MenuBase::menu_force_close_current;
    using MenuBase::menu_get_current_data;
    using MenuBase::menu_get_current_display_data;
    using MenuBase::menu_get_current_display_data_legacy;
};

#endif // _MENU_H_
