#pragma once
#include <ctime>
#include <iomanip>
#include <chrono>

struct initPack
{
    int *start;
    int *checkForError;
    std::string w1;
    std::string w3;
    std::string scene_name;
    std::string import_path;
    int mode;
};

struct camPack
{
    Camera *cam;
    std::string viewName;
    TextBox *current_cam_t;
    Window *window;
    ShapeManager *manager;
    std::string referencedWall;
    Render *render;
};

struct editFloorPack
{
    ShapeManager *manager;
    std::string selectedBitmap;

    std::string selectedRight;
    std::string selectedLeft;
    std::string selectedFront;
    std::string selectedBack;
};

void editFloor(editFloorPack *efp)
{
    (*efp->manager).getShape("floor").changeBitmap(Bitmap::getBitmap(efp->selectedBitmap));
}

void editWalls(editFloorPack *efp)
{
    (*efp->manager).getShape("rightWall").changeBitmap(Bitmap::getBitmap(efp->selectedRight));
    (*efp->manager).getShape("leftWall").changeBitmap(Bitmap::getBitmap(efp->selectedLeft));
    (*efp->manager).getShape("frontWall").changeBitmap(Bitmap::getBitmap(efp->selectedFront));
    (*efp->manager).getShape("backWall").changeBitmap(Bitmap::getBitmap(efp->selectedBack));
}

/**
 * @brief Rend l'ensemble des murs de la pièce visibles
 */
void editWallsVisibility(ShapeManager &manager, bool visibility)
{
    std::vector<std::string> walls;
    walls.insert(walls.end(), {"frontWall", "backWall", "leftWall", "rightWall"});

    for (std::string wall : walls)
    {
        manager.getShape(wall).visible = visibility;
        manager.getShape(wall + 'E').visible = visibility;
        manager.getShape(wall + 'T').visible = visibility;
        manager.getShape(wall + 'L').visible = visibility;
        manager.getShape(wall + 'R').visible = visibility;
    }
}

/**
 * @brief Changement de vue Caméra, actualise le nom de vue, le mur à cacher et la caméra courante.
 */
