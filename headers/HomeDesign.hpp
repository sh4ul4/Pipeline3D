#pragma once
#include <ctime>
#include <iomanip>

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


struct camPack {
	Camera *cam;
	std::string viewName;
	TextBox *current_cam_t;
	Window *window;
	ShapeManager *manager;
	std::string referencedWall;
};

/**
 * @brief Rend l'ensemble des murs de la pièce visibles
 */
void makeWallsVisible(ShapeManager& manager)  {
	manager.getShape("frontWall").visible = true;
	manager.getShape("backWall").visible = true;
	manager.getShape("leftWall").visible = true;
	manager.getShape("rightWall").visible = true;
}

/**
 * @brief Changement de vue Caméra, actualise le nom de vue, le mur à cacher et la caméra courante.
 */
void switchCam(camPack *p) {
	(*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
	makeWallsVisible(*p->manager);
	if (p->referencedWall.compare("none"))
		(*p->manager).getShape(p->referencedWall).visible = false;
	(*p->manager).pushShapesEditing();
	p->cam->setCurrent();
}

void goFreeView(camPack *p) {
	p->cam->setCurrent();
	p->cam->unlock();
	(*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
	makeWallsVisible(*p->manager);
	(*p->manager).pushShapesEditing();
	// Make Zoom buttons invisible ?
}

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
    ShapeManager furnitureManager;

    int un = 1, deux = 2, trois = 3;

    std::vector<TextBox*> text_insertion1;
    std::vector<TextInput*> input_insertion1;
    
    ButtonManager bmInsertion1;

    ButtonManager bmCameras;

    /*===========================================================================================
     *      INTERFACE GRAPHIQUE
    ===========================================================================================*/
    // Window interface;
    // Textbox title;
    // Keyboad keyboard;
    // Mouse mouse;


    
    
    /**
     * @brief Boutons d'interaction globale avec l'application 
     */
    

public:
    static int interactSpace;

    HomeDesign() = delete;
    
    void renderInsertion1(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion1.size(); i++)  
            text_insertion1[i]->render(window.getRenderer(), 0, 0);

        for (size_t i = 0; i < input_insertion1.size(); i++)  {
            input_insertion1[i]->checkForInput(inputEvent, window.getRenderer());
            input_insertion1[i]->render(window.getRenderer(), 0);
        }

        // for (size_t i = 0; i < button_insertion1.size(); i++)  {
        //     button_insertion1[i]->checkButton(inputEvent, Point2D<int>(0, 0));
        //     button_insertion1[i]->render(window.getRenderer());
        // }
        bmInsertion1.checkButtons();
		bmInsertion1.renderButtons(window.getRenderer());
    }

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param InputEvent    Utile aux évènements d'interaction utilisateur
     * @param float         Dimensions des 4 murs formant la pièce principale pour créer les rectangles
     */ 
    HomeDesign(std::string scene, ButtonManager& bm, ShapeManager& manager, Window& window, InputEvent& inputEvent, float w1, float w3)
                : scene_name(scene), bmInsertion1(inputEvent, window), bmCameras(inputEvent, window) {
        std::cout << " > Constructeur HomeDesign" << std::endl;
        surface = w1 * w3;
        std::cout << "Mur 1: " << w1 << "m" << std::endl;
        std::cout << "Mur 3: " << w3 << "m" << std::endl;
        std::cout << "Surface: " << surface << "m²" <<std::endl;

        // bmInsertion1(inputEvent, window);

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
        Bitmap::newBitmap(std::string("defense"), std::string(pth+"/textures/img.png"));
        Bitmap::newBitmap(std::string("BACKBLUE"), std::string(pth+"/textures/blue.png"));
        Bitmap::newBitmap(std::string("FACE"), std::string(pth+"/textures/face.jpg"));
        Bitmap::newBitmap(std::string("wall"), std::string(pth+"/textures/wall.jpg"));
        Bitmap::newBitmap(std::string("RIGHT"), std::string(pth+"/textures/80s_1.jpg"));

        manager.addRectangle("floor", a, b, c, d, Bitmap::getBitmap(std::string("defense")));
        // manager.addSphere("point_a", a, 5, blue);
        // manager.addSphere("point_c", c, 5, yellow);
        // manager.addSphere("point_d", d, 5, red);

        manager.addRectangle("frontWall", c1, d1, c, d, Bitmap::getBitmap("FACE"));
        manager.addRectangle("backWall", a1, b1, a, b, Bitmap::getBitmap("BACKBLUE"));
        manager.addRectangle("leftWall", a1, c1, a, c, Bitmap::getBitmap("wall"));
        manager.addRectangle("rightWall", b1, d1, b, d, Bitmap::getBitmap("RIGHT"));
        std::cout<<"Room créée"<<std::endl;

        // Boutons de vues
        initViewsButtons(manager, window);

        // 2. Boutons interaction Frame : insertion
        /**
         * @brief Boutons relatifs à l'insertion de meuble
         * 
         * Chaque bouton est lié à la méthode insertFurniture() en spécifiant son numéro
         */
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

        // Espace interaction : Point2D<int>(970, 30), 270, 506
        b_topleftx = 975; b_tly = 40;
        b_width = 260;
        text_insertion1.emplace_back(new TextBox("Insertion type 1", pth+std::string("fonts/Segoe UI Bold.ttf"), 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        text_insertion1.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

            // y = 100
        text_insertion1.emplace_back(new TextBox("Nom (referencement)", pth+std::string("fonts/calibri.ttf"), 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("default", pth+std::string("fonts/calibri.ttf"), 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        b_tly += 40;

        text_insertion1.emplace_back(new TextBox("Dimensions (Lxlxh)",  pth+std::string("fonts/calibri.ttf"), 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("10", pth+std::string("fonts/calibri.ttf"), 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        input_insertion1.emplace_back(new TextInput("10", pth+std::string("fonts/calibri.ttf"), 20, black, 
                                    Point2D<int>(1065, b_tly), 80, 25, window.getRenderer()));
        input_insertion1.emplace_back(new TextInput("10", pth+std::string("fonts/calibri.ttf"), 20, black, 
                                    Point2D<int>(1155, b_tly), 80, 25, window.getRenderer()));
        b_tly += 40;


        bmInsertion1.addRectTextButton<void*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Inserer sur la scene");
        bmInsertion1.getButton<void*>("b_insertFinal1").setAction([](void* ptr){std::cout << "AAAAAIIEE batââââârd !!!!!" << std::endl;});
    };

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param ButtonManager Utile aux évènements d'interaction utilisateur
     * @param path          Fichier à importer pour initialiser la scène
     */ 
    HomeDesign(ButtonManager& bm, std::string path);

    std::string getSceneName()  { return scene_name; };
    int getInteractInt() { return interactSpace; };
    void setInteractSpace(int n) { interactSpace = n; };

    void setViewsButtonsAction(camPack& p0, camPack& p1, camPack& p2, camPack& p3, camPack& p4, camPack& p5) {
        bmCameras.getButton<camPack*>("b_mainView").setAction(switchCam, &p0);
        bmCameras.getButton<camPack*>("b_View1").setAction(switchCam, &p1);
        bmCameras.getButton<camPack*>("b_View2").setAction(switchCam, &p2);
        bmCameras.getButton<camPack*>("b_View3").setAction(switchCam, &p3);
        bmCameras.getButton<camPack*>("b_View4").setAction(switchCam, &p4);
        bmCameras.getButton<camPack*>("b_freeMotionView").setAction(goFreeView, &p5);
    }

    void renderViewsButtons(SDL_Renderer *renderer) {
        bmCameras.checkButtons();
		bmCameras.renderButtons(renderer);
    }

    ~HomeDesign()  {
        for (size_t i = 0; i < text_insertion1.size(); i++)  
            delete text_insertion1[i];
        for (size_t i = 0; i < input_insertion1.size(); i++)  
            delete input_insertion1[i];
    }

private:

    /**
     * @brief Méthode appelée lorsque les boutons d'insertion de meuble sont pressés
     * 
     * @param style   Représente le numéro du bouton (entre 1 et 3)
     */ 
    static void insertFurniture(int *style) {
        if (*style == 2)  {
            std::cout << "2 !\n"; 
            HomeDesign::interactSpace = 2;
        }
        else if (*style == 3) {
            std::cout << "3 !\n"; 
            HomeDesign::interactSpace = 3;
        }
        else  {
            HomeDesign::interactSpace = 1;
        }
    }

    /**
     * @brief Boutons relatifs au changement de Vue Caméra pour la scène 
     */
    void initViewsButtons(ShapeManager& manager, Window& window)  {
        int b_width = 120, b_height = 30;
        int b_topleftx = 30, b_tly = 546;
        bmCameras.addRectTextButtonCustom<camPack*>("b_mainView", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Haut", 14, light_gray);
        b_topleftx += 140;

        bmCameras.addRectTextButtonCustom<camPack*>("b_View1", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Face", 14, light_gray);
        b_topleftx += 140;

        bmCameras.addRectTextButtonCustom<camPack*>("b_View2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Gauche", 14, light_gray);
        b_topleftx += 140;

        bmCameras.addRectTextButtonCustom<camPack*>("b_View3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Droite", 14, light_gray);
        b_topleftx += 140;

        bmCameras.addRectTextButtonCustom<camPack*>("b_View4", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Arriere", 14, light_gray);
        b_topleftx += 140;

        bmCameras.addRectTextButtonCustom<camPack*>("b_freeMotionView", Point2D<int>(b_topleftx, b_tly), b_width+80, b_height, hd_greenButtons, black, "Deplacement libre", 14, light_gray);
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

    void exportImage(Render& r) {
        // Générer un path et nom de fichier en fonction de l'heure
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "HM-Export_%d-%m-%y_%H-%M-%S");
        auto file = oss.str();
        // r.savePNG(file);

        // Afficher une TextBox pour indiquer que c'est exporté 
        // Faire une boucle à part pour insérer le nom du fichier ? 
    };
};

int HomeDesign::interactSpace = 0;
