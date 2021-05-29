#pragma once
/**
 * @file ErrorsHandler.hpp
 * @brief Interaction Utilisateur : Contient les différentes gestions d'erreur de chemins de fichier
 */

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

/**
 * @brief Return a corrected path to a file
 */
std::string FIND_FILE(std::string file) {
    if (file.length() < 1)              FATAL_ERR("invalid string given as path : " + file);
    if (FILE_EXISTS("app/"+file))       return "app/"+file;
    if (FILE_EXISTS("../app/"+file))    return "../app/"+file;
    for (int i = 0; i < 3; i++) {
        if (FILE_EXISTS(file))          return file;
        if(file[0] == '.' || file[0] == '/') file = "." + file;
        else file = "./" + file;
    }
    FATAL_ERR("invalid string given as path : " + file);
}

std::string FIND_FONT(std::string file)  {
    if (file.length() < 1)  FATAL_ERR("Invalid string given as fontpath : " + file);
    if (FILE_EXISTS("app/fonts/"+file)) return "app/fonts/"+file;
    else if (FILE_EXISTS("../app/fonts/"+file)) return "../app/fonts/"+file;
    else FATAL_ERR("Unable to find font: " + file);
}


std::string FIND_FILE_BUT_DONT_LEAVE(std::string file) {
    if (file.length() > 1)  {
        for (int i = 0; i < 3; i++) {
            if (FILE_EXISTS(file)) return file;
            if(file[0] == '.' || file[0] == '/')file = "." + file;
            else file = "./" + file;
        }
    }
    std::cout << "invalid string given as path: " << file << '\n';
    return "";
}

/**
 * @brief Path to the root directory of the project
 */
const std::string PATH{ FILE_EXISTS("fonts") ? std::string("./") : std::string("../") };