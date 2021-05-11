#pragma once

/** 
 * @brief Erreur fatale
 * Affiche un message d'erreur et met fin au programme 
 */ 
#define FATAL_ERR(s) { std::cout << "FATAL -- " << s << std::endl; exit(1); }

/**
 * @brief Erreur
 * Affiche un message d'erreur sans quitter le programme
 */ 
#define PRINT_ON_ERR(s) { std::cout << "ERR -- " << s << std::endl; }

/**
 * @brief Assert
 * Si b est évalué à false, affiche un message d'erreur et met fin au programme
 */
#define ASSERT(b, s) { if(!b) {std::cout << "FATAL -- " << s << std::endl; exit(1);} }

/**
 * @brief Check if file exists
 */
inline bool FILE_EXISTS(const std::filesystem::path& p, std::filesystem::file_status s = std::filesystem::file_status{}) {
    return std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(p);
}