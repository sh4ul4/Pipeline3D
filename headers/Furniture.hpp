#pragma once

/**
 * @brief Meuble de base représentée naïvement par un parallélépipède rectangle.
 * Le constructeur crée par défaut le meuble au centre de la caméra utilisateur.
 * 
 * @private string  Indice de la Shape géométrique représentant le meuble
 * @private bool    Mode de sélection du meuble pour définir son intération
 * 
 */ 
class Furniture {
private:
    /**
     * @brief Indice de la Shape géométrique représentant le meuble.
     * 
     * Cet indice sert de pointeur vers l'ensemble des paramètres Shape (vecteur de Triangles et Texture)
     */
    std::string shape_name; 

    /**
     * @brief Booléens indiquant le mode du meuble, si les intéractions sont possibles ou non.
     */ 
    bool selected = false;
    bool clicked = false;

    /**
     * @brief Boutons d'interface apparaissant près de la forme quand elle est sélectionnée.
     * 
     * Chacun des boutons possède un attribut booléen pour indiquer sa visibilité / cliquabilité. 
     */
    // Button b_remove;
    // Button b_rotate;
    // Button b_rename;
    // TextInput t_rename;

public:
    /**
     * @brief Constructeur par défaut: Fait appel au constructeur de Shape en lui attribuant son nom unique
     *        et créé un parallélépipède rectangle à partir des dimensions. 
     *        Le centre de l'objet se positionne sur le centre de la caméra.  
     * 
     * @param string    Nom unique de la Shape (Géométrie) à créer
     * @param height    Hauteur de la forme
     * @param lenght    Longueur de la forme
     * @param large     Largeur de la forme
     * @param Bitmap    Texture à apposer sur la forme géométrique
     */
    Furniture(std::string shape_name, float height, float lenght, float large, Bitmap *bmp = nullptr);

    bool isSelected() { return selected; };
    bool isClicked() { return clicked; };

    /**
     * @brief Évènement déclenché quand un meuble est sélectionné via clic de souris.
     * 
     * Met à jour le booléen et ajoute/retire une texture de surbrillance au meuble sélectionné, 
     * permettant les intéractions (rend visible les boutons et permet le glissement par maintien de clic)
     */ 
    void selectEvent(Mouse mouse);

    /**
     * @brief Permet le déplacement de l'objet par clic maintenu - Sprite dragging (ou glissement d'objet)
     * 
     * - Tant que le clic gauche de souris reste maintenu, le meuble passe en mode transparence
     * - Une fois le clic lâché, si la place est disponible pour les dimensions du meuble, le meuble est placé
     * - Si la place n'est pas disponible, le meuble demeure en mode transparence 
     */ 
    // void dragSprite(Mouse mouse, HomeDesign hm);

    /**
     * @brief Supprime le meuble et la Shape géométrique correspondante suite au clic sur bouton remove
     */ 
    void remove();
    // ~Furniture();
};


/**
 * @brief Meuble par défaut issu de la classe Furniture : 
 *        Parallépipède simple surélevé d'une hauteur de pieds
 */ 
class f_Foot: public Furniture {
private:
    /**
     * @brief Presets de types de meubles pris en charge par cette classe fille. 
     * 
     * Les std::string représentent des chemin d'accès vers des fichiers objets
     */
    std::string p_table;
    std::string p_lowtable;
    std::string p_desk;
    std::string p_bed;
public:
    /**
     * @brief Constructeur par défaut d'une table / bureau / lit : fait appel au constructeur de la classe mère.
     *        Similaire au parallépipède simple mais surélevé d'une hauteur de pieds.
     * 
     * @param string    Nom unique de la Shape (Géométrie) à créer
     * @param ft_height Hauteur des pieds du meuble
     * @param height    Hauteur du meuble hors pieds
     * @param lenght    Longueur de la forme
     * @param large     Largeur de la forme
     * @param Bitmap    Texture à apposer sur la forme géométrique
     */ 
    f_Foot(std::string shape_name, float ft_height, float height, float lenght, float large, Bitmap *bmp = nullptr);


    /**
     * @brief Déclenchée au choix d'un des 4 presets de cette classe pour pré-remplir 
     *        les champs de saisie d'insertion de meuble.
     * 
     * @param int Représente le numéro du preset choisi
     */
    void selectPreset(int preset);
};


/**
 * @brief Meuble par défaut issu de la classe Furniture : 
 *        Chaise (parallélépipède rectangle surélevé d'une hauteur de pieds et dossier)
 */ 
class f_Chair: public Furniture {
private:
    /**
     * @brief Presets de types de meubles pris en charge par cette classe fille. 
     * 
     * Les std::string représentent des chemin d'accès vers des fichiers objets
     */
    std::string p_chair;
    std::string p_armchair;
    std::string p_stool;
public:
    /**
     * @brief Constructeur par défaut d'une chaise : fait appel au constructeur de la classe mère.
     *        Similaire au parallépipède simple mais surélevé d'une hauteur de pieds et augmenté d'un dossier arrière.
     * 
     * @param string    Nom unique de la Shape (Géométrie) à créer
     * @param bk_height Hauteur du dossier de la chaise
     * @param ft_height Hauteur des pieds du meuble
     * @param height    Hauteur du meuble hors pieds
     * @param lenght    Longueur de la forme
     * @param large     Largeur de la forme
     * @param Bitmap    Texture à apposer sur la forme géométrique
     */ 
    f_Chair(std::string shape_name, float bk_height, float ft_height, float height, float lenght, float large, Bitmap *bmp = nullptr);


