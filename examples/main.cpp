#include <Arduino.h>

#include <Menu.h>

/*      
 *         !! NOTA IMPORTANTE !!    
 *  Este ejemplo no tiene implementado ningun codigo funcional para leer botones
 *  solo contiene ejemplos de como funciona la logica de esta libreria
 *  
 *  - Tampoco estan definidas las funciones de callbacks de opciones
*/

// 4 OPCIONES, TITULO: "Main menu"
Menu main_menu(4, "Main menu");

// Funciones de control de menues
void menu_read_buttons();
void menu_show_display();

// Callbacks de opciones de menu (SIN DEFINIR)
void opcion_0_callback();
void opcion_1_callback();
void opcion_2_callback();
void opcion_3_callback();

void setup()
{
    // Esto funciona para todos los menues que se creen
    menu_set_event_listener_display(menu_read_buttons, menu_show_display);

    main_menu.set_option(0, "Opcion 0", opcion_0_callback);
    main_menu.set_option(1, "Opcion 1", opcion_1_callback);
    main_menu.set_option(2, "Opcion 2", opcion_2_callback);
    main_menu.set_option(3, "Opcion 3", opcion_3_callback);
}

void loop()
{
    main_menu.run(); // Ejecutar menu principal (bloqueante)
}

void menu_read_buttons()
{
    // las funciones sirven para cualquier menu en ejecucion
    // OJO:  EL CODIGO DE LOS BOTONES NO ESTA IMPLEMENTADO
    if(btnUp.pressed())
        menu_go_up();

    if(btnDown.pressed())
        menu_go_down();

    if(btnSelect.pressed())
        menu_go_select();

    if(btnBack.pressed())
        menu_go_back();
}

void menu_show_display()
{
    MenuData menuData = menu_get_current_data();

	Serial.print(F("title: "));
	Serial.println(menuData.title);
	for (uint8_t i = 0; i < menuData.n_options; i++)
	{
		if(menuData.current_option == i)
			Serial.print(F(" >"));
		else
			Serial.print(F("  "));
		
		Serial.println( menuData.option_titles[i]);
	}
}