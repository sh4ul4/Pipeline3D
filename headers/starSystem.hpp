#pragma once

#define PLANETMAX 15               // Nos systèmes stellaires comprendront au maximum 15 planètes
#define DISTANCEMAX 6 * pow(10, 9) // En m

/**
 * @class Cette classe représente un système stellaire, avec une étoile autour de laquelle tournent des planètes
 */
class starSystem
{
private:
    /*===========================================================================================
     *      ATTRIBUTS
    ===========================================================================================*/

    // Liste de planètes présentées dans le système stellaire
    std::vector<Planet*> planets;
    // Représente l'étoile du système
    Star* sun = nullptr;
    // Détermine la vitesse de simulation du système (c'est un facteur)
    float simulationSpeed = 1;

public:
    /*===========================================================================================
     *      CONSTRUCTEURS
    ===========================================================================================*/

    // Constructeur par défaut
    starSystem(ButtonManager& bm, ShapeManager& manager, Window& window)
    {
        std::cout << " > Constructeur starSystem" << std::endl;

        //Bitmap::newBitmap(std::string("Space"), std::string(PATH + "textures/space2.jpg"));

        Vertex hg = { 0, 100, 0 };
        Vertex bg = { 0, 0, 0 };
        Vertex hd = { 158, 100, 0 };
        Vertex bd = { 158, 0, 0 };
        std::cout << "Espace créé" << std::endl;
    }

    /**
     * @brief Constructeur avec liste de planètes
     *
     * @param planets Liste des planètes qui seront présentes dans le système stellaire
     */
    starSystem(const std::vector<Planet*>& planets)
    {
        this->planets = planets;
    }

    /**
     * @brief Constructeur avec étoile
     *
     * @param star représente l'étoile autour de laquelle les planètes tournent
     */
    starSystem(Star* star)
    {
        this->sun = star;
    }

    /**
     * @brief Constructeur avec planètes et étoile
     *
     * @param planets Listes des planètes du système stellaire
     * @param star l'étoile autour de laquelle tournent les planètes
     */
    starSystem(const std::vector<Planet*>& planets, Star* star)
    {
        this->planets = planets;
        this->sun = star;
    }

    // Destucteur de la classe
    ~starSystem()
    {
        if (sun)
        {
            delete sun;
            sun = nullptr;
        }

        for (Planet* p : planets)
            delete p;
        planets.clear();
    }

    /*===========================================================================================
     *      METHODES
    ===========================================================================================*/

    /* ----- SETTERS ----- */

    /**
     * @brief Ajoute une planète dans le système stellaire
     *
     * @param planet Planète à ajouter
     */
    void addPlanet(Planet* planet)
    {
        if (planets.size() >= PLANETMAX) {
            std::cout << "Taille maximale de planetes atteintes" << std::endl;
            return;
        }
        if (planet)
            planets.push_back(planet);
            std::cout << "Planete ajoutee au systeme stellaire" << std::endl;
    }

    /**
     * @brief Ajoute une étoile seulement si le système n'en possède pas
     *
     * @param star L'étoile que l'on souhaite ajouter au système
     */
    void addStar(Star* star)
    {
        if (this->sun == nullptr && star != nullptr) {
            this->sun = star;
            std::cout << "Star ajoutee au systeme stellaire" << std::endl;
        }
        else
            std::cout << "Star non ajoutee car il y a deja une etoile presente dans votre systeme stellaire" << std::endl;
    }

    /**
     * @brief Détermine la vitesse de simulation
     *
     * @param speed Facteur de vitesse de la simulation
     */
    void setSimulationSpeed(float speed)
    {
        this->simulationSpeed = speed;
    }

    /* ----- GETTERS ----- */

    /**
     * @brief Récupère la liste des planètes présentent dans le système stellaire courant
     *
     * @return Liste des planètes du système stellaire
     */
    std::vector<Planet*> getPlanets()
    {
        return planets;
    }

    /**
     * @brief Récupère l'étoile du système stellaire courant
     *
     * @return L'étoile du système stellaire
     */
    Star* getStar()
    {
        return sun;
    }

    /**
     * @brief Récupère la vitesse de simulation du système stellaire courant
     *
     * @return Le facteur de vitesse
     */
    float getSimulationSpeed()
    {
        return simulationSpeed;
    }

    /* ----- METHODES ----- */

