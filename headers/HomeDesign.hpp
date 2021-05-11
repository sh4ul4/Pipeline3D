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


/**
 * @brief Classe principale représentant l'application externe "Modélisation d'Aménagement Intérieur"
 * 
 * @private string  Nom de la scène donné à l'ouverture de l'application
 * @private vector  Vector ensemble de 4 murs délimitant la pièce intérieure
 * @private float   Surface de la pièce (en m²)
 * @private vector  Vector des meubles (classe Furniture) existants sur l'instance courante
 * 
 * [+] Interface graphique et intéraction utilisateur
 */ 
class HomeDesign {
private:
    /**
     * @brief Nom de la scène donné à l'ouverture de l'application 
     */
    std::string scene_name;

    /**
     * @brief Vector ensemble de 4 rectangles représentant 
     *        des murs délimitant la pièce intérieure
     */ 
    std::vector<Rectangle> walls; 

    // Surface de la pièce
    float surface;
    
    /**
     * @brief Vector des meubles (classe Furniture) existants sur l'instance courante
     */ 
    std::vector<Furniture> furnitures;

    int un = 1, deux = 2, trois = 3;

    /*===========================================================================================
     *      INTERFACE GRAPHIQUE
    ===========================================================================================*/
    // Window interface;
    // Textbox title;
    // Keyboad keyboard;
    // Mouse mouse;

	// ButtonManager bm(inputEvent);

    /**
     * @brief Boutons relatifs au changement de Vue Caméra pour la scène 
     */
    // Button b_mainView;
    // Button b_view1;
    // Button b_view2;
    // Button b_view3;
    // Button b_view4;
    // Button b_freeMotionView;

    /**
     * @brief Boutons relatifs à l'insertion de meuble
     * 
     * Chaque bouton est lié à la méthode insertFurniture() en spécifiant son numéro
     */
    // Button b_insertDefault;
    // Button b_insertDefault2;
    // Button b_insertDefault3;
    
