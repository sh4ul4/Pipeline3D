#pragma once

/** 
 * @brief Erreur fatale
 * Affiche un message d'erreur et met fin au programme 
 */ 
#define FATAL_ERR(s) { std::cout << "FATAL -- " << s << std::endl; exit(1); }

/**
 * Erreur 
 * Affiche un message d'erreur sans quitter le programme
 */ 
#define PRINT_ON_ERR(s) { std::cout << "ERR -- " << s << std::endl; }