    // Vérifie que les planètes sont dans les limites imposées (pas trop loin de l'étoile)
    void checkPlanets()
    {
        Point2D<double> positionS = sun->getPosition();
        int distance;
        Point2D<double> positionP;
        for (std::vector<Planet*>::iterator it = planets.begin(); it != planets.end();)
        {
            positionP = (*it)->getPosition();
            distance = positionP.distance(positionS);
            if (distance > DISTANCEMAX)
            {
                delete (*it);
                it = planets.erase(it);
            }
            else
                it++;
        }
    }

    /**
     * @brief Retire une planète du système stellaire
     *
     * @param name Nom de la planète
     */
    void deletePlanet(const std::string& name)
    {
        for (std::vector<Planet*>::iterator it = planets.begin(); it != planets.end(); it++)
        {
            if ((*it)->getName() == name)
            {
                delete (*it);
                planets.erase(it);
                return;
            }
        }
    }

    // Retire l'étoile du système stellaire
    void deleteStar()
    {
        if (sun)
        {
            delete sun;
            sun = nullptr;
            std::cout << "Etoile detruite" << std::endl;
        }
    }

    // Retire tous les astres du système stellaire
    void reset()
    {
        deleteStar();
        for (std::vector<Planet*>::iterator it = planets.begin(); it != planets.end();)
        {
            delete (*it);
            it = planets.erase(it);
        }
        std::cout << "Reinitialisation du systeme stellaire terminee" << std::endl;
    }

    // Vérifie qu'aucune collision n'a lieu dans le système stellaire
    void checkCollision()
    {
        double distance;
        Point2D<double> posA, posB;
        for (std::vector<Planet*>::iterator i = planets.begin(); i != planets.end() - 1; i++)
        {
            posA = (*i)->getPosition();
            // Check si il y a une collision entre la planète et toutes les autres
            for (std::vector<Planet*>::iterator j = planets.begin() + 1; j != planets.end(); j++)
            {
                posB = (*j)->getPosition();
                distance = posA.distance(posB);
                if (distance <= (*i)->getRadius() + (*j)->getRadius())
                    std::cout << "STOP THE SIMULATION !" << std::endl;
            }

            // Check si il y a une collision entre la planète et l'étoile
            posB = sun->getPosition();
            distance = posA.distance(posB);
            if (distance <= (*i)->getRadius() + sun->getRadius())
                std::cout << "STOP THE SIMULATION !" << std::endl;
        }
    }

    // Appelle les fonctions dans cinématique sur les planètes du système stellaires. Calcule la nouvelle position de toutes les planètes du système stellaire.
    void simulation()
    {
        // Si il n'y a aucune planète, alors il n'y a rien à simuler
        if (planets.empty()) {
            std::cout << "Aucune planetes dans le systeme stellaire !" << std::endl << "Simulation abordee" << std::endl;
            return;
        }
        std::cout << "Debut de l'initialisation" << std::endl;
        // Somme de toutes les forces qui s'exercent sur les astres
        Point2D<double> force, tmp;
        // Force exercée par toutes les autres planètes
        for (std::vector<Planet*>::iterator i = planets.begin(); i != planets.end() - 1; i++)
        {
            force.x = force.y = 0;
            for (std::vector<Planet*>::iterator j = planets.begin() + 1; j != planets.end(); j++)
            {
                tmp = Cinematic::attractionForce(*i, *j);
                force = force + tmp;
            }
            // Force exercée par l'étoile du système si il y en a une
            if (sun)
            {
                tmp = Cinematic::attractionForce(*i, sun);
                force = force + tmp;
            }

            // On détermine le vecteur accélération de l'astre 'i' grâce à la somme des forces qui s'exercent sur elle
            Cinematic::Newton2nd(force, *i);

            // On détermine le vecteur vitesse de 'i' à partir du vecteur accélération
            Cinematic::getSpeedFromAcceleration(*i);

            // On détermine le vecteur position de 'i' à partir du vecteur vitesse
            Cinematic::getPositionFromSpeed(*i);
        }
    }

    /**
     * @brief Calcule une vitesse initiale qui favorisera la stabilité du système, c'est-à-dire qui favorise l'orbite d'une planète autour de l'étoile
     *
     * @param p Planète dont on veut calculer le vecteur vitesse initiale. L'attribut 'initalSpeed' est modifié par cette méthode
     */
    void calculateInitialSpeed(Planet& p)
    {
    }
};
