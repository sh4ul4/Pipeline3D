#pragma once
#include <ctime>
#include <iomanip>



struct camPack {
	Camera *cam;
	std::string viewName;
	TextBox *current_cam_t;
	Window *window;
	ShapeManager *manager;
	std::string referencedWall;
    Render *render;
};

/**
 * @brief Rend l'ensemble des murs de la pièce visibles
 */
void editWallsVisibility(ShapeManager& manager, bool visibility)  {
	manager.getShape("frontWall").visible = visibility;
	manager.getShape("backWall").visible = visibility;
	manager.getShape("leftWall").visible = visibility;
	manager.getShape("rightWall").visible = visibility;
}


/**
 * @brief Changement de vue Caméra, actualise le nom de vue, le mur à cacher et la caméra courante.
 */
void switchCam(camPack *p) {
	(*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
	editWallsVisibility(*p->manager, true);
    if (!p->referencedWall.compare("top"))
        editWallsVisibility(*p->manager, false);
	if (p->referencedWall.compare("none") && p->referencedWall.compare("top"))
		(*p->manager).getShape(p->referencedWall).visible = false;
	(*p->manager).pushShapesEditing();
	p->cam->setCurrent();
}

void goFreeView(camPack *p) {
	p->cam->unlock();
    
    p->cam->moveTo(Camera::getCurrent().getCameraPosition());
    p->cam->angleX = Camera::getCurrent().angleX;
    p->cam->angleY = Camera::getCurrent().angleY;
    SDL_WarpMouseInWindow((*p->window).getWindow(), (*p->render).globalTexture.getWidth() * 0.5, (*p->render).globalTexture.getHeight() * 0.5);

	p->cam->setCurrent();
	(*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
	// editWallsVisibility(*p->manager, true);
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
    std::vector<std::string> furnitures_ref;
    ShapeManager furnitureManager;

    int un = 1, deux = 2, trois = 3;

    // Espace interaction : affichage par défaut
    std::vector<TextBox*> text_insertion_def;
    // std::vector<TextInput*> input_insertion_def;
    // ButtonManager bmInsertion_def;

    // Interface d'insertion Type 1
    std::vector<TextBox*> text_insertion1;
    std::vector<TextInput*> input_insertion1;
    ButtonManager bmInsertion1;
    std::vector<std::string> checkboxes1;

    // Interface d'insertion Type 2
    std::vector<TextBox*> text_insertion2;
    std::vector<TextInput*> input_insertion2;
    ButtonManager bmInsertion2;

    // Interface d'insertion Type 3
    std::vector<TextBox*> text_insertion3;
    std::vector<TextInput*> input_insertion3;
    ButtonManager bmInsertion3;
    

    ButtonManager bmCameras;
    int checkBoxDominant = -1;
    insertPack ip1, ip2, ip3;
    
public:
    static int interactSpace;

    HomeDesign() = delete;
    

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param InputEvent    Utile aux évènements d'interaction utilisateur
     * @param float         Dimensions des 4 murs formant la pièce principale pour créer les rectangles
     */ 
    HomeDesign(std::string scene, ButtonManager& bm, ShapeManager *manager, Window& window, InputEvent& inputEvent, float w1, float w3)
                : scene_name(scene), bmInsertion1(inputEvent, window), bmInsertion2(inputEvent, window), bmInsertion3(inputEvent, window), bmCameras(inputEvent, window) {
        std::cout << " > Constructeur HomeDesign" << std::endl;
        surface = w1 * w3;
        std::cout << "Mur 1: " << w1 << "m" << std::endl;
        std::cout << "Mur 3: " << w3 << "m" << std::endl;
        std::cout << "Surface: " << surface << "m²" <<std::endl;


        w1 *= 20; w3 *= 20;
        int h = 50; // Hauteur de chaque mur
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
        Bitmap::newBitmap(std::string("sol"), "textures/sol.jpg");
        Bitmap::newBitmap(std::string("BACKBLUE"), "textures/blue.png");
        Bitmap::newBitmap(std::string("FACE"), "textures/face.jpg");
        Bitmap::newBitmap(std::string("wall"), "textures/wall.jpg");
        Bitmap::newBitmap(std::string("RIGHT"), "textures/80s_1.jpg");

        manager->addRectangle("floor", a, b, c, d, 4, white, false, Bitmap::getBitmap(std::string("sol")));
        
        // Division à mettre en fonction de la surface
        manager->addRectangle("frontWall", c1, d1, c, d, 4, white, false, Bitmap::getBitmap("FACE"));
        manager->addRectangle("backWall", a1, b1, a, b, 4, white, false, Bitmap::getBitmap("BACKBLUE"));
        manager->addRectangle("leftWall", a1, c1, a, c, 4, white, false, Bitmap::getBitmap("wall"));
        manager->addRectangle("rightWall", b1, d1, b, d, 4, white, false, Bitmap::getBitmap("RIGHT"));
        std::cout<<"Room créée"<<std::endl;

        // Boutons de vues
        initViewsButtons(*manager, window);

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
        // TextBox tb13("Inserer meuble type 1", "fonts/calibri.ttf", 16, black, Point2D<int>(b_topleftx, b_tly), b_width, b_height, window.getRenderer());
        // bm.addRectButton<int*>("b_insertDefault", nullptr, hd_brownButtons, black, &tb13, Point2D<int>(b_topleftx, b_tly), b_width, b_height);
        b_topleftx += 306;

        bm.addRectTextButton<int*>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Inserer meuble type 2");
        bm.getButton<int*>("b_insertDefault2").setAction(insertFurniture, &deux);
        b_topleftx += 306; 

        bm.addRectTextButton<int*>("b_insertDefault3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Inserer meuble type 3");
        bm.getButton<int*>("b_insertDefault3").setAction(insertFurniture, &trois);

        initDefaultInteractSpace(window);
        initInsertion1(manager, window);
        initInsertion2(manager, window);
        initInsertion3(manager, window);
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
            HomeDesign::interactSpace = 2;
        }
        else if (*style == 3) {
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

        bmCameras.addRectTextButtonCustom<camPack*>("b_View4", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Arrière", 14, light_gray);
        b_topleftx += 140;

        bmCameras.addRectTextButtonCustom<camPack*>("b_freeMotionView", Point2D<int>(b_topleftx, b_tly), b_width+80, b_height, hd_greenButtons, black, "Déplacement libre", 14, light_gray);
    }

    void initDefaultInteractSpace(Window& window) {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion_def.emplace_back(new TextBox(scene_name, "fonts/Segoe UI Bold.ttf", 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        text_insertion_def.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

        text_insertion_def.emplace_back(new TextBox("Surface " + std::to_string(surface), "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
    }

    void initInsertion1(ShapeManager *manager, Window& window) {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion1.emplace_back(new TextBox("Insertion type 1", "fonts/Segoe UI Bold.ttf", 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        text_insertion1.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

            // y = 100
        text_insertion1.emplace_back(new TextBox("Nom (referencement)", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        b_tly += 40;

        // text_insertion1.emplace_back(new TextBox("Dimensions (Lxlxh)",  "fonts/calibri.ttf", 20, black, 
        //                             Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion1.emplace_back(new TextBox("Échelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        

        // CHECKBOXs pour choisir un preset
        b_tly += 80;
        text_insertion1.emplace_back(new TextBox("Meubles préconçus",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion1.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 30;
        bmInsertion1.addCheckBox<void*>(std::string("c_table"), light_gray, black, Point2D(985, b_tly), 40, new Texture2D("HM-Res/TABLE-TRUE.jpg", window.getRenderer()), new Texture2D("HM-Res/TABLE.jpg", window.getRenderer()));
        bmInsertion1.addCheckBox<void*>(std::string("c_2"), light_gray, black, Point2D(1035, b_tly), 40);
        bmInsertion1.addCheckBox<void*>(std::string("c_3"), light_gray, black, Point2D(1085, b_tly), 40);
        bmInsertion1.addCheckBox<void*>(std::string("c_4"), light_gray, black, Point2D(1135, b_tly), 40);
        bmInsertion1.addCheckBox<void*>(std::string("c_5"), light_gray, black, Point2D(1185, b_tly), 40);
        bmInsertion1.addRectTextButton<insertPack*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");
        
        checkboxes1.push_back("c_table");
        ip1 = { &checkBoxDominant, &interactSpace, manager, &furnitures_ref, &bmInsertion1, &checkboxes1};
        bmInsertion1.getButton<insertPack*>("b_insertFinal1").setAction(furnitureInsertion, &ip1);
        // setAction([](void* ptr){std::cout << "AAAAAIIEE batââââârd !!!!!" << std::endl;});
    }

    void initInsertion2(ShapeManager *manager, Window& window)  {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion2.emplace_back(new TextBox("Insertion type 2", "fonts/Segoe UI Bold.ttf", 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        text_insertion2.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

            // y = 100
        text_insertion2.emplace_back(new TextBox("Nom (referencement)", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion2.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        b_tly += 40;

        // text_insertion2.emplace_back(new TextBox("Dimensions (Lxlxh)",  "fonts/calibri.ttf", 20, black, 
        //                             Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion2.emplace_back(new TextBox("Échelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion2.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        

        // CHECKBOXs pour choisir un preset
        b_tly += 80;
        text_insertion2.emplace_back(new TextBox("Meubles préconçus",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion2.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 30;
        bmInsertion2.addCheckBox<void*>(std::string("c_1"), light_gray, black, Point2D(985 , b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_2"), light_gray, black, Point2D(1035, b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_3"), light_gray, black, Point2D(1085, b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_4"), light_gray, black, Point2D(1135, b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_5"), light_gray, black, Point2D(1185, b_tly), 40);
        bmInsertion2.addRectTextButton<insertPack*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");
        ip2 = { &checkBoxDominant, &interactSpace, manager, &furnitures_ref, &bmInsertion2 };
        bmInsertion2.getButton<insertPack*>("b_insertFinal1").setAction(furnitureInsertion, &ip2);
        // setAction([](void* ptr){std::cout << "AAAAAIIEE batââââârd !!!!!" << std::endl;});
    }
    void initInsertion3(ShapeManager *manager, Window& window)  {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion3.emplace_back(new TextBox("Insertion type 3", "fonts/Segoe UI Bold.ttf", 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        text_insertion3.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

            // y = 100
        text_insertion3.emplace_back(new TextBox("Nom (referencement)", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion3.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        b_tly += 40;

        // text_insertion3.emplace_back(new TextBox("Dimensions (Lxlxh)",  "fonts/calibri.ttf", 20, black, 
        //                             Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion3.emplace_back(new TextBox("Échelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion3.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        

        // CHECKBOXs pour choisir un preset
        b_tly += 80;
        text_insertion3.emplace_back(new TextBox("Meubles préconçus",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion3.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 30;
        bmInsertion3.addCheckBox<void*>(std::string("c_1"), light_gray, black, Point2D(985 , b_tly), 40);
        bmInsertion3.addCheckBox<void*>(std::string("c_2"), light_gray, black, Point2D(1035, b_tly), 40);
        bmInsertion3.addCheckBox<void*>(std::string("c_3"), light_gray, black, Point2D(1085, b_tly), 40);
        bmInsertion3.addCheckBox<void*>(std::string("c_4"), light_gray, black, Point2D(1135, b_tly), 40);
        bmInsertion3.addCheckBox<void*>(std::string("c_5"), light_gray, black, Point2D(1185, b_tly), 40);
        bmInsertion3.addRectTextButton<insertPack*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");
        ip3 = { &checkBoxDominant, &interactSpace, manager, &furnitures_ref, &bmInsertion3 };
        bmInsertion3.getButton<insertPack*>("b_insertFinal1").setAction(furnitureInsertion, &ip3);
        // setAction([](void* ptr){std::cout << "AAAAAIIEE batââââârd !!!!!" << std::endl;});
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

public: // Méthodes liées à des boutons créés dans main.cpp

    void renderDefault(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion_def.size(); i++)  
            text_insertion_def[i]->render(window.getRenderer(), 0, 0);

        // for (size_t i = 0; i < input_insertion_def.size(); i++)  {
        //     input_insertion_def[i]->checkForInput(inputEvent, window.getRenderer());
        //     input_insertion_def[i]->render(window.getRenderer(), 0);
        // }

        // bmInsertion_def.checkButtons();
		// bmInsertion_def.renderButtons(window.getRenderer());
    }

    void renderInsertion1(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion1.size(); i++)  
            text_insertion1[i]->render(window.getRenderer(), 0, 0);

        for (size_t i = 0; i < input_insertion1.size(); i++)  {
            input_insertion1[i]->checkForInput(inputEvent, window.getRenderer());
            input_insertion1[i]->render(window.getRenderer(), 0);
        }

        bmInsertion1.checkButtons();
		bmInsertion1.renderButtons(window.getRenderer());
        ip1.name = input_insertion1[0]->getText();
        try  {
		    ip1.scale = stof(input_insertion1[1]->getText());
	    }  catch (const std::invalid_argument& ia)  {
            ip1.scale = 1;
        }
        
        if (bmInsertion1.getButton<void*>("c_table").isClicked()) 
            checkBoxDominant = 1;
        else
            checkBoxDominant = 0;
    }

    void renderInsertion2(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion2.size(); i++)  
            text_insertion2[i]->render(window.getRenderer(), 0, 0);

        for (size_t i = 0; i < input_insertion2.size(); i++)  {
            input_insertion2[i]->checkForInput(inputEvent, window.getRenderer());
            input_insertion2[i]->render(window.getRenderer(), 0);
        }

        bmInsertion2.checkButtons();
		bmInsertion2.renderButtons(window.getRenderer());
        ip2.name = input_insertion2[0]->getText();
        try  {
		    ip2.scale = stof(input_insertion2[1]->getText());
	    }  catch (const std::invalid_argument& ia)  {
            ip2.scale = 1;
        }
        
        if (bmInsertion2.getButton<void*>("c_1").isClicked()) 
            checkBoxDominant = 1;
        else
            checkBoxDominant = 0;
    } 

    void renderInsertion3(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion3.size(); i++)  
            text_insertion3[i]->render(window.getRenderer(), 0, 0);

        for (size_t i = 0; i < input_insertion3.size(); i++)  {
            input_insertion3[i]->checkForInput(inputEvent, window.getRenderer());
            input_insertion3[i]->render(window.getRenderer(), 0);
        }

        bmInsertion3.checkButtons();
		bmInsertion3.renderButtons(window.getRenderer());
        ip3.name = input_insertion3[0]->getText();
        try  {
		    ip3.scale = stof(input_insertion3[1]->getText());
	    }  catch (const std::invalid_argument& ia)  {
            ip3.scale = 1;
        }
        
        if (bmInsertion3.getButton<void*>("c_1").isClicked()) 
            checkBoxDominant = 1;
        else
            checkBoxDominant = 0;
    }

    /**
     * @brief Check si un click de mouse a touché l'un des meubles de la pièce
     */
    void checkFurnitureClick(Mouse mouse, ShapeManager& manager) {
        // Boucle sur tous les meubles de la pièce
        for (size_t i = 0; i < furnitures_ref.size(); i++)  {
            if (mouse.leftClick && manager.getShape(furnitures_ref[i]).hit2D(Point2D<int>(mouse.x, mouse.y), Point2D<int>(30,30), 900, 506)) {
                manager.getShape(furnitures_ref[i]).move(Vector(1, 0, 0));
                interactSpace = 4;
            }
            
            // Get bouding box de la shape et check si le clic est dessus
        }

    }

    static void exportImage(Render *r) {
        // Générer un path et nom de fichier en fonction de l'heure
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "../HM-Export_%d-%m-%y_%H-%M-%S");
        auto file = oss.str();
        r->savePNG(file);
        std::cout << file << " exporté !!!!\n";

        // Afficher une TextBox pour indiquer que c'est exporté 
        // Faire une boucle à part pour insérer le nom du fichier ? 
    };
};

int HomeDesign::interactSpace = 0;
