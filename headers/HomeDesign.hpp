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

struct editFloorPack {
    ShapeManager *manager;
    std::string selectedBitmap;
};

void editFloor(editFloorPack *efp)  {
    (*efp->manager).getShape("floor").changeBitmap(Bitmap::getBitmap(efp->selectedBitmap));
}

void editWalls(editFloorPack *efp)  {

}

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
    float w1, w3, surface;
    
    /**
     * @brief Vector des meubles (classe Furniture) existants sur l'instance courante
     */ 
    // std::vector<Furniture> furnitures;
    std::vector<furnitureInfos*> furnitures;

    // J'aurais pu utiliser ce manager ??
    // En soi ça change rien à part les 5 murs 
    // ShapeManager furnitureManager;

    int un = 1, deux = 2, trois = 3;

    // Espace interaction : affichage par défaut
    std::vector<TextBox*> text_insertion_def;
    // std::vector<TextInput*> input_insertion_def;
    ButtonManager bmInsertion_def;

    // Interface d'insertion Type 1
    std::vector<TextBox*> text_insertion1;
    std::vector<TextInput*> input_insertion1;
    ButtonManager bmInsertion1;
    std::vector<std::string> checkboxes1;
    std::vector<radioButtonPack> rp1;

    radioButtonPack rp11, rp12, rp13, rp14, rp15;

    // Interface d'insertion Type 2
    std::vector<TextBox*> text_insertion2;
    std::vector<TextInput*> input_insertion2;
    ButtonManager bmInsertion2;

    // Interface d'insertion Type 3
    std::vector<TextBox*> text_insertion3;
    std::vector<TextInput*> input_insertion3;
    ButtonManager bmInsertion3;

    // Interface interaction meuble
    std::vector<TextBox*> text_furnitInteract;
    std::vector<TextInput*> input_furnitInteract;
    ButtonManager bmFurnitInteract;
    

    ButtonManager bmCameras;
    int checkBoxDominant = -1;
    insertPack ip1, ip2;
    insertObjPack ip3;

    editFloorPack efp1, efp2, efp3; 

    // Contient les informations nécessaires au modifications du meuble selectionné
    editFurniturePack fp;
    
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
                : scene_name(scene), w1(w1), w3(w3), bmInsertion_def(inputEvent, window), bmInsertion1(inputEvent, window), bmInsertion2(inputEvent, window), 
                  bmInsertion3(inputEvent, window), bmFurnitInteract(inputEvent, window), bmCameras(inputEvent, window) {
        std::cout << " > Constructeur HomeDesign" << std::endl;
        surface = w1 * w3;
        std::cout << "Mur 1: " << w1 << "m" << std::endl;
        std::cout << "Mur 3: " << w3 << "m" << std::endl;
        std::cout << "Surface: " << surface << "m²" <<std::endl;


        w1 *= 20; w3 *= 20;
        int h = 50; // Hauteur de chaque mur
        // abcd : sol, a1b1c1d1 : plafond
        Vertex a(-w1/2, 0, -w3/2); // Haut gauche      a           c 
        Vertex b(-w1/2, 0,  w3/2); // Haut droit     
        Vertex c( w1/2, 0, -w3/2); // Bas gauche
        Vertex d( w1/2, 0,  w3/2); // Bas droit        b           d

        Vertex amax(-w1/2-150, 0, -w3/2);       
        Vertex atop(-w1/2, 0, -w3/2-150);  
        Vertex bmax(-w1/2-150, 0,  w3/2);
        Vertex bbot(-w1/2, 0,  w3/2+150);
        Vertex cmax( w1/2+150, 0, -w3/2 );
        Vertex ctop( w1/2, 0, -w3/2-150);  
        Vertex dmax( w1/2+150, 0,  w3/2 );
        Vertex dbot( w1/2, 0,  w3/2+150);
        
        Vertex hg(-w1/2-150, 0, -w3/2-150);
        Vertex hd(w1/2+150, 0, -w3/2-150);
        Vertex bg(-w1/2-150, 0, w3/2+150); 
        Vertex bd(w1/2+150, 0, w3/2+150);
        // hg             atop       ctop       hd
        

        // amax          a          c           cmax  


        // bmax          b          d           dmax

        Vertex a1(-w1/2, h, -w3/2);  
        Vertex b1(-w1/2, h,  w3/2);  
        Vertex c1( w1/2, h, -w3/2);
        Vertex d1( w1/2, h,  w3/2);

        // sol
        Bitmap::newBitmap("basicFloor", "textures/sol.jpg");
        Bitmap::newBitmap("parquetFloor", "HM-Res/parquetFloor.jpg");
        Bitmap::newBitmap("moquetteFloor", "HM-Res/moquetteFloor.png");
        efp1 = { manager, "basicFloor" };
        efp2 = { manager, "parquetFloor" };
        efp3 = { manager, "moquetteFloor" };

        Bitmap::newBitmap("wall", "HM-Res/WALL.jpg");
        Bitmap::newBitmap("basic-wall", "HM-Res/basic-wall.png");
        Bitmap::newBitmap("windowed-wall", "HM-Res/windowed-wall-rotated.png");
        Bitmap::newBitmap("doored-wall", "HM-Res/doored-wall-right-rotated.png");

        Bitmap::newBitmap("grass", "HM-Res/grass2.jpg");


        // manager->addRectangle("max_floor", amax, bmax, cmax, dmax, 4, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("floor", a, b, c, d, 4, white, false, Bitmap::getBitmap("basicFloor"));

        manager->addRectangle("cdmax_floor", c, d, cmax, dmax, 2, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("abmax_floor", amax, bmax, a, b, 2, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("top_floor", atop, a, ctop, c, 2, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("bot_floor", b, bbot, d, dbot, 2, white, false, Bitmap::getBitmap("grass"));
        
        manager->addRectangle("hg_floor", hg, amax, atop, a, 2, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("hd_floor", ctop, c, hd, cmax, 2, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("bg_floor", bmax, bg, b, bbot, 2, white, false, Bitmap::getBitmap("grass"));
        manager->addRectangle("bd_floor", d, dbot, dmax, bd, 2, white, false, Bitmap::getBitmap("grass"));

        // Division à mettre en fonction de la surface
        manager->addRectangle("frontWall", c1, d1, c, d, 4, white, false, Bitmap::getBitmap("basic-wall"));
        manager->addRectangle("backWall", a1, b1, a, b, 4, white, false, Bitmap::getBitmap("windowed-wall"));
        manager->addRectangle("leftWall", a1, c1, a, c, 4, white, false, Bitmap::getBitmap("doored-wall"));
        manager->addRectangle("rightWall", b1, d1, b, d, 4, white, false, Bitmap::getBitmap("basic-wall"));
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
        bm.addRectTextButton<int*>("b_insertDefault", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Insérer meuble type 1");
        bm.getButton<int*>("b_insertDefault").setAction(insertFurniture, &un);
        b_topleftx += 306;

        bm.addRectTextButton<int*>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Insérer meuble type 2");
        bm.getButton<int*>("b_insertDefault2").setAction(insertFurniture, &deux);
        b_topleftx += 306; 

        bm.addRectTextButton<int*>("b_insertDefault3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Insérer un meuble depuis un fichier .obj");
        bm.getButton<int*>("b_insertDefault3").setAction(insertFurniture, &trois);

        initDefaultInteractSpace(window);
        initInsertionSpace(manager, window);
        initObjInsertionSpace(manager, window);
        initFurnitureInteractionSpace(manager, window);
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
        for (size_t i = 0; i < furnitures.size(); i++)  
            delete furnitures[i];

        for (size_t i = 0; i < text_insertion1.size(); i++)  
            delete text_insertion1[i];
        for (size_t i = 0; i < input_insertion1.size(); i++)  
            delete input_insertion1[i];

        for (size_t i = 0; i < text_insertion2.size(); i++)  
            delete text_insertion2[i];
        for (size_t i = 0; i < input_insertion2.size(); i++)  
            delete input_insertion2[i];

        for (size_t i = 0; i < text_insertion3.size(); i++)  
            delete text_insertion3[i];
        for (size_t i = 0; i < input_insertion3.size(); i++)  
            delete input_insertion3[i];
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

    // À chaque clic sur checkBox, toutes les autres checkboxes du groupes se désactivent
    static void radioButtonMode(radioButtonPack *rp)  { 
        for (size_t i = 0; i < (*rp->checkboxes).size(); i++)  {
            if (((*rp->checkboxes)[rp->selected]).compare((*rp->bm).getButton<radioButtonPack*>((*rp->checkboxes)[i]).name))  {
                (*rp->bm).getButton<radioButtonPack*>((*rp->checkboxes)[i]).setClicked(false);
            }
        }
    }

    /**
     * @brief Boutons relatifs au changement de Vue Caméra pour la scène 
     */
    void initViewsButtons(ShapeManager& manager, Window& window)  {
        int b_width = 120, b_height = 30;
        int b_topleftx = 30, b_tly = 546; // Chaque bouton est espacé de 140 en X
        bmCameras.addRectTextButtonCustom<camPack*>("b_mainView", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Haut", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack*>("b_View1", Point2D<int>(b_topleftx+=140, b_tly), b_width, b_height, dark_gray, black, "Vue Face", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack*>("b_View2", Point2D<int>(b_topleftx+=140, b_tly), b_width, b_height, dark_gray, black, "Vue Gauche", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack*>("b_View3", Point2D<int>(b_topleftx+=140, b_tly), b_width, b_height, dark_gray, black, "Vue Droite", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack*>("b_View4", Point2D<int>(b_topleftx+=140, b_tly), b_width, b_height, dark_gray, black, "Vue Arrière", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack*>("b_freeMotionView", Point2D<int>(b_topleftx+=140, b_tly), b_width+80, b_height, hd_greenButtons, black, "Déplacement libre", 14, light_gray);
    }

    void initDefaultInteractSpace(Window& window) {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion_def.emplace_back(new TextBox(scene_name, "fonts/Segoe UI Bold.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        text_insertion_def.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

        std::stringstream stream, stream1, stream2;
        stream << std::fixed << std::setprecision(2) << w1;
        stream2 << std::fixed << std::setprecision(2) << w3;
        text_insertion_def.emplace_back(new TextBox("Dimension de la pièce : " + stream.str() + "x" + stream2.str() + "m", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        b_tly += 60;
        stream1 << std::fixed << std::setprecision(2) << surface;
        text_insertion_def.emplace_back(new TextBox("Surface : " + stream.str() + "m²", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        
        // input_furnitInteract.emplace_back(new TextInput("<nouvelle tapisserie>", "fonts/calibri.ttf", 20, black,
        //                                                Point2D<int>(b_topleftx, b_tly), 250, 25, window.getRenderer()));
        b_tly +=25;
        // bmInsertion_def.addRectTextButton<void*>("b_editFloorBmp", Point2D<int>(980, b_tly), 250, 25, "Modifier ");
        // fp = { &interactSpace, manager, &furnitures };
        // bmInsertion_def.getButton<editFurniturePack*>("bmfi_fu_Renommer").setAction(renameFurniture, &fp);
        // bmInsertion_def.addCheckBox<void*>("b_wallsVis", light_gray, black, Point2D(985, 300), 40);

        b_tly += 100;
        text_insertion_def.emplace_back(new TextBox("Changer la tapisserie",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion_def.back()->center(Point2D<int>(970, 30), 270);

        b_tly += 30;
        bmInsertion_def.addCheckBox<editFloorPack*>("c_basicFloor", light_gray, black, Point2D(985, b_tly), 40, new Texture2D("textures/sol.jpg", window.getRenderer()), new Texture2D("textures/sol.jpg", window.getRenderer()));
        bmInsertion_def.addCheckBox<editFloorPack*>("c_parquetFloor", light_gray, black, Point2D(1035, b_tly), 40, new Texture2D("HM-Res/parquetFloor.jpg", window.getRenderer()), new Texture2D("HM-Res/parquetFloor.jpg", window.getRenderer()));
        bmInsertion_def.addCheckBox<editFloorPack*>("c_moquetteFloor", light_gray, black, Point2D(1085, b_tly), 40, new Texture2D("HM-Res/moquetteFloor.png", window.getRenderer()), new Texture2D("HM-Res/moquetteFloor.png", window.getRenderer()));
        bmInsertion_def.getButton<editFloorPack*>("c_basicFloor").setAction(editFloor, &efp1);
        bmInsertion_def.getButton<editFloorPack*>("c_parquetFloor").setAction(editFloor, &efp2);
        bmInsertion_def.getButton<editFloorPack*>("c_moquetteFloor").setAction(editFloor, &efp3);

        bmInsertion_def.addCheckBox<void*>("c_4", light_gray, black, Point2D(1135, b_tly), 40);
        bmInsertion_def.addCheckBox<void*>("c_5", light_gray, black, Point2D(1185, b_tly), 40);
    }
    
    void initInsertionSpace(ShapeManager *manager, Window& window) {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion1.emplace_back(new TextBox("Insertion type 1", "fonts/Segoe UI Bold.ttf", 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));

        text_insertion2.emplace_back(new TextBox("Insertion type 2", "fonts/Segoe UI Bold.ttf", 30, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        // text_insertion1.back()->center(Point2D<int>(970, 30), 270);
        // text_insertion2.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;
            // y = 100
        text_insertion1.emplace_back(new TextBox("Nom (referencement)", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        text_insertion2.emplace_back(new TextBox("Nom (referencement)", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        input_insertion2.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        b_tly += 40;
        text_insertion1.emplace_back(new TextBox("Échelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion2.emplace_back(new TextBox("Échelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion1.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        input_insertion2.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), 80, 25, window.getRenderer()));
        
        // CHECKBOXs pour choisir un preset
        b_tly += 80;
        text_insertion1.emplace_back(new TextBox("Meubles préconçus",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion2.emplace_back(new TextBox("Meubles préconçus",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        text_insertion1.back()->center(Point2D<int>(970, 30), 270);
        text_insertion2.back()->center(Point2D<int>(970, 30), 270);

        b_tly += 30;
        bmInsertion1.addCheckBox<radioButtonPack*>("c_table", light_gray, black, Point2D(985, b_tly), 40, new Texture2D("HM-Res/TABLE-TRUE.jpg", window.getRenderer()), new Texture2D("HM-Res/TABLE.jpg", window.getRenderer()));
        bmInsertion1.addCheckBox<radioButtonPack*>("c_commode", light_gray, black, Point2D(1035, b_tly), 40, new Texture2D("HM-Res/Commode-TRUE.jpg", window.getRenderer()), new Texture2D("HM-Res/Commode.jpg", window.getRenderer()));
        bmInsertion1.addCheckBox<radioButtonPack*>("c_lit", light_gray, black, Point2D(1085, b_tly), 40, nullptr/* texture true */);
        bmInsertion1.addCheckBox<radioButtonPack*>("c_bureau", light_gray, black, Point2D(1135, b_tly), 40);
        bmInsertion1.addCheckBox<radioButtonPack*>("c_chaise", light_gray, black, Point2D(1185, b_tly), 40);
        bmInsertion1.addRectTextButton<insertPack*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");
        
        bmInsertion2.addCheckBox<void*>(std::string("c_1"), light_gray, black, Point2D(985 , b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_2"), light_gray, black, Point2D(1035, b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_3"), light_gray, black, Point2D(1085, b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_4"), light_gray, black, Point2D(1135, b_tly), 40);
        bmInsertion2.addCheckBox<void*>(std::string("c_5"), light_gray, black, Point2D(1185, b_tly), 40);
        bmInsertion2.addRectTextButton<insertPack*>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");
        ip2 = { &checkBoxDominant, &interactSpace, manager, &furnitures, &bmInsertion2 };
        bmInsertion2.getButton<insertPack*>("b_insertFinal1").setAction(furnitureInsertion, &ip2);
        
        checkboxes1.insert(checkboxes1.end(),{"c_table", "c_commode", "c_lit", "c_bureau", "c_chaise"});
        rp11 = {&bmInsertion1, &checkboxes1, 0};
        rp12 = {&bmInsertion1, &checkboxes1, 1};
        rp13 = {&bmInsertion1, &checkboxes1, 2};
        rp14 = {&bmInsertion1, &checkboxes1, 3};
        rp15 = {&bmInsertion1, &checkboxes1, 4};
        bmInsertion1.getButton<radioButtonPack*>("c_table").setAction(radioButtonMode, &rp11);
        bmInsertion1.getButton<radioButtonPack*>("c_commode").setAction(radioButtonMode, &rp12);
        bmInsertion1.getButton<radioButtonPack*>("c_lit").setAction(radioButtonMode, &rp13);
        bmInsertion1.getButton<radioButtonPack*>("c_bureau").setAction(radioButtonMode, &rp14);
        bmInsertion1.getButton<radioButtonPack*>("c_chaise").setAction(radioButtonMode, &rp15);
        // for (int i = 0; i < checkboxes1.size(); i++)  {
        //     rp1.push_back({&bmInsertion1, &checkboxes1, i});
        //     bmInsertion1.getButton<radioButtonPack*>(checkboxes1[i]).setAction(radioButtonMode, &(rp1[i]));
        // }


        ip1 = { &checkBoxDominant, &interactSpace, manager, &furnitures, &bmInsertion1, &checkboxes1};
        bmInsertion1.getButton<insertPack*>("b_insertFinal1").setAction(furnitureInsertion, &ip1);
        // setAction([](void* ptr){std::cout << "AAAAAIIEE batââââârd !!!!!" << std::endl;});
    }

    void initObjInsertionSpace(ShapeManager *manager, Window& window)  {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        text_insertion3.emplace_back(new TextBox("Insérer un fichier .obj", "fonts/Segoe UI Bold.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 80, window.getRenderer()));
        text_insertion3.back()->center(Point2D<int>(970, 30), 270);
        b_tly += 60;

            // y = 100
        text_insertion3.emplace_back(new TextBox("Nom (référencement)", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion3.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly),  250, 25, window.getRenderer()));
        b_tly += 40;

        text_insertion3.emplace_back(new TextBox("Chemin d'accès au dossier",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion3.emplace_back(new TextInput("OBJ/default", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), b_width, 25, window.getRenderer()));
        b_tly += 40;

        text_insertion3.emplace_back(new TextBox("Source du fichier .obj",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion3.emplace_back(new TextInput("default.obj", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), b_width, 25, window.getRenderer()));
        b_tly += 40;

        text_insertion3.emplace_back(new TextBox("Échelle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        input_insertion3.emplace_back(new TextInput("1", "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(975, b_tly), b_width, 25, window.getRenderer()));
        
        text_insertion3.emplace_back(new TextBox("ceci est un test de textbox un peu trop grande pour le rectangle",  "fonts/calibri.ttf", 20, black, 
                                    Point2D<int>(b_topleftx, 400), 260, 80, window.getRenderer()));

        bmInsertion3.addRectTextButton<insertObjPack*>("b_insertFinal1", Point2D<int>(980, 350), 250, 40, "Insérer sur la scène");
        ip3 = { &interactSpace, manager, &furnitures };
        bmInsertion3.getButton<insertObjPack*>("b_insertFinal1").setAction(objInsertion, &ip3);
    }

    void initFurnitureInteractionSpace(ShapeManager *manager, Window &window)  {
        int b_topleftx = 980, b_tly = 40;
        int b_width = 260;
        text_furnitInteract.emplace_back(new TextBox("Nom du meuble", "fonts/Segoe UI Bold.ttf", 30, black, Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        
        b_tly += 60;

        text_furnitInteract.emplace_back(new TextBox("Type de meuble", "fonts/Calibri Bold.ttf", 20, black,
                                                     Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        text_furnitInteract.emplace_back(new TextBox("<type de meuble>", "fonts/calibri.ttf", 20, black,
                                                     Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));

        b_tly += 40;
        text_furnitInteract.emplace_back(new TextBox("Échelle actuelle", "fonts/Calibri Bold.ttf", 20, black,
                                                     Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        text_furnitInteract.emplace_back(new TextBox("<echelle>", "fonts/calibri.ttf", 20, black,
                                                     Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));

        b_tly += 40;
        input_furnitInteract.emplace_back(new TextInput("<nom du meuble>", "fonts/calibri.ttf", 20, black,
                                                       Point2D<int>(b_topleftx, b_tly), 250, 25, window.getRenderer()));
        b_tly += 25;
        bmFurnitInteract.addRectTextButton<editFurniturePack*>("bmfi_fu_Renommer", Point2D<int>(980, b_tly), 250, 25, "Renommer le meuble");
        fp = { &interactSpace, manager, &furnitures };
        bmFurnitInteract.getButton<editFurniturePack*>("bmfi_fu_Renommer").setAction(renameFurniture, &fp);

        b_tly += 40;
        input_furnitInteract.emplace_back(new TextInput("<échelle>", "fonts/calibri.ttf", 20, black,
                                                        Point2D<int>(b_topleftx, b_tly), 250, 25, window.getRenderer()));
        b_tly += 25;

        bmFurnitInteract.addRectTextButton<editFurniturePack*>("bmfi_fu_Echelle", Point2D<int>(980, b_tly), 250, 25, "Changer l'échelle");
        bmFurnitInteract.getButton<editFurniturePack*>("bmfi_fu_Echelle").setAction(rescaleFurniture, &fp);
       
        // bmFurnitInteract.addRectTextButton<editFurniturePack *>("bmfi_fu_Deplacer", Point2D<int>(980, 390), 250, 40, "Deplacer le meuble");
        // bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_Deplacer").setAction(dragAndDropFurniture, &fp);

        bmFurnitInteract.addRectTextButton<void *>("bmfi_fu_Rotation", Point2D<int>(980, 440), 250, 40, "Rotation du meuble");
        // bmFurnitInteract.getButton<void *>("bmfi_fu_Rotation").setAction(furnitureRotation, &fp);

        bmFurnitInteract.addRectTextButtonCustom<editFurniturePack *>("bmfi_fu_suppr", Point2D<int>(980, 490), 250, 40, red, black, "Supprimer", 14, black);
        bmFurnitInteract.getButton<editFurniturePack*>("bmfi_fu_suppr").setAction(deleteFurniture, &fp);
    }
    
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

    std::string getSelectedShape() const { return furnitures[fp.selected]->name; };

    void renderDefault(InputEvent& inputEvent, Window& window)  {
        for (size_t i = 0; i < text_insertion_def.size(); i++)  
            text_insertion_def[i]->render(window.getRenderer(), 0, 0);

        // for (size_t i = 0; i < input_insertion_def.size(); i++)  {
        //     input_insertion_def[i]->checkForInput(inputEvent, window.getRenderer());
        //     input_insertion_def[i]->render(window.getRenderer(), 0);
        // }

        bmInsertion_def.checkButtons();
		bmInsertion_def.renderButtons(window.getRenderer());
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

        if (bmInsertion1.getButton<radioButtonPack*>("c_table").isClicked()) 
            checkBoxDominant = 1;
        else if (bmInsertion1.getButton<radioButtonPack*>("c_commode").isClicked())
            checkBoxDominant = 2;
        else if (bmInsertion1.getButton<radioButtonPack*>("c_lit").isClicked())
            checkBoxDominant = 3;
        else if (bmInsertion1.getButton<radioButtonPack*>("c_bureau").isClicked())
            checkBoxDominant = 4;
        else if (bmInsertion1.getButton<radioButtonPack*>("c_chaise").isClicked())
            checkBoxDominant = 5;
        else // Ajouter un elseIf pour chaque meuble
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

    void renderObjInsertionSpace(InputEvent& inputEvent, Window& window)  {
        text_insertion3.back()->update(ip3.objRetVal, window.getRenderer());
        for (size_t i = 0; i < text_insertion3.size(); i++)  {
            text_insertion3[i]->render(window.getRenderer(), 0, 0);
            text_insertion3[i]->center(Point2D<int>(970, 30), 270);
        }

        for (size_t i = 0; i < input_insertion3.size(); i++)  {
            input_insertion3[i]->checkForInput(inputEvent, window.getRenderer());
            input_insertion3[i]->render(window.getRenderer(), 0);
        }

        bmInsertion3.checkButtons();
		bmInsertion3.renderButtons(window.getRenderer());
        // ip3 = { .name = input_insertion3[0]->getText(), input_insertion3[1]->getText(), input_insertion3[2]->getText() };
        ip3.name = input_insertion3[0]->getText();
        ip3.objPath = input_insertion3[1]->getText();
        ip3.objSource = input_insertion3[2]->getText();
        try  {
		    ip3.scale = stof(input_insertion3[3]->getText());
	    }  catch (const std::invalid_argument& ia)  {
            ip3.scale = 1;
        }
    }

    void renderFurnitureInteractionSpace(InputEvent& inputEvent, Window& window, ShapeManager& manager)  {
        // TextBoxes:
        // 0 Nom du meuble
        // 2 Type du meuble
        // 4 Echelle

        furnitureInfos *curr = furnitures[fp.selected];       
        
        text_furnitInteract[0]->update(curr->name, window.getRenderer());
        text_furnitInteract[2]->update(curr->type, window.getRenderer());
        
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << curr->scale;
        text_furnitInteract[4]->update(stream.str(), window.getRenderer());
        

        for (size_t i = 0; i < text_furnitInteract.size(); i++)  {
            text_furnitInteract[i]->render(window.getRenderer(), 0, 0);
            text_furnitInteract[i]->center(Point2D<int>(970, 30), 270);
        }

        for (size_t i = 0; i < input_furnitInteract.size(); i++)  {
            input_furnitInteract[i]->checkForInput(inputEvent, window.getRenderer());
            input_furnitInteract[i]->render(window.getRenderer(), 0);
        }

        fp.newName = input_furnitInteract[0]->getText();

        try  {
		    fp.newScale = stof(input_furnitInteract[1]->getText());
	    }  catch (const std::invalid_argument& ia)  {
            fp.newScale = curr->scale;
        }

        bmFurnitInteract.checkButtons();
		bmFurnitInteract.renderButtons(window.getRenderer());
    }

    /**
     * @brief Check si un click de souris a touché l'un des meubles de la pièce et le marque comme selectionné
     */
    void checkFurnitureClick(Mouse mouse, ShapeManager& manager, Window& window) {
        float minx, maxx, miny, maxy;
        // Boucle sur tous les meubles de la pièce
        for (size_t i = 0; i < furnitures.size(); i++)  {
            if (mouse.leftClick && manager.getShape(furnitures[i]->name).hit2D(Point2D<int>(mouse.x, mouse.y), Point2D<int>(30,30), 900, 506)) {
                interactSpace = 4;
                fp.selected = i;

                // Drag and drop en vue haut et face uniquement
                if (((Camera::getCurrent().getCamId() == "topCam") || (Camera::getCurrent().getCamId() == "faceCam")) && mouse.moving)  {
                    manager.getShape(furnitures[i]->name).move(Vector(mouse.ymov, 0, mouse.xmov));

                        // Option1 : Warp sur le centre du meuble
                    // manager.getShape(furnitures[i]->name).getBoundingBox2D(minx, miny, maxx, maxy);
                    // SDL_WarpMouseInWindow(window.getWindow(), ((maxx+minx)/2)+30, ((maxy+miny)/2)+30);
                }

                return;
            }
        }
        // Pas sur un meuble ET pas dans le rectangle -> reset 
        // Clic hors du rectangle : mouse.x < 970 || mouse.x > 1240 || mouse.y < 30 || mouse.y > 536
        // Clic dans la pipeline : Point2D<int>(30,30), 900, 506
        if (mouse.leftClick && (mouse.x > 30 && mouse.x < 930 && mouse.y > 30 && mouse.y < 536)) {
                interactSpace = 0;
        }
    }

    void moveFurniture(ShapeManager& manager, int direction)  {
        switch (direction)  {
            case 0: // Move Left
                manager.getShape(furnitures[fp.selected]->name).move(Vector(0, 0, -2));
                break;
            case 1: // Move Right
                manager.getShape(furnitures[fp.selected]->name).move(Vector(0, 0, 2));
                break; 
            case 2: // Move Down
                manager.getShape(furnitures[fp.selected]->name).move(Vector(2, 0, 0));
                break;
            case 3: // Move Up
                manager.getShape(furnitures[fp.selected]->name).move(Vector(-2, 0, 0));
                break;
            default:
                break;
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
