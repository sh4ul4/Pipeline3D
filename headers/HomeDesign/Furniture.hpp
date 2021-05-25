#pragma once


/**
 * @file Furniture.hpp
 * @brief Contient les informations d'un meuble inséré ainsi que ses méthodes d'interaction.
 */ 

class Furniture {
private:

public:
    /**
     * @brief Indice de la Shape géométrique représentant le meuble.
     * Cet indice sert de pointeur vers l'ensemble des paramètres Shape (vecteur de Triangles et Texture)
     */
    std::string name;
    std::string type;
    std::string path;
    std::string source;
    int rotation;
    float scale;

    /**
     * @brief Booléen indiquant le mode du meuble, si les interactions sont possibles ou non.
     */ 
    bool selected = false;


    /**
     * @brief Constructeur par défaut: Fait appel au constructeur de Shape en lui attribuant son nom unique
     *        et créé un parallélépipède rectangle à partir des dimensions. 
     *        Le centre de l'objet se positionne sur le centre de la caméra.  
     * 
     * @param string    Nom unique référent de la Shape (Géométrie)
     * @param type      Type de meuble
     * @param path      Lien du dossier contenant le fichier .obj
     * @param source    Source du fichier .obj
     * @param rotation  Rotation par tranche de 90° du meuble
     * @param scale     Echelle d'affichage du meuble           
     * @param Bitmap    Texture à apposer sur la forme géométrique
     */
    Furniture(std::string name, std::string type, std::string path, std::string source, int rotation, float scale) : 
        name(name), type(type), scale(scale), rotation(rotation), path(path), source(source) {}

    Furniture(std::string name, std::string type, float scale) : 
        name(name), type(type), scale(scale) {}

    Furniture() = delete;

    bool isSelected() { return selected; };
    

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


    void leftRotate(ShapeManager *manager)  {
        Vertex center = manager->getShape(name).center;
        manager->getShape(name).rotateY(center, 1.5708);
        rotation = (rotation+3) % 4;
    }

    void rightRotate(ShapeManager *manager)  {
        Vertex center = manager->getShape(name).center;
        manager->getShape(name).rotateY(center, -1.5708);
        rotation = (rotation+1) % 4;
    }

    void rename(ShapeManager *manager, std::string newName)  {
        // Vérifier si le nouveau nom n'est pas déjà pris
        if (!manager->nameTaken(newName))  {
            // Rename la shape dans manager et dans furnitures.name
            manager->getShape(name).name = newName;
            name = newName;
        }
        else  {
            // fp->retVal = "Déjà existant !";
            std::cout << "Le nom de meuble '" << newName << "' est déjà existant !\n";
        } 
    }

    void rescale(ShapeManager *manager, float newScale)  {
         // Multiplication de l'echelle par nouveau/ancienne pour reset la scale
        manager->getShape(name).rescale(newScale / scale);
        scale = newScale;
    }
    
    /**
     * @brief Supprime le meuble et la Shape géométrique correspondante suite au clic sur bouton remove
     */ 
    void remove(ShapeManager *manager)  {
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

        // manager->getShape(name).visible = false;
        manager->getShape(name).setPos(Vertex(0,255,0)); // On le met dans l'espace en gros
        manager->getShape(name).name = newName;
        name = newName;
        manager->pushShapesEditing();


        // Option 2: supprimer du Manager et du Vector Furnitures (non fonctionnel)
        // manager->removeShape(name);
        // delete this;
    };
    ~Furniture() { };
};

struct insertPack {
    int *selectedBox;
    int *interactSpace;
    ShapeManager *manager;
    std::vector<Furniture*> *furnitures;
    ButtonManager *bmInsertion;
    std::vector<std::string> *checkboxes;
    std::string name;
    float scale;
};

struct editFurniturePack  {
    int *interactSpace;
    ShapeManager *manager;
    std::vector<Furniture*> *furnitures;
    int selected;

