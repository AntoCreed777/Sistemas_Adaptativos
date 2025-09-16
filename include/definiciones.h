#pragma once

/**
 * @file definiciones.hpp
 * @brief Definiciones y macros útiles.
 * 
 * Incluye tipos de datos, macros para bucles y funciones de optimización de entrada/salida.
 * 
 * @note Se recomienda incluir este archivo en cada codigo.
 */

#include <iostream>
#include <vector>
#include <utility> // Para std::pair
#include <limits>

typedef std::vector<int> vc;
typedef std::pair<int, int> par;

#define rep(i,n) for(int i=0;i<(int)n;i++)
#define repx(i,x,n) for(int i=x;i<(int)n;i++)
#define invrep(i,n) for(int i=n;i>=0;i--)
#define pb push_back
#define imprimir(mensaje) std::cout << mensaje << std::endl

#define INF std::numeric_limits<int>::max()
#define NEG_INF std::numeric_limits<int>::min()

//Constantes para manejar colores
#define ROJO        "\033[0;31m"
#define VERDE       "\033[0;32m"
#define AMARILLO    "\033[0;33m"
#define AZUL        "\033[0;34m"
#define MAGENTA     "\033[1;35m"
#define CIAN        "\033[1;36m"
#define BLANCO      "\033[1;37m"
#define RESET_COLOR "\033[0m"

/**
 * @brief Función para optimizar la entrada/salida
 * 
 * Esta función desactiva la sincronización de C y C++ para mejorar la velocidad de entrada/salida.
 * Además, desactiva el uso de la función std::endl para evitar la sincronización innecesaria.
 * 
 * @note Se recomienda usar esta función al inicio del programa para optimizar el rendimiento.
 * @note En caso de estar testando el código, se recomienda comentar esta función.
 * @warning Esta función puede causar problemas si se mezcla con otras funciones de entrada/salida que dependen de la sincronización.
 */
inline void FunctionOptimization() {
   std::ios_base::sync_with_stdio(false);
   std::cin.tie(0);
}