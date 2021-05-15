#pragma once
#include <ctime>
#include <iomanip>

struct insertPack {
    int *selectedBox;
    ShapeManager *manager;
    std::string name;
    float scale;
};

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
void makeWallsVisible(ShapeManager& manager)  {
	manager.getShape("frontWall").visible = true;
	manager.getShape("backWall").visible = true;
	manager.getShape("leftWall").visible = true;
	manager.getShape("rightWall").visible = true;
}

void makeWallsInvisible(ShapeManager& manager)  {
	manager.getShape("frontWall").visible = false;
	manager.getShape("backWall").visible = false;
	manager.getShape("leftWall").visible = false;
	manager.getShape("rightWall").visible = false;
}

/**
 * @brief Changement de vue Caméra, actualise le nom de vue, le mur à cacher et la caméra courante.
 */
void switchCam(camPack *p) {
	(*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
	makeWallsVisible(*p->manager);
    if (!p->referencedWall.compare("top"))
        makeWallsInvisible(*p->manager);
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
	// makeWallsVisible(*p->manager);
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
    int checkBoxDominant = -1;
    insertPack ip0;
    Texture2D TABLE, TABLETRUE;
    
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
                : scene_name(scene), bmInsertion1(inputEvent, window), bmCameras(inputEvent, window), TABLE("HM-Res/TABLE.jpg", window.getRenderer()), TABLETRUE("HM-Res/TABLE-TRUE.jpg", window.getRenderer()) {
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
        // manager.addSphere("point_a", a, 5, blue);
        // manager.addSphere("point_c", c, 5, yellow);
        // manager.addSphere("point_d", d, 5, red);
        
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

        // Espace interaction : Point2D<int>(970, 30), 270, 506
        b_topleftx = 975; b_tly = 40;
        b_width = 260;
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
        text_insertion1.emplace_back(new TextBox("Echelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        // input_insertion1.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
        //                             Point2D<int>(1065, b_tly), 80, 25, window.getRenderer()));
        // input_insertion1.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
        //                             Point2D<int>(1155, b_tly), 80, 25, window.getRenderer()));
        b_tly += 40;

        // CHECKBOXs pour choisir un preset
        
        bmInsertion1.addCheckBox<void*>(std::string("c_table"), light_gray, black, Point2D(990, b_tly), 50, &TABLETRUE, &TABLE);
        bmInsertion1.addRectTextButton<insertPack*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Inserer sur la scene");
        ip0 = { &checkBoxDominant, manager };
        bmInsertion1.getButton<insertPack*>("b_insertFinal1").setAction(insertPourDeVrai, &ip0);
        // setAction([](void* ptr){std::cout << "AAAAAIIEE batââââârd !!!!!" << std::endl;});
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

    static void insertPourDeVrai(insertPack *ip)  {
        if((*ip->selectedBox) == 1)  {
            // (*ip->manager).imprtShapeObj(std::string("OBJ/lit/"), "Bunk_Bed.obj", "lit", 1.5);
            // (*ip->manager).imprtShapeObj(std::string("OBJ/tabletest/"), "Desk OBJ.obj", "lit", 1);
            (*ip->manager).imprtShapeObj(std::string("OBJ/woodtable/"), "Wood_Table.obj", ip->name, ip->scale);
            
            // (*ip->manager).getShape("lit").groundZero();
            HomeDesign::interactSpace = 0;
        }
        else
            std::cout << "Insertion de RIEN DU TOUT\n"; 
    }

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

    void renderInsertion1(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion1.size(); i++)  
            text_insertion1[i]->render(window.getRenderer(), 0, 0);

        for (size_t i = 0; i < input_insertion1.size(); i++)  {
            input_insertion1[i]->checkForInput(inputEvent, window.getRenderer());
            input_insertion1[i]->render(window.getRenderer(), 0);
        }

        bmInsertion1.checkButtons();
		bmInsertion1.renderButtons(window.getRenderer());
        ip0.name = input_insertion1[0]->getText();
        try  {
		    ip0.scale = stof(input_insertion1[1]->getText());
	    }  catch (const std::invalid_argument& ia)  {
            ip0.scale = 1;
        }
        
        if (bmInsertion1.getButton<void*>("c_table").isClicked()) 
            checkBoxDominant = 1;
        else
            checkBoxDominant = 0;
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
