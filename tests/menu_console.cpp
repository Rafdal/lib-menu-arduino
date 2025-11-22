
#define DEBUG_PRINT_BUFFER_SIZE 5
#include "menu_console.h"

#include "MessageBox.h"

Menu mainMenu(4, "Console Menu");

void update_display();

int main()
{
    menu_set_display_type(MENU_DISPLAY_TYPE_LCD_20x4);
    menu_set_event_listener_display(keyboard_listener, update_display);
    menu_debug(false);

    mainMenu.set_option(0, "Show message", [](){
        Serial.println(F("[Option] Showing a short message"));
        delay(400);
    });
    mainMenu.set_option(1, "Toggle wrap", [](){
        mainMenu.enable_option_roll = !mainMenu.enable_option_roll;
        Serial.println(mainMenu.enable_option_roll ?
            F("[Option] Wrap navigation enabled") :
            F("[Option] Wrap navigation disabled"));
        delay(400);
    });
    mainMenu.set_option(2, "Test", [](){
        MessageBox msgBox("This is a test of   the MessageBox class");
        msgBox.exec();
    });
    mainMenu.set_option(3, "Exit", menu_force_close_current);

    Serial.println(F("Console menu ready. Press any supported key to interact."));
    mainMenu.run();

    Serial.println(F("Menu finished. Bye!"));
    return 0;
}


void update_display()
{
    clear_console();
    
    std::cout << "Use WASD or Arrows to move, Enter/Space to select, Q or ESC to go back" << "\n\n";

    const DisplayData data = menu_get_current_display_data();
    for (uint8_t i = 0; i < data.n_lines; ++i) {
        std::cout << data.lines[i] << std::endl;
    }
    
    std::cout << std::endl;

    debug.printAll();
}