    std::string newName;
    float newScale;
    // std::string retVal = "";
};

struct insertObjPack {
    int *interactSpace;
    ShapeManager *manager;
    std::vector<Furniture*> *furnitures;

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
    std::string defaultName;
    TextInput& texti;

    radioButtonPack(ButtonManager *bm, std::vector<std::string> *checkboxes, int selected, 
                    Window *window, std::string defaultName, TextInput& texti) : 
                bm(bm), checkboxes(checkboxes), selected(selected), window(window), defaultName(defaultName), texti(texti) {}
    
    radioButtonPack() = delete; 
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
                (*ip->furnitures).push_back(new Furniture(ip->name, "Bureau", "HM-Res/furnitures/bureau/", "bureau.obj", 0, ip->scale));
                break;
            case 2: // Chaise
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/chaise/"), "chaise.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Chaise", "HM-Res/furnitures/chaise/", "chaise.obj", 0, ip->scale));
                break;
            case 3: // lit
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/lit/"), "lit.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Lit simple", "HM-Res/furnitures/lit/", "lit.obj", 0, ip->scale));
                break;
            case 4: // Commode
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/commode/"), "commode.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Commode 4 tiroirs", "HM-Res/furnitures/commode/", "commode.obj", 0, ip->scale));
                break;
            case 5: // Armoire
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/armoire/"), "armoire.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Armoire", "HM-Res/furnitures/armoire/", "armoire.obj", 0, ip->scale));
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
                (*ip->furnitures).push_back(new Furniture(ip->name, "Placard de cuisine", "HM-Res/furnitures/placard/", "placard.obj", 0, ip->scale));
                break;
            case 2: // evier
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/blocEvier/"), "evier.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Évier de cuisine", "HM-Res/furnitures/blocEvier/", "evier.obj", 0, ip->scale));
                break;
            case 3: // frigo
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/blocFrigo/"), "frigo.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Réfrigérateur", "HM-Res/furnitures/blocFrigo/", "frigo.obj", 0, ip->scale));
                break;
            case 4: // micro onde
                (*ip->manager).imprtShapeObj(std::string("HM-Res/furnitures/microOnde/"), "microOnde.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Micro-onde", "HM-Res/furnitures/microOnde/", "microOnde.obj", 0, ip->scale));
                break;
            case 5: // Table en bois 
                (*ip->manager).imprtShapeObj(std::string("OBJ/woodtable/"), "Wood_Table.obj", ip->name, ip->scale);
                (*ip->furnitures).push_back(new Furniture(ip->name, "Table en bois", "OBJ/woodtable/", "Wood_Table.obj", 0, ip->scale));
                // (*ip->manager).getShape(ip->name).groundZero();
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
        (*ip->furnitures).push_back(new Furniture(ip->name, "Objet importé manuellement", path+'/', ip->objSource, 0, ip->scale));
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
    (*fp->furnitures)[fp->selected]->rename(fp->manager, fp->newName);
}

static void rescaleFurniture(editFurniturePack *fp)  {
    (*fp->furnitures)[fp->selected]->rescale(fp->manager, fp->newScale);
}

static void dragAndDropFurniture(editFurniturePack *fp)  {
    // Need Mouse, manager
    // (*fp->furnitures)[fp->selected]->dragSprite();
}

// Mettre les checks de collision à la rotation !
static void leftRotateFurniture(editFurniturePack *fp)  {
    (*fp->furnitures)[fp->selected]->leftRotate(fp->manager);
}

static void rightRotateFurniture(editFurniturePack *fp)  {
    (*fp->furnitures)[fp->selected]->rightRotate(fp->manager);
}

static void deleteFurniture(editFurniturePack *fp)  {
    (*fp->furnitures)[fp->selected]->remove(fp->manager);
    
    // Quitter l'interface d'interaction 
    (*fp->interactSpace) = 0;
}