void switchCam(camPack *p)
{
    (*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
    editWallsVisibility(*p->manager, true);
    if (!p->referencedWall.compare("top")) // Vue du haut: pas de murs
        editWallsVisibility(*p->manager, false);
    if (p->referencedWall.compare("none") && p->referencedWall.compare("top"))
    {
        (*p->manager).getShape(p->referencedWall).visible = false;
        (*p->manager).getShape(p->referencedWall + 'E').visible = false;
        (*p->manager).getShape(p->referencedWall + 'T').visible = false;
        (*p->manager).getShape(p->referencedWall + 'L').visible = false;
        (*p->manager).getShape(p->referencedWall + 'R').visible = false;
    }
    (*p->manager).pushShapesEditing();
    p->cam->setCurrent();
}

void goFreeView(camPack *p)
{
    p->cam->unlock();

    p->cam->moveTo(Camera::getCurrent().getCameraPosition());
    p->cam->angleX = Camera::getCurrent().angleX;
    p->cam->angleY = Camera::getCurrent().angleY;
    SDL_WarpMouseInWindow((*p->window).getWindow(), (*p->render).globalTexture.getWidth() * 0.5, (*p->render).globalTexture.getHeight() * 0.5);

    p->cam->setCurrent();
    (*p->current_cam_t).update(p->viewName, (*p->window).getRenderer());
    // editWallsVisibility(*p->manager, true);
    (*p->manager).pushShapesEditing();
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
class HomeDesign
{
private:
    /**
     * @brief Nom de la scène donné à l'ouverture de l'application 
     */
    std::string scene_name;

    /**
     * @brief Vector ensemble de 4 rectangles représentant 
     *        des murs délimitant la pièce intérieure
     */
    std::vector<std::string> walls;
    std::vector<std::string> ground;

    // Surface de la pièce
    float w1, w3, surface;
    int fopt = 0, wopt = 0;

    ShapeManager &refManager;
    Window &refWindow;
    InputEvent &refInputEvent;
    Mouse &refMouse;
    Render &refRender;
    Keyboard &refKeyboard;

    /**
     * @brief Vector des meubles (classe Furniture) existants sur l'instance courante
     */
    std::vector<Furniture *> furnitures;

    int un = 1, deux = 2, trois = 3;

    // Espace interaction : affichage par défaut
    std::vector<TextBox *> textBoxesDefault;
    ButtonManager bmDefault;

    // Interface d'insertion Type 1 et 2
    std::vector<TextBox *> textBoxesInsert1;
    std::vector<TextBox *> textBoxesInsert2;
    std::vector<TextInput *> textInputsInsert1;
    std::vector<TextInput *> textInputsInsert2;
    ButtonManager bmInsert1;
    ButtonManager bmInsert2;
    std::vector<std::string> checkboxes1, checkboxes2;

    // Interface d'insertion Type 3
    std::vector<TextBox *> textBoxesObjInsert;
    std::vector<TextInput *> textInputsObjInsert;
    ButtonManager bmObjInsert;

    // Interface interaction meuble
    std::vector<TextBox *> text_furnitInteract;
    std::vector<TextInput *> input_furnitInteract;
    ButtonManager bmFurnitInteract;
    std::chrono::steady_clock::time_point launch;

    ButtonManager bmCameras;
    int checkBoxDominant = -1;
    insertPack ip1, ip2;
    insertObjPack ip3;

    editFloorPack efp1, efp2, efp3, efp4, efp5;

    // Contient les informations nécessaires au modifications du meuble selectionné
    editFurniturePack fp;
    // int maVar = 0;
    // bool monMoving = false;

public:
    static int interactSpace;

    HomeDesign() = delete;

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param InputEvent    Utile aux évènements d'interaction utilisateur
     * @param float         Dimensions des 4 murs formant la pièce principale pour créer les rectangles
     */
    HomeDesign(std::string scene, ButtonManager &bm, ShapeManager *manager, Window &window, InputEvent &inputEvent, Render &render, Mouse &mouse, Keyboard &keyboard, float w1, float w3, std::string importPath, bool mode)
        : scene_name(scene), w1(w1), w3(w3), bmDefault(inputEvent, window), bmInsert1(inputEvent, window), bmInsert2(inputEvent, window),
          bmObjInsert(inputEvent, window), bmFurnitInteract(inputEvent, window), bmCameras(inputEvent, window),
          refManager(*manager), refWindow(window), refInputEvent(inputEvent), refRender(render), refMouse(mouse), refKeyboard(keyboard)  {

        walls.insert(walls.end(), {"frontWall", "backWall", "leftWall", "rightWall"});
        ground.insert(ground.end(), {"floor", "cdmax_floor", "abmax_floor", "top_floor", "bot_floor", "hg_floor", "hd_floor", "bg_floor", "bd_floor"});
        
        // Initialisation des boutons de changement de vues
        initViewsButtons(*manager, window);

        // Initialisation des boutons principaux d'insertion
        initInsertionButtons(bm);

        // Initialisation de l'espace interaction : (1) défaut, (2) insertion type 1 et 2, (3) insertion .obj
        //                                       et (4) interactions avec meuble
        initDefaultInteractSpace(window);
        initInsertionSpace(manager, window);
        initObjInsertionSpace(manager, window);
        initFurnitureInteractionSpace(manager, window);

        if (mode)  {
            initImport(manager, importPath);
            return;
        }

        surface = w1 * w3;
        launch = std::chrono::steady_clock::now();

        w1 *= 20;
        w3 *= 20;

        // Initialisation de l'environnement 3D selon les dimensions des murs
        initializeRoom(manager, w1, w3);
    };

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param ButtonManager Utile aux évènements d'interaction utilisateur
     * @param path          Fichier à importer pour initialiser la scène
     */
    void initImport(ShapeManager *manager, std::string path)  {
        launch = std::chrono::steady_clock::now();
        path = FIND_FILE(path);
        std::ifstream in(path);
        if (!in.is_open())
            in = std::ifstream(path);

        if (!in.is_open())
        {
            PRINT_ON_ERR("There's no file named " + path);
            return;
        }

        // Parcours du fichier
        std::string line;
        while (std::getline(in, line))  {
            std::string type;
            std::istringstream iss(line);
            iss >> type;

            if (!type.compare("TITLE"))  {
                iss >> scene_name;
            }
            else if (!type.compare("WALLS"))  {
                iss >> w1 >> w3;
            }
            else if (!type.compare("WOPTS"))  {
                iss >> fopt >> wopt;
            }
            else if (!type.compare("FURNI"))  {
                std::string name, furType, path, source, scale, rotation;

                std::getline(iss, name, '|');
                std::getline(iss, furType, '|');
                std::getline(iss, path, '|');
                std::getline(iss, source, '|');
                std::getline(iss, scale, '|');
                std::getline(iss, rotation, '|');
                name.erase(0, 1);
                furnitures.push_back(new Furniture(name, furType, path, source, stoi(rotation), stof(scale)));
            }
            else if (!type.compare("SHAPE"))  {
                std::string name;
                float x, y, z;
                bool visible;
                std::getline(iss, name, '|');
                name.erase(0, 1);
                iss >> x >> y >> z >> visible;
                for (Furniture *fur : furnitures)  {
                    if (fur->name == name)  {
                        manager->imprtShapeObj(fur->path, fur->source, name, fur->scale);
                        manager->getShape(name).visible = visible;
                        for (size_t i = 0; i < fur->rotation; i++)
                            manager->getShape(name).rotateY(manager->getShape(name).center, -1.5708);
                        manager->getShape(name).setPos(Vertex(x, y, z));
                        break;
                    }
                }
            }
            else
                continue;
        }

        // Fin du parsing
        surface = w1 * w3;

        // fopts à save aussi

        // Initialisation de l'environnement 3D selon les dimensions des murs
        initializeRoom(manager, w1 * 20, w3 * 20);
    };

    std::string getSceneName() { return scene_name; };
    std::string getSelectedShape() const { return furnitures[fp.selected]->name; };

    ~HomeDesign()
    {
        for (size_t i = 0; i < furnitures.size(); i++)
            delete furnitures[i];

        for (size_t i = 0; i < textBoxesDefault.size(); i++)
            delete textBoxesDefault[i];

        for (size_t i = 0; i < textBoxesInsert1.size(); i++)
            delete textBoxesInsert1[i];
        for (size_t i = 0; i < textInputsInsert1.size(); i++)
            delete textInputsInsert1[i];

        for (size_t i = 0; i < textBoxesInsert2.size(); i++)
            delete textBoxesInsert2[i];
        for (size_t i = 0; i < textInputsInsert2.size(); i++)
            delete textInputsInsert2[i];

        for (size_t i = 0; i < textBoxesObjInsert.size(); i++)
            delete textBoxesObjInsert[i];
        for (size_t i = 0; i < textInputsObjInsert.size(); i++)
            delete textInputsObjInsert[i];
    }

private:
    void resetRoom(float w1, float w3)
    {

        for (std::string floor : ground)
            refManager.removeShape(floor);

        for (std::string wall : walls)
        {
            refManager.removeShape(wall);
            refManager.removeShape(wall + 'E');
            refManager.removeShape(wall + 'T');
            refManager.removeShape(wall + 'L');
            refManager.removeShape(wall + 'R');
        }
        initializeRoom(&refManager, w1 * 20, w3 * 20);
    }

    void initializeRoom(ShapeManager *manager, float w1, float w3)
    {

        int h = 50; // Hauteur de chaque mur
        // abcd : sol, a1b1c1d1 : plafond
        Vertex a(-w1 / 2, 0, -w3 / 2); // Haut gauche      a           c
        Vertex b(-w1 / 2, 0, w3 / 2);  // Bas gauche
        Vertex c(w1 / 2, 0, -w3 / 2);  // Haut droite
        Vertex d(w1 / 2, 0, w3 / 2);   // Bas droit        b           d

        // Hauteur plafond
        Vertex a1(-w1 / 2, h, -w3 / 2);
        Vertex b1(-w1 / 2, h, w3 / 2);
        Vertex c1(w1 / 2, h, -w3 / 2);
        Vertex d1(w1 / 2, h, w3 / 2);

        Vertex amax(-w1 / 2 - 150, 0, -w3 / 2);
        Vertex atop(-w1 / 2, 0, -w3 / 2 - 150);
        Vertex bmax(-w1 / 2 - 150, 0, w3 / 2);
        Vertex bbot(-w1 / 2, 0, w3 / 2 + 150);
        Vertex cmax(w1 / 2 + 150, 0, -w3 / 2);
        Vertex ctop(w1 / 2, 0, -w3 / 2 - 150);
        Vertex dmax(w1 / 2 + 150, 0, w3 / 2);
        Vertex dbot(w1 / 2, 0, w3 / 2 + 150);

        Vertex hg(-w1 / 2 - 150, 0, -w3 / 2 - 150);
        Vertex hd(w1 / 2 + 150, 0, -w3 / 2 - 150);
        Vertex bg(-w1 / 2 - 150, 0, w3 / 2 + 150);
        Vertex bd(w1 / 2 + 150, 0, w3 / 2 + 150);

        // sol
        Bitmap::newBitmap("basicFloor", "HM-Res/quadrillage.jpg");
        Bitmap::newBitmap("parquetFloor", "HM-Res/parquetFloor.jpg");
        Bitmap::newBitmap("moquetteFloor", "HM-Res/moquetteFloor.png");
        Bitmap::newBitmap("grassFloor", "HM-Res/grass2.jpg");

        Bitmap::newBitmap("wall", "HM-Res/WALL.jpg");
        Bitmap::newBitmap("basic-wall", "HM-Res/basic-wall.png");
        Bitmap::newBitmap("basic-wall-2", "HM-Res/basic-wall-2.png");
        Bitmap::newBitmap("basic-wall-3", "HM-Res/basic-wall-3.png");
        Bitmap::newBitmap("windowed-wall", "HM-Res/windowed-wall.png");
        Bitmap::newBitmap("windowed-wall-2", "HM-Res/windowed-wall-2.png");
        Bitmap::newBitmap("windowed-wall-3", "HM-Res/windowed-wall-3.png");
        Bitmap::newBitmap("doored-wall", "HM-Res/doored-wall.png");
        Bitmap::newBitmap("doored-wall-2", "HM-Res/doored-wall-2.png");
        Bitmap::newBitmap("doored-wall-3", "HM-Res/doored-wall-3.png");
        Bitmap::newBitmap("baie-wall", "HM-Res/baie-wall.png");
        Bitmap::newBitmap("exterior-wall", "HM-Res/exterior-wall.png");
        Bitmap::newBitmap("blue-floor", "HM-Res/blue-floor.jpg");

        efp1 = {manager, "basicFloor", "basic-wall", "basic-wall", "basic-wall", "basic-wall"};          // Tout blanc
        efp2 = {manager, "parquetFloor", "basic-wall", "doored-wall", "basic-wall", "windowed-wall"};    // Porte et fenetre
        efp3 = {manager, "moquetteFloor", "doored-wall", "windowed-wall", "windowed-wall", "baie-wall"}; // Porte, fenetre et baie
        efp4 = {manager, "grassFloor", "basic-wall-2", "doored-wall-2", "basic-wall-2", "windowed-wall-2"};
        efp5 = {manager, "blue-floor", "basic-wall-3", "doored-wall-3", "basic-wall-3", "windowed-wall-3"};

        // manager->addRectangle("max_floor", amax, bmax, cmax, dmax, 4, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("floor", a, b, c, d, 4, white, false, Bitmap::getBitmap("basicFloor"));

        manager->addRectangle("cdmax_floor", c, d, cmax, dmax, 3, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("abmax_floor", amax, bmax, a, b, 3, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("top_floor", atop, a, ctop, c, 3, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("bot_floor", b, bbot, d, dbot, 3, white, false, Bitmap::getBitmap("grassFloor"));

        manager->addRectangle("hg_floor", hg, amax, atop, a, 3, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("hd_floor", ctop, c, hd, cmax, 3, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("bg_floor", bmax, bg, b, bbot, 3, white, false, Bitmap::getBitmap("grassFloor"));
        manager->addRectangle("bd_floor", d, dbot, dmax, bd, 3, white, false, Bitmap::getBitmap("grassFloor"));

        // Division à mettre en fonction de la surface a b c d coOrdonnées du soL
        manager->addRectangle("frontWall", c1, d1, c, d, 4, white, false, Bitmap::getBitmap("basic-wall"));
        manager->addRectangle("frontWallE", Vertex(c1.x + 3, c1.y, c1.z), Vertex(d1.x + 3, d1.y, d1.z), Vertex(c.x + 3, c.y, c.z), Vertex(d.x + 3, d.y, d.z), 4, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("frontWallT", Vertex(c1.x + 3, c1.y, c1.z), Vertex(d1.x + 3, d1.y, d1.z), c1, d1, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("frontWallL", Vertex(c1.x + 3, c1.y, c1.z), c1, Vertex(c.x + 3, c.y, c.z), c, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("frontWallR", Vertex(d1.x + 3, d1.y, d1.z), d1, Vertex(d.x + 3, d.y, d.z), d, 1, white, false, Bitmap::getBitmap("exterior-wall"));

        manager->addRectangle("backWall", a1, b1, a, b, 4, white, false, Bitmap::getBitmap("windowed-wall"));
        manager->addRectangle("backWallE", Vertex(a1.x - 3, a1.y, a1.z), Vertex(b1.x - 3, b1.y, b1.z), Vertex(a.x - 3, a.y, a.z), Vertex(b.x - 3, b.y, b.z), 4, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("backWallT", Vertex(a1.x - 3, a1.y, a1.z), Vertex(b1.x - 3, b1.y, b1.z), a1, b1, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("backWallL", Vertex(a1.x - 3, a1.y, a1.z), a1, Vertex(a.x - 3, a.y, a.z), a, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("backWallR", Vertex(b1.x - 3, b1.y, b1.z), b1, Vertex(b.x - 3, b.y, b.z), b, 1, white, false, Bitmap::getBitmap("exterior-wall"));

        manager->addRectangle("leftWall", a1, c1, a, c, 4, white, false, Bitmap::getBitmap("doored-wall"));
        manager->addRectangle("leftWallE", Vertex(a1.x, a1.y, a1.z - 3), Vertex(c1.x, c1.y, c1.z - 3), Vertex(a.x, a.y, a.z - 3), Vertex(c.x, c.y, c.z - 3), 4, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("leftWallT", Vertex(a1.x, a1.y, a1.z - 3), Vertex(c1.x, c1.y, c1.z - 3), a1, c1, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("leftWallL", Vertex(a1.x, a1.y, a1.z - 3), a1, Vertex(a.x, a.y, a.z - 3), a, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("leftWallR", Vertex(c1.x, c1.y, c1.z - 3), c1, Vertex(c.x, c.y, c.z - 3), c, 1, white, false, Bitmap::getBitmap("exterior-wall"));

        manager->addRectangle("rightWall", b1, d1, b, d, 4, white, false, Bitmap::getBitmap("basic-wall"));
        manager->addRectangle("rightWallE", Vertex(b1.x, b1.y, b1.z + 3), Vertex(d1.x, d1.y, d1.z + 3), Vertex(b.x, b.y, b.z + 3), Vertex(d.x, d.y, d.z + 3), 4, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("rightWallT", Vertex(d1.x, d1.y, d1.z + 3), Vertex(b1.x, b1.y, b1.z + 3), d1, b1, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("rightWallL", Vertex(d1.x, d1.y, d1.z + 3), d1, Vertex(d.x, d.y, d.z + 3), d, 1, white, false, Bitmap::getBitmap("exterior-wall"));
        manager->addRectangle("rightWallR", Vertex(b1.x, b1.y, b1.z + 3), b1, Vertex(b.x, b.y, b.z + 3), b, 1, white, false, Bitmap::getBitmap("exterior-wall"));

        std::cout << "Scène créée" << std::endl;
    }

    /**
     * @brief Méthode appelée lorsque les boutons d'insertion de meuble sont pressés
     * 
     * @param style   Représente le numéro du bouton (entre 1 et 3)
     */
    static void insertFurniture(int *style)  {
        HomeDesign::interactSpace = *style;
    }

    // À chaque clic sur checkBox, toutes les autres checkboxes du groupes se désactivent
    static void radioButtonMode(radioButtonPack *rp)  {
        for (size_t i = 0; i < (*rp->checkboxes).size(); i++)  {
            if (((*rp->checkboxes)[rp->selected]).compare((*rp->bm).getButton<radioButtonPack *>((*rp->checkboxes)[i]).name))  {
                (*rp->bm).getButton<radioButtonPack *>((*rp->checkboxes)[i]).setClicked(false);
            }
        }
        rp->texti.overwrite(rp->defaultName, (*rp->window).getRenderer());
    }

    /**
     * @brief Boutons relatifs à l'insertion de meuble
     * 
     * Chaque bouton est lié à la méthode insertFurniture() en spécifiant son numéro
     */
    void initInsertionButtons(ButtonManager &bm)
    {
        int b_width = 286, b_height = 104;
        int b_topleftx = 30, b_tly = 596;
        bm.addRectTextButton<int *>("b_insertDefault", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Insérer un meuble de chambre");
        bm.getButton<int *>("b_insertDefault").setAction(insertFurniture, &un);
        b_topleftx += 306;

        bm.addRectTextButton<int *>("b_insertDefault2", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Insérer un meuble de cuisine");
        bm.getButton<int *>("b_insertDefault2").setAction(insertFurniture, &deux);
        b_topleftx += 306;

        bm.addRectTextButton<int *>("b_insertDefault3", Point2D<int>(b_topleftx, b_tly), b_width, b_height, "Insérer un meuble depuis un fichier .obj");
        bm.getButton<int *>("b_insertDefault3").setAction(insertFurniture, &trois);
    }

    /**
     * @brief Boutons relatifs au changement de Vue Caméra pour la scène 
     */
    void initViewsButtons(ShapeManager &manager, Window &window)
    {
        int b_width = 120, b_height = 30;
        int b_topleftx = 30, b_tly = 546; // Chaque bouton est espacé de 140 en X
        bmCameras.addRectTextButtonCustom<camPack *>("b_mainView", Point2D<int>(b_topleftx, b_tly), b_width, b_height, dark_gray, black, "Vue Haut", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack *>("b_View1", Point2D<int>(b_topleftx += 140, b_tly), b_width, b_height, dark_gray, black, "Vue Face", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack *>("b_View2", Point2D<int>(b_topleftx += 140, b_tly), b_width, b_height, dark_gray, black, "Vue Gauche", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack *>("b_View3", Point2D<int>(b_topleftx += 140, b_tly), b_width, b_height, dark_gray, black, "Vue Droite", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack *>("b_View4", Point2D<int>(b_topleftx += 140, b_tly), b_width, b_height, dark_gray, black, "Vue Arrière", 14, light_gray);
        bmCameras.addRectTextButtonCustom<camPack *>("b_freeMotionView", Point2D<int>(b_topleftx += 140, b_tly), b_width + 80, b_height, hd_greenButtons, black, "Déplacement libre", 14, light_gray);
    }

    void initDefaultInteractSpace(Window &window)
    {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        textBoxesDefault.emplace_back(new TextBox(scene_name, "fonts/Calibri Bold.ttf", 25, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        b_tly += 50;

        std::stringstream stream, stream1, stream2;
        stream << std::fixed << std::setprecision(2) << w1;
        stream2 << std::fixed << std::setprecision(2) << w3;
        textBoxesDefault.emplace_back(new TextBox("Dimension de la pièce", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        b_tly += 25; //125
        textBoxesDefault.emplace_back(new TextBox(stream.str() + "x" + stream2.str() + "m", "fonts/calibri.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        b_tly += 50;
        textBoxesDefault.emplace_back(new TextBox("Surface de la pièce", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25; //200
        stream1 << std::fixed << std::setprecision(2) << surface;
        textBoxesDefault.emplace_back(new TextBox(stream1.str() + "m²", "fonts/calibri.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        b_tly += 50;

        textBoxesDefault.emplace_back(new TextBox("En création depuis :", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25; //275
        textBoxesDefault.emplace_back(new TextBox("00:00:00", "fonts/calibri.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        // bmDefault.addRectTextButton<void*>("b_editFloorBmp", Point2D<int>(980, b_tly), 250, 25, "Modifier ");
        // fp = { &interactSpace, manager, &furnitures };
        // bmDefault.getButton<editFurniturePack*>("bmfi_fu_Renommer").setAction(renameFurniture, &fp);
        // bmDefault.addCheckBox<void*>("b_wallsVis", light_gray, black, Point2D(985, 300), 40);

        b_tly += 60; //335
        textBoxesDefault.emplace_back(new TextBox("Changer la tapisserie", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));

        b_tly += 30; //365
        bmDefault.addCheckBox<editFloorPack *>("c_basicFloor", light_gray, black, Point2D(985, b_tly), 40, new Texture2D("HM-Res/quadr-preview.png", window.getRenderer()), new Texture2D("HM-Res/quadr-preview.png", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_parquetFloor", light_gray, black, Point2D(1035, b_tly), 40, new Texture2D("HM-Res/parquetFloor.jpg", window.getRenderer()), new Texture2D("HM-Res/parquetFloor.jpg", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_moquetteFloor", light_gray, black, Point2D(1085, b_tly), 40, new Texture2D("HM-Res/moquetteFloor.png", window.getRenderer()), new Texture2D("HM-Res/moquetteFloor.png", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_grassFloor", light_gray, black, Point2D(1135, b_tly), 40, new Texture2D("HM-Res/grass2.jpg", window.getRenderer()), new Texture2D("HM-Res/grass2.jpg", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_blueFloor", light_gray, black, Point2D(1185, b_tly), 40, new Texture2D("HM-Res/blue-floor.jpg", window.getRenderer()), new Texture2D("HM-Res/blue-floor.jpg", window.getRenderer()));
        bmDefault.getButton<editFloorPack *>("c_basicFloor").setAction(editFloor, &efp1);
        bmDefault.getButton<editFloorPack *>("c_parquetFloor").setAction(editFloor, &efp2);
        bmDefault.getButton<editFloorPack *>("c_moquetteFloor").setAction(editFloor, &efp3);
        bmDefault.getButton<editFloorPack *>("c_grassFloor").setAction(editFloor, &efp4);
        bmDefault.getButton<editFloorPack *>("c_blueFloor").setAction(editFloor, &efp5);

        b_tly += 60; //425
        b_tly += 25; //TEST:440
        textBoxesDefault.emplace_back(new TextBox("Changer le papier peint", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));

        textBoxesDefault.emplace_back(new TextBox("", "fonts/Calibri Bold.ttf", 15, hd_greenButtons,
                                                  Point2D<int>(0, 701), 1280, 15, window.getRenderer()));

        b_tly += 30; //455
        bmDefault.addCheckBox<editFloorPack *>("c_basicWall", light_gray, black, Point2D(985, b_tly), 40, new Texture2D("HM-Res/basic-wall.png", window.getRenderer()), new Texture2D("HM-Res/basic-wall.png", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_parquetWall", light_gray, black, Point2D(1035, b_tly), 40, new Texture2D("HM-Res/fenetre.png", window.getRenderer()), new Texture2D("HM-Res/fenetre.png", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_moquetteWall", light_gray, black, Point2D(1085, b_tly), 40, new Texture2D("HM-Res/baie.jpg", window.getRenderer()), new Texture2D("HM-Res/baie.jpg", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_secondWall", light_gray, black, Point2D(1135, b_tly), 40, new Texture2D("HM-Res/basic-wall-2.png", window.getRenderer()), new Texture2D("HM-Res/basic-wall-2.png", window.getRenderer()));
        bmDefault.addCheckBox<editFloorPack *>("c_blueWall", light_gray, black, Point2D(1185, b_tly), 40, new Texture2D("HM-Res/basic-wall-3.png", window.getRenderer()), new Texture2D("HM-Res/basic-wall-3.png", window.getRenderer()));
        bmDefault.getButton<editFloorPack *>("c_basicWall").setAction(editWalls, &efp1);
        bmDefault.getButton<editFloorPack *>("c_parquetWall").setAction(editWalls, &efp2);
        bmDefault.getButton<editFloorPack *>("c_moquetteWall").setAction(editWalls, &efp3);
        bmDefault.getButton<editFloorPack *>("c_secondWall").setAction(editWalls, &efp4);
        bmDefault.getButton<editFloorPack *>("c_blueWall").setAction(editWalls, &efp5);
    }

    void initInsertionSpace(ShapeManager *manager, Window &window)  {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        textBoxesInsert1.emplace_back(new TextBox("Meuble de chambre", "fonts/Calibri Bold.ttf", 25, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));

        textBoxesInsert2.emplace_back(new TextBox("Meuble de cuisine", "fonts/Calibri Bold.ttf", 25, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));
        b_tly += 60;
        // y = 100
        textBoxesInsert1.emplace_back(new TextBox("Nom du meuble", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        textBoxesInsert2.emplace_back(new TextBox("Nom du meuble", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        textInputsInsert1.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black,
                                                     Point2D<int>(b_topleftx, b_tly), 260, 25, window.getRenderer()));
        textInputsInsert2.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, b_tly), 260, 25, window.getRenderer()));
        b_tly += 40;
        textBoxesInsert1.emplace_back(new TextBox("Échelle", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        textBoxesInsert2.emplace_back(new TextBox("Échelle", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        textInputsInsert1.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black,
                                                     Point2D<int>(b_topleftx, b_tly), 260, 25, window.getRenderer()));
        textInputsInsert2.emplace_back(new TextInput("10", "fonts/calibri.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, b_tly), 260, 25, window.getRenderer()));

        b_tly += 80;
        textBoxesInsert1.emplace_back(new TextBox("Meubles préconçus", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        textBoxesInsert2.emplace_back(new TextBox("Meubles préconçus", "fonts/Calibri Bold.ttf", 20, black,
                                                  Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));

        b_tly += 30;
        bmInsert1.addCheckBox<radioButtonPack *>("c_bureau", light_gray, black, Point2D(985, b_tly), 40, new Texture2D("HM-Res/furnitures/bureau/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/bureau/preview.png", window.getRenderer()));
        bmInsert1.addCheckBox<radioButtonPack *>("c_chaise", light_gray, black, Point2D(1035, b_tly), 40, new Texture2D("HM-Res/furnitures/chaise/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/chaise/preview.png", window.getRenderer()));
        bmInsert1.addCheckBox<radioButtonPack *>("c_lit", light_gray, black, Point2D(1085, b_tly), 40, new Texture2D("HM-Res/furnitures/lit/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/lit/preview.png", window.getRenderer()));
        bmInsert1.addCheckBox<radioButtonPack *>("c_commode", light_gray, black, Point2D(1135, b_tly), 40, new Texture2D("HM-Res/furnitures/commode/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/commode/preview.png", window.getRenderer()));
        bmInsert1.addCheckBox<radioButtonPack *>("c_armoire", light_gray, black, Point2D(1185, b_tly), 40, new Texture2D("HM-Res/furnitures/armoire/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/armoire/preview.png", window.getRenderer()));
        bmInsert1.addRectTextButton<insertPack *>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");

        bmInsert2.addCheckBox<radioButtonPack *>(std::string("c_placard"), light_gray, black, Point2D(985, b_tly), 40, new Texture2D("HM-Res/furnitures/placard/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/placard/preview.png", window.getRenderer()));
        bmInsert2.addCheckBox<radioButtonPack *>(std::string("c_evier"), light_gray, black, Point2D(1035, b_tly), 40, new Texture2D("HM-Res/furnitures/blocEvier/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/blocEvier/preview.png", window.getRenderer()));
        bmInsert2.addCheckBox<radioButtonPack *>(std::string("c_frigo"), light_gray, black, Point2D(1085, b_tly), 40, new Texture2D("HM-Res/furnitures/blocFrigo/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/blocFrigo/preview.png", window.getRenderer()));
        bmInsert2.addCheckBox<radioButtonPack *>(std::string("c_microonde"), light_gray, black, Point2D(1135, b_tly), 40, new Texture2D("HM-Res/furnitures/microOnde/preview-TRUE.png", window.getRenderer()), new Texture2D("HM-Res/furnitures/microOnde/preview.png", window.getRenderer()));
        bmInsert2.addCheckBox<radioButtonPack *>(std::string("c_table"), light_gray, black, Point2D(1185, b_tly), 40, new Texture2D("HM-Res/TABLE-TRUE.jpg", window.getRenderer()), new Texture2D("HM-Res/TABLE.jpg", window.getRenderer()));
        bmInsert2.addRectTextButton<insertPack *>("b_insertFinal2", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");

        checkboxes1.insert(checkboxes1.end(), {"c_bureau", "c_chaise", "c_lit", "c_commode", "c_armoire"});
        bmInsert1.getButton<radioButtonPack *>("c_bureau").setAction(radioButtonMode, new radioButtonPack(&bmInsert1, &checkboxes1, 0, &window, "Bureau", *textInputsInsert1[0]));
        bmInsert1.getButton<radioButtonPack *>("c_chaise").setAction(radioButtonMode, new radioButtonPack(&bmInsert1, &checkboxes1, 1, &window, "Chaise", *textInputsInsert1[0]));
        bmInsert1.getButton<radioButtonPack *>("c_lit").setAction(radioButtonMode, new radioButtonPack(&bmInsert1, &checkboxes1, 2, &window, "Lit", *textInputsInsert1[0]));
        bmInsert1.getButton<radioButtonPack *>("c_commode").setAction(radioButtonMode, new radioButtonPack(&bmInsert1, &checkboxes1, 3, &window, "Commode", *textInputsInsert1[0]));
        bmInsert1.getButton<radioButtonPack *>("c_armoire").setAction(radioButtonMode, new radioButtonPack(&bmInsert1, &checkboxes1, 4, &window, "Armoire", *textInputsInsert1[0]));
        ip1 = {&checkBoxDominant, &interactSpace, manager, &furnitures, &bmInsert1, &checkboxes1};
        bmInsert1.getButton<insertPack *>("b_insertFinal1").setAction(furnitureInsertion, &ip1);

        checkboxes2.insert(checkboxes2.end(), {"c_placard", "c_evier", "c_frigo", "c_microonde", "c_table"});
        bmInsert2.getButton<radioButtonPack *>("c_placard").setAction(radioButtonMode, new radioButtonPack(&bmInsert2, &checkboxes2, 0, &window, "Placard", *textInputsInsert2[0]));
        bmInsert2.getButton<radioButtonPack *>("c_evier").setAction(radioButtonMode, new radioButtonPack(&bmInsert2, &checkboxes2, 1, &window, "Evier", *textInputsInsert2[0]));
        bmInsert2.getButton<radioButtonPack *>("c_frigo").setAction(radioButtonMode, new radioButtonPack(&bmInsert2, &checkboxes2, 2, &window, "Frigo", *textInputsInsert2[0]));
        bmInsert2.getButton<radioButtonPack *>("c_microonde").setAction(radioButtonMode, new radioButtonPack(&bmInsert2, &checkboxes2, 3, &window, "Micro-onde", *textInputsInsert2[0]));
        bmInsert2.getButton<radioButtonPack *>("c_table").setAction(radioButtonMode, new radioButtonPack(&bmInsert2, &checkboxes2, 4, &window, "default", *textInputsInsert2[0]));
        ip2 = {&checkBoxDominant, &interactSpace, manager, &furnitures, &bmInsert2, &checkboxes2};
        bmInsert2.getButton<insertPack *>("b_insertFinal2").setAction(furnitureInsertion, &ip2);
    }

    void initObjInsertionSpace(ShapeManager *manager, Window &window)
    {
        // Espace interaction : Point2D<int>(970, 30), 270, 506
        int b_topleftx = 975, b_tly = 40;
        int b_width = 260;
        textBoxesObjInsert.emplace_back(new TextBox("Insérer un fichier .obj", "fonts/Calibri Bold.ttf", 25, black,
                                                    Point2D<int>(b_topleftx, b_tly), b_width, 80, window.getRenderer()));
        b_tly += 60;

        // y = 100
        textBoxesObjInsert.emplace_back(new TextBox("Nom du meuble", "fonts/Calibri Bold.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, b_tly), b_width, 20, window.getRenderer()));
        b_tly += 25;
        textInputsObjInsert.emplace_back(new TextInput("default", "fonts/calibri.ttf", 20, black,
                                                       Point2D<int>(b_topleftx, b_tly), 260, 25, window.getRenderer()));
        b_tly += 40;

        textBoxesObjInsert.emplace_back(new TextBox("Chemin d'accès au dossier", "fonts/Calibri Bold.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        textInputsObjInsert.emplace_back(new TextInput("OBJ/default", "fonts/calibri.ttf", 20, black,
                                                       Point2D<int>(975, b_tly), b_width, 25, window.getRenderer()));
        b_tly += 40;

        textBoxesObjInsert.emplace_back(new TextBox("Source du fichier .obj", "fonts/Calibri Bold.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        textInputsObjInsert.emplace_back(new TextInput("default.obj", "fonts/calibri.ttf", 20, black,
                                                       Point2D<int>(975, b_tly), b_width, 25, window.getRenderer()));
        b_tly += 40;

        textBoxesObjInsert.emplace_back(new TextBox("Échelle", "fonts/Calibri Bold.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, b_tly), 260, 20, window.getRenderer()));
        b_tly += 25;
        textInputsObjInsert.emplace_back(new TextInput("1", "fonts/calibri.ttf", 20, black,
                                                       Point2D<int>(975, b_tly), b_width, 25, window.getRenderer()));

        textBoxesObjInsert.emplace_back(new TextBox("ceci est un test de textbox un peu trop grande pour le rectangle", "fonts/calibri.ttf", 20, black,
                                                    Point2D<int>(b_topleftx, 400), 260, 80, window.getRenderer()));

        bmObjInsert.addRectTextButton<insertObjPack *>("b_insertFinal1", Point2D<int>(980, 440), 250, 40, "Insérer sur la scène");
        ip3 = {&interactSpace, manager, &furnitures};
        bmObjInsert.getButton<insertObjPack *>("b_insertFinal1").setAction(objInsertion, &ip3);
    }

    void initFurnitureInteractionSpace(ShapeManager *manager, Window &window)
    {
        int b_topleftx = 980, b_tly = 40;
        int b_width = 260;
        text_furnitInteract.emplace_back(new TextBox("Nom du meuble", "fonts/Calibri Bold.ttf", 30, black, Point2D<int>(b_topleftx, b_tly), b_width, 40, window.getRenderer()));

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
        bmFurnitInteract.addRectTextButton<editFurniturePack *>("bmfi_fu_Renommer", Point2D<int>(980, b_tly), 250, 25, "Renommer le meuble");
        fp = {&interactSpace, manager, &furnitures};
        bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_Renommer").setAction(renameFurniture, &fp);

        b_tly += 40;
        input_furnitInteract.emplace_back(new TextInput("<échelle>", "fonts/calibri.ttf", 20, black,
                                                        Point2D<int>(b_topleftx, b_tly), 250, 25, window.getRenderer()));
        b_tly += 25;

        bmFurnitInteract.addRectTextButton<editFurniturePack *>("bmfi_fu_Echelle", Point2D<int>(980, b_tly), 250, 25, "Changer l'échelle");
        bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_Echelle").setAction(rescaleFurniture, &fp);

        // bmFurnitInteract.addRectTextButton<editFurniturePack *>("bmfi_fu_Deplacer", Point2D<int>(980, 390), 250, 40, "Deplacer le meuble");
        // bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_Deplacer").setAction(dragAndDropFurniture, &fp);

        bmFurnitInteract.addRectTextButton<editFurniturePack *>("bmfi_fu_leftRotation", Point2D<int>(980, 440), 120, 40, "Rotation gauche");
        bmFurnitInteract.addRectTextButton<editFurniturePack *>("bmfi_fu_rightRotation", Point2D<int>(1110, 440), 120, 40, "Rotation droite");
        bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_leftRotation").setAction(leftRotateFurniture, &fp);
        bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_rightRotation").setAction(rightRotateFurniture, &fp);

        bmFurnitInteract.addRectTextButtonCustom<editFurniturePack *>("bmfi_fu_suppr", Point2D<int>(980, 490), 250, 40, red, black, "Supprimer", 14, black);
        bmFurnitInteract.getButton<editFurniturePack *>("bmfi_fu_suppr").setAction(deleteFurniture, &fp);
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

public: // Méthodes liées à des boutons créés dans main.cpp
    void setViewsButtonsAction(camPack &p0, camPack &p1, camPack &p2, camPack &p3, camPack &p4, camPack &p5)
    {
        bmCameras.getButton<camPack *>("b_mainView").setAction(switchCam, &p0);
        bmCameras.getButton<camPack *>("b_View1").setAction(switchCam, &p1);
        bmCameras.getButton<camPack *>("b_View2").setAction(switchCam, &p2);
        bmCameras.getButton<camPack *>("b_View3").setAction(switchCam, &p3);
        bmCameras.getButton<camPack *>("b_View4").setAction(switchCam, &p4);
        bmCameras.getButton<camPack *>("b_freeMotionView").setAction(goFreeView, &p5);
    }

    void renderViewsButtons(SDL_Renderer *renderer)
    {
        bmCameras.checkButtons();
        bmCameras.renderButtons(renderer);
    }

    void renderDefault(InputEvent &inputEvent, Window &window)
    {
        std::stringstream ss, ss1, ss2, ss3;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        int seconds = (std::chrono::duration_cast<std::chrono::microseconds>(end - launch).count()) / 1000000.0;
        std::stringstream minutes;
        minutes << std::setw(2) << std::setfill('0') << int((seconds / 60) % 60);
        std::stringstream hours;
        hours << std::setw(2) << std::setfill('0') << int((seconds / 60) / 60);
        std::stringstream seconds_s;
        seconds_s << std::setw(2) << std::setfill('0') << int(seconds % 60);

        ss << hours.str() + ":" + minutes.str() + ":" + seconds_s.str();
        // ss << std::fixed << std::setprecision(2) << (std::chrono::duration_cast<std::chrono::microseconds>(end - launch).count()) /1000000.0;
        textBoxesDefault[6]->update(ss.str(), window.getRenderer());
        textBoxesDefault[0]->update(scene_name, window.getRenderer());

        ss1 << std::fixed << std::setprecision(2) << w1;
        ss2 << std::fixed << std::setprecision(2) << w3;
        textBoxesDefault[2]->update(ss1.str() + "x" + ss2.str() + "m", window.getRenderer());

        ss3 << std::fixed << std::setprecision(2) << surface;
        textBoxesDefault[4]->update(ss3.str() + "m²", window.getRenderer());

        for (size_t i = 0; i < textBoxesDefault.size(); i++)
        {
            textBoxesDefault[i]->center(Point2D<int>(970, 30), 270);
            textBoxesDefault[9]->center(Point2D<int>(0, 0), 1280);
            textBoxesDefault[i]->render(window.getRenderer(), 0, 0);
        }

        bmDefault.checkButtons();
        bmDefault.renderButtons(window.getRenderer());
    }

    void renderInsertion1(InputEvent &inputEvent, Window &window)  {
        for (size_t i = 0; i < textBoxesInsert1.size(); i++)  {
            textBoxesInsert1[i]->render(window.getRenderer(), 0, 0);
            textBoxesInsert1[i]->center(Point2D<int>(970, 30), 270);
        }

        for (size_t i = 0; i < textInputsInsert1.size(); i++)  {
            textInputsInsert1[i]->checkForInput(inputEvent, window.getRenderer());
            textInputsInsert1[i]->centerizedRender(Point2D<int>(970, 40), 270, window.getRenderer(), 0);
        }

        bmInsert1.checkButtons();
        bmInsert1.renderButtons(window.getRenderer());
        ip1.name = textInputsInsert1[0]->getText();
        try  {
            ip1.scale = stof(textInputsInsert1[1]->getText());
        }
        catch (const std::invalid_argument &ia)  {
            ip1.scale = 1;
        }

        if (bmInsert1.getButton<radioButtonPack *>("c_bureau").isClicked())
            checkBoxDominant = 1;
        else if (bmInsert1.getButton<radioButtonPack *>("c_chaise").isClicked())
            checkBoxDominant = 2;
        else if (bmInsert1.getButton<radioButtonPack *>("c_lit").isClicked())
            checkBoxDominant = 3;
        else if (bmInsert1.getButton<radioButtonPack *>("c_commode").isClicked())
            checkBoxDominant = 4;
        else if (bmInsert1.getButton<radioButtonPack *>("c_armoire").isClicked())
            checkBoxDominant = 5;
        else // Ajouter un elseIf pour chaque meuble
            checkBoxDominant = 0;
    }

    void renderInsertion2(InputEvent &inputEvent, Window &window)
    {
        for (size_t i = 0; i < textBoxesInsert2.size(); i++)
        {
            textBoxesInsert2[i]->render(window.getRenderer(), 0, 0);
            textBoxesInsert2[i]->center(Point2D<int>(970, 30), 270);
        }

        for (size_t i = 0; i < textInputsInsert2.size(); i++)
        {
            textInputsInsert2[i]->checkForInput(inputEvent, window.getRenderer());
            textInputsInsert2[i]->centerizedRender(Point2D<int>(970, 40), 270, window.getRenderer(), 0);
        }

        bmInsert2.checkButtons();
        bmInsert2.renderButtons(window.getRenderer());
        ip2.name = textInputsInsert2[0]->getText();
        try
        {
            ip2.scale = stof(textInputsInsert2[1]->getText());
        }
        catch (const std::invalid_argument &ia)
        {
            ip2.scale = 1;
        }

        if (bmInsert2.getButton<radioButtonPack *>("c_placard").isClicked())
            checkBoxDominant = 1;
        else if (bmInsert2.getButton<radioButtonPack *>("c_evier").isClicked())
            checkBoxDominant = 2;
        else if (bmInsert2.getButton<radioButtonPack *>("c_frigo").isClicked())
            checkBoxDominant = 3;
        else if (bmInsert2.getButton<radioButtonPack *>("c_microonde").isClicked())
            checkBoxDominant = 4;
        else if (bmInsert2.getButton<radioButtonPack *>("c_table").isClicked())
            checkBoxDominant = 5;
        else // Ajouter un elseIf pour chaque meuble
            checkBoxDominant = 0;
    }

    void renderObjInsertionSpace(InputEvent &inputEvent, Window &window)
    {
        textBoxesObjInsert.back()->update(ip3.objRetVal, window.getRenderer());
        for (size_t i = 0; i < textBoxesObjInsert.size(); i++)
            textBoxesObjInsert[i]->centerizedRender(Point2D<int>(970, 30), 270, window.getRenderer());

        for (size_t i = 0; i < textInputsObjInsert.size(); i++)
        {
            textInputsObjInsert[i]->checkForInput(inputEvent, window.getRenderer());
            textInputsObjInsert[i]->centerizedRender(Point2D<int>(975, 30), 260, window.getRenderer(), 0);
        }

        bmObjInsert.checkButtons();
        bmObjInsert.renderButtons(window.getRenderer());
        ip3.name = textInputsObjInsert[0]->getText();
        ip3.objPath = textInputsObjInsert[1]->getText();
        ip3.objSource = textInputsObjInsert[2]->getText();
        try
        {
            ip3.scale = stof(textInputsObjInsert[3]->getText());
        }
        catch (const std::invalid_argument &ia)
        {
            ip3.scale = 1;
        }
    }

    void renderFurnitureInteractionSpace(InputEvent &inputEvent, Window &window, ShapeManager &manager)
    {
        // TextBoxes:
        // 0 Nom du meuble
        // 2 Type du meuble
        // 4 Echelle

        textBoxesDefault[9]->update("", window.getRenderer()); // Remise à 0 de la textbox errors

        Furniture *curr = furnitures[fp.selected];

        text_furnitInteract[0]->update(curr->name, window.getRenderer());
        text_furnitInteract[2]->update(curr->type, window.getRenderer());

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << curr->scale;
        text_furnitInteract[4]->update(stream.str(), window.getRenderer());

        for (size_t i = 0; i < text_furnitInteract.size(); i++)
        {
            text_furnitInteract[i]->render(window.getRenderer(), 0, 0);
            text_furnitInteract[i]->center(Point2D<int>(970, 30), 270);
        }

        for (size_t i = 0; i < input_furnitInteract.size(); i++)
        {
            input_furnitInteract[i]->checkForInput(inputEvent, window.getRenderer());
            input_furnitInteract[i]->render(window.getRenderer(), 0);
        }

        fp.newName = input_furnitInteract[0]->getText();

        try
        {
            fp.newScale = stof(input_furnitInteract[1]->getText());
        }
        catch (const std::invalid_argument &ia)
        {
            fp.newScale = curr->scale;
        }

        bmFurnitInteract.checkButtons();
        bmFurnitInteract.renderButtons(window.getRenderer());
    }

    /**
     * @brief Check si un click de souris a touché l'un des meubles de la pièce et le marque comme selectionné
     */
    void checkFurnitureClick(Mouse mouse, ShapeManager &manager, Window &window)
    {
        float minx, maxx, miny, maxy;
        // Boucle sur tous les meubles de la pièce
        for (size_t i = 0; i < furnitures.size(); i++)
        {
            if (mouse.leftClick && manager.getShape(furnitures[i]->name).hit2D(Point2D<int>(mouse.x, mouse.y), Point2D<int>(30, 30), 900, 506))
            {
                interactSpace = 4;
                fp.selected = i;

                // Drag and drop en vue haut et face uniquement
                // if (((Camera::getCurrent().getCamId() == "topCam") || (Camera::getCurrent().getCamId() == "faceCam")) && mouse.moving)
                // {
                //     const float scale = (float)1.0 / (float)tan(60 / 2.0 * M_PI / 180); // 1.732
                //     // std::cout << "Mouse move: " << mouse.ymov << " | " << mouse.xmov << "\n";
                //     // std::cout << Vector(mouse.ymov, 0, mouse.xmov) / scale * manager.getShape(furnitures[i]->name).center.distance(Camera::getCurrent().getCameraPosition()) << '\n';
                //     maVar++;

                //     // Option1 : Warp sur le centre du meuble
                //     if (maVar % 5 == 0)
                //     {
                //         manager.getShape(furnitures[i]->name).getBoundingBox2D(minx, miny, maxx, maxy);
                //         SDL_WarpMouseInWindow(window.getWindow(), ((maxx + minx) / 2) + 30, ((maxy + miny) / 2) + 30);
                //     }
                //     else if (mouse.ymov < 10 && mouse.xmov < 10 && mouse.ymov > -10 && mouse.xmov > -10)
                //     {
                //         manager.getShape(furnitures[i]->name).move(Vector(mouse.ymov, 0, mouse.xmov));
                //     }
                // }

                return;
            }
        }
        // Pas sur un meuble ET pas dans le rectangle -> reset
        // Clic hors du rectangle : mouse.x < 970 || mouse.x > 1240 || mouse.y < 30 || mouse.y > 536
        // Clic dans la pipeline : Point2D<int>(30,30), 900, 506
        if (mouse.leftClick && (mouse.x > 30 && mouse.x < 930 && mouse.y > 30 && mouse.y < 536))
        {
            interactSpace = 0;
        }
    }

    bool checkWallCollision(ShapeManager &manager, Shape &object)
    {
        if (object.hit(manager.getShape("frontWall")))
            return false;
        if (object.hit(manager.getShape("backWall")))
            return false;
        if (object.hit(manager.getShape("leftWall")))
            return false;
        if (object.hit(manager.getShape("rightWall")))
            return false;
        return true;
    }

    bool checkFurnitureCollision(ShapeManager &manager, Shape &object)  {
        for (Furniture *fur : furnitures)
        {
            if (object.name == fur->name)
                continue;
            if (object.hit(manager.getShape(fur->name)))
                return false;
        }
        return true;
    }

    void moveFurniture(ShapeManager &manager, Keyboard &keyboard, int direction)  {
        Shape copiedShape(manager.getShape(furnitures[fp.selected]->name));
        Vector deplacement;
        if (Camera::getCurrent().getCamId() == "topCam" || Camera::getCurrent().getCamId() == "faceCam")  {
            switch (direction)
            {
            case 0: // Move Left
                deplacement = Vector(0, 0, -1);
                break;
            case 1: // Move Right
                deplacement = Vector(0, 0, 1);
                break;
            case 2: // Move Down
                deplacement = Vector(1, 0, 0);
                break;
            case 3: // Move Up
                deplacement = Vector(-1, 0, 0);
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "backCam")  {
            switch (direction)
            {
            case 0: // Move Left
                deplacement = Vector(0, 0, 1);
                break;
            case 1: // Move Right
                deplacement = Vector(0, 0, -1);
                break;
            case 2: // Move Down
                deplacement = Vector(-1, 0, 0);
                break;
            case 3: // Move Up
                deplacement = Vector(1, 0, 0);
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "droitCam")  {
            switch (direction)
            {
            case 0: // Move Left
                deplacement = Vector(1, 0, 0);
                break;
            case 1: // Move Right
                deplacement = Vector(-1, 0, 0);
                break;
            case 2: // Move Down
                deplacement = Vector(0, 0, 1);
                break;
            case 3: // Move Up
                deplacement = Vector(0, 0, -1);
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "gaucheCam")  {
            switch (direction)
            {
            case 0: // Move Left
                deplacement = Vector(-1, 0, 0);
                break;
            case 1: // Move Right
                deplacement = Vector(1, 0, 0);
                break;
            case 2: // Move Down
                deplacement = Vector(0, 0, -1);
                break;
            case 3: // Move Up
                deplacement = Vector(0, 0, 1);
                break;
            }
        }
        if (Camera::getCurrent().getCamId() == "topCam" || Camera::getCurrent().getCamId() == "faceCam" || Camera::getCurrent().getCamId() == "droitCam" || Camera::getCurrent().getCamId() == "gaucheCam" || Camera::getCurrent().getCamId() == "backCam")
        {
            if (keyboard.shift.pressed)
                manager.getShape(furnitures[fp.selected]->name).move(deplacement);
            else
            {
                copiedShape.move(deplacement);
                if (checkWallCollision(manager, copiedShape) && checkFurnitureCollision(manager, copiedShape))
                    manager.getShape(furnitures[fp.selected]->name).move(deplacement);
            }
        }
    }

    void moveCamera(int direction)  {
        Vector deplacement;
        if (Camera::getCurrent().getCamId() == "topCam")  {
            switch (direction)  {
            case 0: // Move Left
                deplacement = Vector(0, 0, -1);
                break;
            case 1: // Move Right
                deplacement = Vector(0, 0, 1);
                break;
            case 2: // Move Down
                deplacement = Vector(1, 0, 0);
                break;
            case 3: // Move Up
                deplacement = Vector(-1, 0, 0);
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "faceCam")  {
            switch (direction)  {
            case 0: // Move Left
                deplacement = Vector(0, 0, -1);
                break;
            case 1: // Move Right
                deplacement = Vector(0, 0, 1);
                break;
            default:
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "backCam")  {
            switch (direction)  {
            case 0: // Move Left
                deplacement = Vector(0, 0, 1);
                break;
            case 1: // Move Right
                deplacement = Vector(0, 0, -1);
                break;
            default:
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "droitCam")  {
            switch (direction)
            {
            case 0: // Move Left
                deplacement = Vector(1, 0, 0);
                break;
            case 1: // Move Right
                deplacement = Vector(-1, 0, 0);
                break;
            default:
                break;
            }
        }
        else if (Camera::getCurrent().getCamId() == "gaucheCam")  {
            switch (direction)
            {
            case 0: // Move Left
                deplacement = Vector(-1, 0, 0);
                break;
            case 1: // Move Right
                deplacement = Vector(1, 0, 0);
                break;
            default:
                break;
            }
        }
        
        Camera::getCurrent().moveLockedCam(deplacement);
    }

    static void exportImage(HomeDesign *hm)  {
        // Générer un path et nom de fichier en fonction de l'heure
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "../HM-Export_%d-%m-%y_%H-%M-%S");
        auto file = oss.str();
        hm->refRender.savePNG(file);

        hm->textBoxesDefault[9]->update("La capture d'écran '" + file + "' a été enregistrée.", hm->refWindow.getRenderer());
    };

    

    static void checkParamsEdition(initPack *i)
    {
        if (!i->mode)
        {
            // Mode avec murs
            if (i->w1.empty() || i->w3.empty())
            {
                *i->checkForError = 2;
                return;
            }

            try
            {
                stof(i->w1);
                stof(i->w3);
            }
            catch (const std::invalid_argument &ia)
            {
                *i->checkForError = 1; // Flottant invalide
                // Ajouter un if pour gérer les valeurs max.
                return;
            }
            *i->checkForError = 3; // Flottants valides !
            *i->start = 1;
        }
        else
        {
            *i->checkForError = 6; // Annulation
            *i->start = 2;         // Annulation
        }
    }

    static void editSceneParams(HomeDesign *hm)
    {
        int start = 0;
        int checkForError = 0;

        TextBox t_intro("Éditer les paramètres de la scène actuelle", "fonts/Calibri Bold.ttf", 30, black, Point2D<int>(0, 230), 1280, 40, hm->refWindow.getRenderer());
        int i_TLx = 440, i_TLy = 280;
        TextBox t_mur1("Mur 1", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, hm->refWindow.getRenderer());
        TextInput i_mur1(std::to_string(hm->w1), "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx + 60, i_TLy), 60, 25, hm->refWindow.getRenderer());
        TextBox t_mur2("Mur 2", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy + 45), 60, 20, hm->refWindow.getRenderer());
        TextInput i_mur2(std::to_string(hm->w1), "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx + 60, i_TLy + 45), 60, 25, hm->refWindow.getRenderer());

        i_TLx += 280;
        TextBox t_mur3("Mur 3", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy), 60, 20, hm->refWindow.getRenderer());
        TextInput i_mur3(std::to_string(hm->w3), "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx + 60, i_TLy), 60, 25, hm->refWindow.getRenderer());
        TextBox t_mur4("Mur 4", "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx, i_TLy + 45), 60, 20, hm->refWindow.getRenderer());
        TextInput i_mur4(std::to_string(hm->w3), "fonts/calibri.ttf", 20, black, Point2D<int>(i_TLx + 60, i_TLy + 45), 60, 25, hm->refWindow.getRenderer());

        TextBox t_error("", "fonts/calibri.ttf", 20, red, Point2D<int>(0, i_TLy + 80), 1000, 25, hm->refWindow.getRenderer());

        TextInput scene_name(hm->scene_name, "fonts/calibri.ttf", 21, black, Point2D<int>(440, 390), 400, 25, hm->refWindow.getRenderer());
        RectTextButton<initPack *> b_confirm("b_confirmEdits", Point2D<int>(440, 415), 400, 40, "Confirmer les modifications de nom et de surface", hm->refWindow);

        RectTextButton<initPack *> b_cancel("b_cancelEdits", Point2D<int>(440, 475), 400, 40, "Annuler les modifications", hm->refWindow);
        initPack i0 = {&start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), " ", 0};
        initPack i1 = {&start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), " ", 1};

        b_confirm.setAction(checkParamsEdition, &i0);
        b_cancel.setAction(checkParamsEdition, &i0);

        while (!start)  {
            // La touche Echap annule les modifications 
            if (hm->refKeyboard.escape.down)  {
                start = 2;
                break;
            }

            scene_name.checkForInput(hm->refInputEvent, hm->refWindow.getRenderer());
            i_mur1.checkForInput(hm->refInputEvent, hm->refWindow.getRenderer(), i_mur2);
            i_mur3.checkForInput(hm->refInputEvent, hm->refWindow.getRenderer(), i_mur4);
            i_mur2.checkForInput(i_mur1, hm->refWindow.getRenderer());
            i_mur4.checkForInput(i_mur3, hm->refWindow.getRenderer());

            hm->refInputEvent.update();

            hm->refInputEvent.updateMouse(hm->refMouse);
            hm->refInputEvent.updateKeyBoard(hm->refKeyboard);

            i0 = {&start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), " ", 0};
            i1 = {&start, &checkForError, i_mur1.getText(), i_mur3.getText(), scene_name.getText(), " ", 1};
            b_confirm.setParam(&i0);
            b_cancel.setParam(&i1);
            b_confirm.checkButton(hm->refInputEvent, Point2D<int>(0, 0));
            b_confirm.render(hm->refWindow.getRenderer());
            b_cancel.checkButton(hm->refInputEvent, Point2D<int>(0, 0));
            b_cancel.render(hm->refWindow.getRenderer());

            scene_name.centerizedRender(Point2D<int>(440, 375), 400, hm->refWindow.getRenderer(), 0);
            i_mur1.render(hm->refWindow.getRenderer(), 0);
            i_mur2.render(hm->refWindow.getRenderer(), 0);
            i_mur3.render(hm->refWindow.getRenderer(), 0);
            i_mur4.render(hm->refWindow.getRenderer(), 0);

            t_intro.centerizedRender(Point2D<int>(0, 0), 1280, hm->refWindow.getRenderer());
            t_mur1.render(hm->refWindow.getRenderer(), 0, 0);
            t_mur2.render(hm->refWindow.getRenderer(), 0, 0);
            t_mur3.render(hm->refWindow.getRenderer(), 0, 0);
            t_mur4.render(hm->refWindow.getRenderer(), 0, 0);
            if (checkForError)
            {
                switch (checkForError)
                {
                case 1:
                    t_error.update("Merci d'entrer un nombre flottant valide pour vos dimensions !", hm->refWindow.getRenderer());
                    break;
                case 2:
                    t_error.update("Merci d'entrer des dimensions valides en cliquant sur les barres de saisie !", hm->refWindow.getRenderer());
                    break;
                case 3:
                    t_error.update("Dimensions valides ! Rechargement de la pièce...", "fonts/calibri.ttf", 20, hd_greenButtons, hm->refWindow.getRenderer());
                    break;
                case 6:
                    t_error.update("Annulation de la saisie...", "fonts/calibri.ttf", 20, hd_greenButtons, hm->refWindow.getRenderer());
                    break;
                default:
                    t_error.update("Erreur innatendue !", hm->refWindow.getRenderer());
                    break;
                }
                t_error.center(Point2D<int>(0, i_TLy + 90), 1280);
                t_error.render(hm->refWindow.getRenderer(), 0, 0);
            }

            hm->refWindow.RenderScreen();
            hm->refWindow.FillScreen(hd_beigeBackground);
        }

        if (start == 2)
            return; // Annulation
        float w1 = 0, w3 = 0;
        w1 = stof(i_mur1.getText());
        w3 = stof(i_mur3.getText());
        if (w3 < w1)
            std::swap(w1, w3);
        hm->scene_name = i0.scene_name;

        if (w1 != hm->w1 || w3 != hm->w3)  {
            hm->w1 = w1;
            hm->w3 = w3;
            hm->surface = w1 * w3;
            hm->resetRoom(w1, w3);
        }
    }

    static void saveScene(HomeDesign *hm)
    {
        // Générer un path et nom de fichier en fonction de l'heure
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << "../" + hm->scene_name;
        int i = 1;
        while(FILE_EXISTS(oss.str() + ".3dh"))  {
            if (!FILE_EXISTS(oss.str()+"_"+std::to_string(i) + ".3dh"))  {
                oss << "_" + std::to_string(i);
                break;
            }
            i++;
        }
        // oss << std::put_time(&tm, "_%d-%m_%H-%M.3dh");
        std::ofstream outfile;
        outfile.open(oss.str() + ".3dh");

        outfile << "# HomeDesign Software\n";
        outfile << '\n';
        outfile << "# TITLE - Titre de la scène\n";
        outfile << "# WALLS - Dimensions définies des murs de la pièce\n";
        outfile << "# WOPTS - Options choisies pour le sol et les murs par défaut (99 si personnalisé)\n";
        outfile << "# TEXTF - Path vers la texture à appliquer au sol si WOPTS[0] = 99\n";
        outfile << "# TEXTW - Path vers les textures à appliquer aux murs si WOPTS[1] = 99\n";
        outfile << "# FURNI - Meuble unique : Nom | Type | Path | Obj source | Échelle\n";
        outfile << "# SHAPE - Shape associée au meuble : Nom | Coordonnées du centre | Visibilité | Rotation\n";
        outfile << '\n';

        outfile << "TITLE " << hm->scene_name << '\n';
        outfile << "WALLS " << hm->w1 << ' ' << hm->w3 << '\n';
        outfile << "WOPTS " << hm->fopt << ' ' << hm->wopt << '\n';
        outfile << "TEXTF\n"
                << "TEXTW" << '\n'; // Paths vers textures custom pour murs et sol
        outfile << '\n';

        for (Furniture *fur : hm->furnitures)
        {
            outfile << "FURNI " << fur->name << "|" << fur->type << "|" << fur->path << "|" << fur->source << "|" << fur->scale << '|' << fur->rotation << '\n';
        }
        outfile << '\n';
        for (Furniture *fur : hm->furnitures)
        {
            Shape copiedShape((hm->refManager).getShape(fur->name));

            outfile << "SHAPE " << fur->name << "|" << copiedShape.center.x << ' ' << copiedShape.center.y << ' ' << copiedShape.center.z << ' ';
            outfile << copiedShape.visible << '\n';
        }

        outfile.close();
        hm->textBoxesDefault[9]->update("La scène a correctement été exportée vers " + oss.str() + ".3dh", hm->refWindow.getRenderer());
    };
};

int HomeDesign::interactSpace = 0;
