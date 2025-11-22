# lib-Menu

Esta libreria permite crear diferentes menues y poder controlarlos           
facilmente, navegar entre sus opciones, crear sub-menues y estructuras       
complejas de menues dentro de otros menues con total libertad                
                                                                             
Tambien permite acceder con facilidad al los datos del menu actual para poder
mostrarlos por pantalla (lista de opciones, opcion seleccionada, titulo, etc)
                                                                             
Esta libreria NO cumple funciones de frontend, el programador decide como    
conectar esta libreria con el frontend y que interfaz usara                  

## PC console test harness

Se agrego un entorno de pruebas para escritorio basado en CMake que permite
ejecutar y depurar la libreria sin depender de un microcontrolador. El flujo
basico es el siguiente:

1. Configurar CMake en modo debug: `cmake --preset ninja-debug`
2. Compilar el ejecutable: `cmake --build --preset ninja-debug`
3. Ejecutar la prueba interactiva: `build/ninja-debug/menu_console`

Para depurar paso a paso desde VS Code basta con seleccionar el preset
`ninja-debug` dentro de la extension *CMake Tools* y usar la accion *Debug* sobre
el objetivo `menu_console`. El modo *Release* (`ninja-release`) esta disponible
con los mismos comandos de arriba.

### Controles del ejecutable `menu_console`

- `W` / `S` o flechas: navegar entre opciones.
- `Enter` o barra espaciadora: seleccionar la opcion.
- `Q` o `ESC`: volver o cerrar el menu.

Detras de escena se utiliza el stub `pc-arduino-stub/Arduino.*` para simular las
funciones basicas de Arduino (Serial, millis, delay, etc.). Si en algun momento
se compila la libreria contra un toolchain real de Arduino basta con desactivar
la opcion `LIB_MENU_USE_PC_STUB` al configurar CMake y proveer la ubicacion del
`Arduino.h` real.
