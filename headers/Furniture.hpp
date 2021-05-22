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
    void dragSprite(Mouse mouse, Shape selectedShape)  {
        if (mouse.leftClick && mouse.moving)  {
            selectedShape.move(Vector(mouse.xmov, 0, mouse.ymov));
        }
    };

    /**
     * @brief Supprime le meuble et la Shape géométrique correspondante suite au clic sur bouton remove
     */ 
    void remove();
    // ~Furniture();
};


struct furnitureInfos  {
    std::string name;
    std::string type;
    std::string path;
    std::string source;
    int rotation;
    float scale;

    furnitureInfos(std::string name, std::string type, float scale) : 
        name(name), type(type), scale(scale) {}

    furnitureInfos(std::string name, std::string type, std::string path, std::string source, int rotation, float scale) : 
        name(name), type(type), scale(scale), rotation(rotation), path(path), source(source) {}

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
    Window *window;
    std::string lebail;
    TextInput *texti;

    radioButtonPack(ButtonManager *bm, std::vector<std::string> *checkboxes, int selected, 
                    Window *window, std::string lebail, TextInput *texti) : 
                bm(bm), checkboxes(checkboxes), selected(selected), window(window), lebail(lebail), texti(texti) {}
    
    radioButtonPack() = delete; 
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
    int k = 1;
    // Rename si déjà pris
    while ((*ip->manager).nameTaken(ip->name)) {
        if (std::to_string(k-1)[0] == ip->name.back())
            ip->name.pop_back();
        ip->name += std::to_string(k); k++;
    }

    if (*ip->interactSpace == 1)  {  // Insertion des meubles de type 1
        switch ((*ip->selectedBox))  {
            case 1: // Bureau
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/bureau/"), "bureau.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Bureau", "HM-Res/furnitures/bureau/", "bureau.obj", 0, ip->scale));
                break;
            case 2: // Chaise
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/chaise/"), "chaise.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Chaise", "HM-Res/furnitures/chaise/", "chaise.obj", 0, ip->scale));
                break;
            case 3: // lit
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/lit/"), "lit.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Lit simple", "HM-Res/furnitures/lit/", "lit.obj", 0, ip->scale));
                break;
            case 4: // Commode
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/commode/"), "commode.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Commode 4 tiroirs", "HM-Res/furnitures/commode/", "commode.obj", 0, ip->scale));
                break;
            case 5: // Armoire
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/armoire/"), "armoire.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Armoire", "HM-Res/furnitures/armoire/", "armoire.obj", 0, ip->scale));
                break;
            default:
                std::cout << "Insertion de RIEN DU TOUT\n";
                break;
        }
    }
    else  {// Insertion des meubles de type 2
        switch ((*ip->selectedBox))  {
            case 1: // Placard
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/placard/"), "placard.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Placard de cuisine", "HM-Res/furnitures/placard/", "placard.obj", 0, ip->scale));
                break;
            case 2: // evier
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/blocEvier/"), "evier.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Évier de cuisine", "HM-Res/furnitures/blocEvier/", "evier.obj", 0, ip->scale));
                break;
            case 3: // frigo
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/blocFrigo/"), "frigo.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Réfrigirateur", "HM-Res/furnitures/blocFrigo/", "frigo.obj", 0, ip->scale));
                break;
            case 4: // micro onde
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/microOnde/"), "microOnde.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Micro-onde", "HM-Res/furnitures/microOnde/", "microOnde.obj", 0, ip->scale));
                break;
            case 5: // Table en bois 
                (*ip->manager).imprtShapeObj(std::string("OBJ/woodtable/"), "Wood_Table.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Table en bois", "OBJ/woodtable/", "Wood_Table.obj", 0, ip->scale));
                // (*ip->manager).getShape(ip->name).groundZero();
                // (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/chaise/"), "chaise.obj", ip->name, ip->scale);
                // (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Chaise d'école", "HM-Res/furnitures/chaise/", "chaise.obj", ip->scale));
                break;
            default:
                std::cout << "Insertion de RIEN DU TOUT\n";
                break;
        }
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
        int k = 1;
        // Rename si déjà pris
        while ((*ip->manager).nameTaken(ip->name)) {
            if (std::to_string(k-1)[0] == ip->name.back())
                ip->name.pop_back();
            ip->name += std::to_string(k); k++;
        }
        std::cout << "Import OBJ de " + path+'/' + ip->objSource + "\n";
        (*ip->manager).imprtShapeObj(path+'/', ip->objSource, ip->name, ip->scale);
        (*ip->furnitures).push_back(new furnitureInfos(ip->name, "Objet importé manuellement", path+'/', ip->objSource, 0, ip->scale));
        // (*ip->manager).getShape(ip->name).groundZero();
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

static void dragAndDropFurniture(editFurniturePack *fp)  {
    // Need Mouse, manager
    
    // Get les coordonnées 2D Window du centre du meuble pour warp la souris dedans
    // SDL_WarpMouseInWindow((*p->window).getWindow(), center.x, center.y);

    // Tant que le clic est pas lâché ET qu'il y a de la place pour le meuble (pas de hit)
}

// Mettre les checks de collision à la rotation !
static void leftRotateFurniture(editFurniturePack *fp)  {
    Vertex center = (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).center;
    (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).rotateY(center, 1.5708);
    int rotat = (*fp->furnitures)[fp->selected]->rotation;
    rotat = (rotat+3) % 4;
    (*fp->furnitures)[fp->selected]->rotation = rotat;
}

static void rightRotateFurniture(editFurniturePack *fp)  {
    Vertex center = (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).center;
    (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).rotateY(center, -1.5708);
    int rotat = (*fp->furnitures)[fp->selected]->rotation;
    rotat = (rotat+1) % 4;
    (*fp->furnitures)[fp->selected]->rotation = rotat;
}

static void deleteFurniture(editFurniturePack *fp)  {
    // Option 1: rendre la shape invisible avec un nom random (pour faciliter le CTRL+Z)
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    int stringLength = sizeof(alphanum) - 1;
    std::string newName = "del_";
    for(int z=0; z < 5; z++)  {
        newName += alphanum[rand() % stringLength];
    }

    // (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).visible = false;
    (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).setPos(Vertex(0,255,0)); // On le met dans l'espace en gros
    (*fp->manager).getShape((*fp->furnitures)[fp->selected]->name).name = newName;
    (*fp->furnitures)[fp->selected]->name = newName;
    (*fp->manager).pushShapesEditing();


    // Option 2: supprimer du Manager et du Vector Furnitures (non fonctionnel)
    // (*fp->manager).removeShape((*fp->furnitures)[fp->selected]->name);
    // (*fp->furnitures).erase((*fp->furnitures).begin()+fp->selected);
    
    // Quitter l'interface d'interaction 
    (*fp->interactSpace) = 0;
}