    /**
     * @brief Boutons d'interaction globale avec l'application 
     */
    // Button b_editSurface;
    // Button b_save;
    // Button b_export;
    // Button b_quit;

public:
    HomeDesign() = delete;

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param InputEvent    Utile aux évènements d'interaction utilisateur
     * @param float         Dimensions des 4 murs formant la pièce principale pour créer les rectangles
     */ 
    HomeDesign(ButtonManager& bm, ShapeManager& manager, Window& window, float w1, float w3) {
        std::cout << " > Constructeur HomeDesign" << std::endl;
        surface = w1 * w3;
        std::cout << "Mur 1: " << w1 << "m" << std::endl;
        std::cout << "Mur 3: " << w3 << "m" << std::endl;
        std::cout << "Surface: " << surface << "m²" <<std::endl;

        w1 *= 10; w3 *= 10;
        int h = 60; // Hauteur de chaque mur
        // abcd : sol, a1b1c1d1 : plafond
        Vertex a(-w1/2, 0, -w3/2); // Haut gauche      a           b
        Vertex b(-w1/2, 0,  w3/2); // Haut droit     
        Vertex c( w1/2, 0, -w3/2); // Bas gauche
        Vertex d( w1/2, 0,  w3/2); // Bas droit        c           d
 
        Vertex a1(-w1/2, h, -w3/2); 
        Vertex b1(-w1/2, h,  w3/2); 
        Vertex c1( w1/2, h, -w3/2);
        Vertex d1( w1/2, h,  w3/2);

        // sol
        Bitmap::newBitmap(std::string("defense"), std::string("../textures/img.png"));
        Bitmap::newBitmap(std::string("80s"), std::string("../textures/face.jpg"));
        Bitmap::newBitmap(std::string("wall"), std::string("../textures/wall.jpg"));
    
        manager.addRectangle("floor", a, b, c, d, Bitmap::getBitmap(std::string("defense")));
        // manager.addSphere("point_a", a, 5, blue);
        // manager.addSphere("point_c", c, 5, yellow);
        // manager.addSphere("point_d", d, 5, red);

        manager.addRectangle("frontWall", c1, d1, c, d, Bitmap::getBitmap("80s"));
        manager.addRectangle("backWall", a1, b1, a, b, Bitmap::getBitmap("80s"));
        manager.addRectangle("leftWall", a1, c1, a, c, Bitmap::getBitmap("wall"));
        manager.addRectangle("rightWall", b1, d1, b, d, Bitmap::getBitmap("wall"));
        std::cout<<"Room créée"<<std::endl;


        // 2. Boutons interaction Frame : insertion
        int b_width = 286, b_height = 104;
        int b_topleftx = 30, b_tly = 596;
        bm.addRectTextButton<int*>("b_insertDefault", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Inserer meuble type 1");
        bm.getButton<int*>("b_insertDefault").setAction(insertFurniture, &un);
        // TextBox tb13("Inserer meuble type 1", pth+std::string("fonts/calibri.ttf"), 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
        // bm.addRectButton<int*>("b_insertDefault", nullptr, hd_brownButtons, black, &tb13, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
        b_topleftx += 306;

        bm.addRectTextButton<int*>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Inserer meuble type 2");
        bm.getButton<int*>("b_insertDefault2").setAction(insertFurniture, &deux);
        b_topleftx += 306; 

        bm.addRectTextButton<int*>("b_insertDefault3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Inserer meuble type 3");
        bm.getButton<int*>("b_insertDefault3").setAction(insertFurniture, &trois);

        // Espace interaction
        // Point2D<int>(970, 30), 270, 506
        // TextBox t_nom("Nom (référencement)", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(975, 40), 60, 20, window.getRenderer());
	    // TextInput i_nom("default", pth+std::string("fonts/calibri.ttf"), 20, black, Point2D<int>(975, 50), 260, 25, window.getRenderer());

        
        // bm.addRectTextButtonDefault<int*>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Inserer meuble type 2");
        // bm.getButton<int*>("b_insertDefault2").setAction(insertFurniture, &deux);
    };

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param ButtonManager Utile aux évènements d'interaction utilisateur
     * @param path          Fichier à importer pour initialiser la scène
     */ 
    HomeDesign(ButtonManager& bm, std::string path);

    // ~HomeDesign();

private:

    /**
     * @brief Méthode appelée lorsque les boutons d'insertion de meuble sont pressés
     * 
     * @param style   Représente le numéro du bouton (entre 1 et 3)
     */ 
    static void insertFurniture(int *style) {
	    std::cout << "Insertion de meuble de type " << *style << std::endl;
        if (*style == 2)  {
            std::cout << "2 !\n"; 
        }
        else if (*style == 3) {
            std::cout << "3 !\n"; 
        }
        else  {
            std::cout << "Default !\n"; 
            // while (!keyboard.escape.down) {
		    //     i_nom.checkForInput(inputEvent, window.getRenderer(),
            // }
            
        }
    }

    /**
     * @brief Ajoute un meuble au vecteur des meubles de la scène
     * 
     * @param Furniture     Meuble à ajouter
     */
    void addFurniture(Furniture f);

    /**
     * @brief Méthode appelée lorsque les boutons de changement de vue
     * 
     * @param int   Représente le numéro de la vue (entre 1 et 6)
     */ 
    void setView(int view);

    /**
     * @brief Responsable de l'affichage sur l'espace (4) lors des modifications 
     */
    void reloadInterface();

    /**
     * @brief Vérifie selon la surface disponible et les meubles déjà en place si 
     * le meuble référencé par @param f_name peut être placé 
     * 
     * @return  bool    Renvoie vrai si le meuble peut être placé 
     */
    bool checkSpace(std::string f_name);

    void editSurface();

    void saveScene();

    void exportImage();
};