    /**
     * @brief Déclenchée au choix d'un des 4 presets de cette classe pour pré-remplir 
     *        les champs de saisie d'insertion de meuble.
     * 
     * @param int Représente le numéro du preset choisi
     */
    void selectPreset(int preset);
};


struct furnitureInfos  {
    std::string name;
    std::string type;
    float scale;

    furnitureInfos(std::string name, std::string type, float scale) : 
        name(name), type(type), scale(scale) {}

    furnitureInfos() = delete;
};

struct insertPack {
    int *selectedBox;
    int *interactSpace;
    ShapeManager *manager;
    std::vector<furnitureInfos*> *furnitures;
    ButtonManager *bmInsertion;
    std::vector<std::string> *checkboxes;
    std::string name;
    float scale;
};

struct insertObjPack {
    int *interactSpace;
    ShapeManager *manager;
    std::vector<furnitureInfos*> *furnitures;

    std::string name;
    float scale;
    std::string objPath;
    std::string objSource;
    std::string objRetVal = "";
};

struct radioButtonPack  {
    ButtonManager *bm;
    std::vector<std::string> *checkboxes;
    int selected;
};

struct editFurniturePack  {
    int *interactSpace;
    ShapeManager *manager;
    std::vector<furnitureInfos*> *furnitures;
    int selected;

    std::string newName;
    float newScale;
    // std::string retVal = "";
};

static void furnitureInsertion(insertPack *ip)  {
    switch (*ip->interactSpace)  {
    case 1: // Insertion des meubles de type 1
        switch ((*ip->selectedBox))  {
            case 1: // Table en bois
                (*ip->manager).imprtShapeObj(std::string("OBJ/woodtable/"), "Wood_Table.obj", ip->name, ip->scale);
    
                // (*ip->manager).getShape(ip->name).groundZero();
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Table en bois", ip->scale));
                break;
            case 2:
                // (*ip->manager).imprtShapeObj(std::string("OBJ/lit/"), "Bunk_Bed.obj", "lit", 1.5);
                // (*ip->manager).imprtShapeObj(std::string("OBJ/tabletest/"), "Desk OBJ.obj", "lit", 1);
                break;
            default:
                std::cout << "Insertion de RIEN DU TOUT\n";
                break;
        }
        break;

    case 2: // Insertion des meubles de type 2
        break;

    default:
        break;
    }
    
    // Reset du rectangle d'interaction
    (*ip->interactSpace) = 0;
    
    // Reset des checkboxes du bmInsertion
    for (size_t i = 0; i < (*ip->checkboxes).size(); i++)  
        (*ip->bmInsertion).getButton<radioButtonPack*>((*ip->checkboxes)[i]).setClicked(false);
}

static void objInsertion(insertObjPack *ip)  {
    std::string path = FIND_FILE_BUT_DONT_LEAVE(ip->objPath);
    if (!path.empty())  { 
        (*ip->manager).imprtShapeObj(path+'/', ip->objSource, ip->name, ip->scale);
        (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Objet importé manuellement", ip->scale));
        ip->objRetVal = "L'objet '" + ip->name + "' a été importé avec succès.";
    }
    else  {
        ip->objRetVal = "Le fichier '" + ip->objPath + "' est introuvable.";
    }
    std::cout << ip->objRetVal << '\n';

     // pas de reset du rectangle d'interaction pour afficher le dernier message
    // (*ip->interactSpace) = 0;
}

static void renameFurniture(editFurniturePack *fp)  {
    // Vérifier si le nouveau nom n'est pas déjà pris
    if (!(*fp->manager).nameTaken(fp->newName))  {
        // Rename la shape dans manager et dans furnitures.name
        (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).name = fp->newName;
        (*fp->furnitures)[fp->selected]->name = fp->newName;
    }
    else  {
        // fp->retVal = "Déjà existant !";
        std::cout << "Le nom de meuble '" << fp->newName << "' est déjà existant !\n";
    } 
}

static void rescaleFurniture(editFurniturePack *fp)  {
    // Multiplication de l'echelle par nouveau/ancienne pour reset la scale
    (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).rescale(fp->newScale / (*fp->furnitures)[fp->selected]->scale);
    (*fp->furnitures)[fp->selected]->scale = fp->newScale;
}

static void deleteFurniture(editFurniturePack *fp)  {
    // Faire une vraie fonction delete, ou alors utiliser la visibilité pour un CTRL Z
    (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).visible = false;
    (*fp->manager).pushShapesEditing();
    // Quitter l'interface d'interaction 
    (*fp->interactSpace) = 0;
}