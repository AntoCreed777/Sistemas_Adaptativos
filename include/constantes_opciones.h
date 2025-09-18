#pragma once

// Versiones abreviadas de los flags
constexpr const char* OPT_ALGORITMO_SHORT = "a";
constexpr const char* OPT_INSTANCIA_SHORT = "i";
constexpr const char* OPT_LONGITUD_LISTA_SHORT = "l";
constexpr const char* OPT_ALPHA_SHORT = "p";
constexpr const char* OPT_HELP_SHORT = "h";

// Versiones largas de los flags
constexpr const char* OPT_ALGORITMO = "algoritmo";
constexpr const char* OPT_INSTANCIA = "instancia";
constexpr const char* OPT_LONGITUD_LISTA = "longitud-lista";
constexpr const char* OPT_ALPHA = "alpha";
constexpr const char* OPT_HELP = "help";

// Versiones combinadas (para cxxopts)
constexpr const char* OPT_ALGORITMO_COMBINED = "a,algoritmo";
constexpr const char* OPT_INSTANCIA_COMBINED = "i,instancia";
constexpr const char* OPT_LONGITUD_LISTA_COMBINED = "l,longitud-lista";
constexpr const char* OPT_ALPHA_COMBINED = "p,alpha";
constexpr const char* OPT_HELP_COMBINED = "h,help";

// Descripciones de las flags
constexpr const char* DESC_ALGORITMO = "Algoritmo a ejecutar (Greedy o Greedy-probabilista) [OBLIGATORIA]";
constexpr const char* DESC_INSTANCIA = "Ruta al archivo de la instancia del problema [OBLIGATORIA]";
constexpr const char* DESC_LONGITUD_LISTA = "Longitud de la lista de selección aleatoria (solo para Greedy-probabilista) [opcional]";
constexpr const char* DESC_ALPHA = "Parámetro alpha (solo para Greedy-probabilista) [opcional]";
constexpr const char* DESC_HELP = "Muestra la ayuda";

// Valores por defecto
constexpr const int DEFAULT_LONGITUD_LISTA = -1;    // De forma logica esto representara que es INF o indeterminado
constexpr const float DEFAULT_ALPHA = 0.2;

// Nombres algoritmos
constexpr const char* GREEDY = "Greedy";
constexpr const char* GREEDY_RANDOM = "Greedy-random";
