#pragma once

#define PLANETMAX 15 // Nos systèmes stellaires comprendront au maximum 15 planètes
#define DISTANCEMAX 6 * pow(10,9)

/**
 * @brief Classe principale de l'application starSystem
 * 
 * @private string  Nom de la scène donné à l'ouverture de l'application
 * @private vector<Planet*>  Vecteur contenant les différentes planètes actuelles
 * @private Star  étoile du système stellaire
 * @private float  vitesse de simulation du système
 */
class starSystem{
private:
    /*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/

    // Liste de planètes présentées dans le système stellaire
    std::vector<Planet*> planets;
    // Représente l'étoile du système
    Star *sun = nullptr;
    // Détermine la vitesse de simulation du système (c'est un facteur)
    float simulationSpeed = 1;

public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    // Constructeur par défaut
    starSystem(ButtonManager& bm, ShapeManager& manager, Window& window) {
       std::cout << " > Constructeur starSystem" << std::endl;

      /* int un = 1, deux = 2, trois = 3;
       int b_width = 120, b_height = 30;
       int b_topleftx = 800, b_tly = 420;
       bm.addRectTextButtonDefault<int*>("b_insertDefault", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Start Simulation");
       bm.getButton<int*>("b_insertDefault").setAction(simulation(), &un);
       // TextBox tb13("Inserer meuble type 1", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
       // bm.addRectButton<int*>("b_insertDefault", nullptr, elynmarron, black, &tb13, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
       b_topleftx += 306;

       // TextBox tb14("Inserer meuble type 2", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
       // bm.addRectButton<int*>("b_insertDefault2", nullptr, elynmarron, black, &tb14, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
       bm.addRectTextButtonDefault<int*>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Stop Simulation");
       bm.getButton<int*>("b_insertDefault2").setAction(setSimulationSpeed(0), &deux);
       b_topleftx += 306;

       // TextBox tb15("Inserer meuble type 3", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
       // bm.addRectButton<int*>("b_insertDefault3", nullptr, elynmarron, black, &tb15, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
       bm.addRectTextButtonDefault<int*>("b_insertDefault3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Reset");
       bm.getButton<int*>("b_insertDefault3").setAction(reset(), &trois);*/
    }

    /**
     * @brief Constructeur avec liste de planètes
     * 
     * @param planets Liste des planètes qui seront présentes dans le système stellaire
     */
    starSystem(const std::vector<Planet*> &planets);

    /**
     * @brief Constructeur avec étoile
     *
     * @param star représente l'étoile autour de laquelle les planètes tournent
     */
    starSystem(Star *star);

    /**
     * @brief Constructeur avec planètes et étoile
     * 
     * @param planets Listes des planètes du système stellaire
     * @param star l'étoile autour de laquelle tournent les planètes
     */
    starSystem(ButtonManager& bm, ShapeManager& manager, Window& window, const std::vector<Planet*> &planets, Star *star){
        std::cout << " > Constructeur starSystem" << std::endl;

        /*int un = 1, deux = 2, trois = 3;
        int b_width = 120, b_height = 30;
        int b_topleftx = 1200, b_tly = 470;
        bm.addRectTextButtonDefault<int*>("b_insertDefault", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Start Simulation");
        bm.getButton<int*>("b_insertDefault").setAction(simulation(), &un);
        // TextBox tb13("Inserer meuble type 1", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
        // bm.addRectButton<int*>("b_insertDefault", nullptr, elynmarron, black, &tb13, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
        b_topleftx += 306;

        // TextBox tb14("Inserer meuble type 2", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
        // bm.addRectButton<int*>("b_insertDefault2", nullptr, elynmarron, black, &tb14, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
        bm.addRectTextButtonDefault<int*>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Stop Simulation");
        bm.getButton<int*>("b_insertDefault2").setAction(setSimulationSpeed(0), &deux);
        b_topleftx += 306; 

        // TextBox tb15("Inserer meuble type 3", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
        // bm.addRectButton<int*>("b_insertDefault3", nullptr, elynmarron, black, &tb15, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
        bm.addRectTextButtonDefault<int*>("b_insertDefault3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Reset");
        bm.getButton<int*>("b_insertDefault3").setAction(reset(), &trois);*/
    }

    // Destucteur de la classe
    //~starSystem();

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Ajoute une planète dans le système stellaire
     * 
     * @param planet Planète à ajouter
     */
    void addPlanet(Planet *planet);

    /**
     * @brief Ajoute une étoile seulement si le système n'en possède pas
     * 
     * @param star L'étoile que l'on souhaite ajouter au système 
     */
    void addStar(Star *star);

    /**
     * @brief Détermine la vitesse de simulation
     * 
     * @param speed Facteur de vitesse de la simulation
     */
    void setSimulationSpeed(float speed);

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère la liste des planètes présentent dans le système stellaire courant
     * 
     * @return Liste des planètes du système stellaire 
     */
    std::vector<Planet*> getPlanets();

    /**
     * @brief Récupère l'étoile du système stellaire courant
     * 
     * @return L'étoile du système stellaire 
     */
    Star *getStar();

    /**
     * @brief Récupère la vitesse de simulation du système stellaire courant
     * 
     * @return Le facteur de vitesse
     */
    float getSimulationSpeed();

    /* ----- METHODES ----- */

    // Vérifie que les planètes sont dans les limites imposées (pas trop loin de l'étoile)
    void checkPlanets();

    /**
     * @brief Retire une planète du système stellaire
     * 
     * @param name Nom de la planète
     */
    void deletePlanet(const std::string &name);

    // Retire l'étoile du système stellaire
    void deleteStar();

    // Retire tous les astres du système stellaire (appelle en boucle 'deletePlanet' et appelle 'deleteStar')
    void reset();

    // Vérifie qu'aucune collision n'a lieu dans le système stellaire
    void checkCollision();

    // Appelle les fonctions dans cinématique sur les planètes du système stellaires. Calcule la nouvelle position de toutes les planètes du système stellaire.
    void simulation();

    /**
     * @brief Calcule une vitesse initiale qui favorisera la stabilité du système, c'est-à-dire qui favorise l'orbite d'une planète autour de l'étoile
     * 
     * @param p Planète dont on veut calculer le vecteur vitesse initiale. L'attribut 'initalSpeed' est modifié par cette méthode
     */
    void calculateInitialSpeed(Planet &p);
};
