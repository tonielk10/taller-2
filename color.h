#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

using namespace std;

// Función para habilitar colores en Windows
void habilitarColoresWindows() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif
}

void colorP(int n_color, string texto)
{
    // Habilitar colores en Windows la primera vez
    static bool coloresHabilitados = false;
    if (!coloresHabilitados) {
        habilitarColoresWindows();
        coloresHabilitados = true;
    }

    string color_code;

    switch (n_color) {
        case 1:  color_code = "\033[91m"; break;  // Rojo brillante
        case 2:  color_code = "\033[92m"; break;  // Verde brillante
        case 3:  color_code = "\033[93m"; break;  // Amarillo brillante
        case 4:  color_code = "\033[94m"; break;  // Azul brillante
        case 5:  color_code = "\033[95m"; break;  // Magenta brillante
        case 6:  color_code = "\033[96m"; break;  // Cyan brillante
        case 7:  color_code = "\033[97m"; break;  // Blanco brillante
        case 8:  color_code = "\033[90m"; break;  // Gris
        case 9:  color_code = "\033[34m"; break;  // Azul
        case 10: color_code = "\033[32m"; break;  // Verde
        case 11: color_code = "\033[36m"; break;  // Cyan
        case 12: color_code = "\033[31m"; break;  // Rojo
        case 13: color_code = "\033[35m"; break;  // Magenta
        case 14: color_code = "\033[33m"; break;  // Amarillo
        case 15: color_code = "\033[37m"; break;  // Blanco
        case 30: color_code = "\033[30m"; break;  // Negro
        case 31: color_code = "\033[31m"; break;  // Rojo
        case 32: color_code = "\033[32m"; break;  // Verde
        case 33: color_code = "\033[33m"; break;  // Amarillo
        case 34: color_code = "\033[34m"; break;  // Azul
        case 35: color_code = "\033[35m"; break;  // Magenta
        case 36: color_code = "\033[36m"; break;  // Cyan
        case 37: color_code = "\033[37m"; break;  // Blanco
        default: color_code = "\033[0m"; break;   // Reset
    }
    
    cout << color_code << texto << "\033[0m";
    
    // En Windows, asegurar que se muestre inmediatamente
    #ifdef _WIN32
    cout.flush();
    #endif
    
    cout << endl;
}

void color_siguiente(int n_color, string texto)
{
    colorP(n_color, texto);
}

// Función adicional para texto sin salto de línea
void colorPrint(int n_color, string texto)
{
    static bool coloresHabilitados = false;
    if (!coloresHabilitados) {
        habilitarColoresWindows();
        coloresHabilitados = true;
    }

    string color_code;

    switch (n_color) {
        case 1:  color_code = "\033[91m"; break;  // Rojo brillante
        case 2:  color_code = "\033[92m"; break;  // Verde brillante
        case 3:  color_code = "\033[93m"; break;  // Amarillo brillante
        case 4:  color_code = "\033[94m"; break;  // Azul brillante
        case 5:  color_code = "\033[95m"; break;  // Magenta brillante
        case 6:  color_code = "\033[96m"; break;  // Cyan brillante
        case 7:  color_code = "\033[97m"; break;  // Blanco brillante
        case 8:  color_code = "\033[90m"; break;  // Gris
        case 9:  color_code = "\033[34m"; break;  // Azul
        case 10: color_code = "\033[32m"; break;  // Verde
        case 11: color_code = "\033[36m"; break;  // Cyan
        case 12: color_code = "\033[31m"; break;  // Rojo
        case 13: color_code = "\033[35m"; break;  // Magenta
        case 14: color_code = "\033[33m"; break;  // Amarillo
        case 15: color_code = "\033[37m"; break;  // Blanco
        case 30: color_code = "\033[30m"; break;  // Negro
        case 31: color_code = "\033[31m"; break;  // Rojo
        case 32: color_code = "\033[32m"; break;  // Verde
        case 33: color_code = "\033[33m"; break;  // Amarillo
        case 34: color_code = "\033[34m"; break;  // Azul
        case 35: color_code = "\033[35m"; break;  // Magenta
        case 36: color_code = "\033[36m"; break;  // Cyan
        case 37: color_code = "\033[37m"; break;  // Blanco
        default: color_code = "\033[0m"; break;   // Reset
    }
    
    cout << color_code << texto << "\033[0m";
    
    #ifdef _WIN32
    cout.flush();
    #endif
}

//creador GHOTER_PROM
//Mejorado para compatibilidad multiplataforma

#endif // COLOR_H
