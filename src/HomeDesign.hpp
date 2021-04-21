#pragma once
#include "headers.hpp"

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
    Button b_remove;
    Button b_rotate;
    Button b_rename;
    TextInput t_rename;

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
    void dragSprite(Mouse mouse, HomeDesign hm);

    /**
     * @brief Supprime le meuble et la Shape géométrique correspondante suite au clic sur bouton remove
     */ 
    void remove();
    ~Furniture();
};


/**
 * @brief Meuble par défaut issu de la classe Furniture : 
 *        Parallépipède simple surélevé d'une hauteur de pieds
 */ 
class f_Foot: class Furniture {
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
}


/**
 * @brief Meuble par défaut issu de la classe Furniture : 
 *        Chaise (parallélépipède rectangle surélevé d'une hauteur de pieds et dossier)
 */ 
class f_Chair: class Furniture {
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



    /*===========================================================================================
     *      INTERFACE GRAPHIQUE
    ===========================================================================================*/
    Window interface;
    Textbox title;
    Keyboad keyboard;
    Mouse mouse;

	ButtonManager bm(inputEvent);

    /**
     * @brief Boutons relatifs au changement de Vue Caméra pour la scène 
     */
    Button b_mainView;
    Button b_view1;
    Button b_view2;
    Button b_view3;
    Button b_view4;
    Button b_freeMotionView;

    /**
     * @brief Boutons relatifs à l'insertion de meuble
     * 
     * Chaque bouton est lié à la méthode insertFurniture() en spécifiant son numéro
     */
    Button b_insertDefault;
    Button b_insertDefault2;
    Button b_insertDefault3;
    
    /**
     * @brief Boutons d'interaction globale avec l'application 
     */
    Button b_editSurface;
    Button b_save;
    Button b_export;
    Button b_quit;

public:
    HomeDesign() = delete;

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param InputEvent    Utile aux évènements d'interaction utilisateur
     * @param float         Dimensions des 4 murs formant la pièce principale pour créer les rectangles
     */ 
    HomeDesign(ButtonManager& bm, float w1, float w2, float w3, float w4);

    /**
     * @brief Constructeur par défaut, appelé à la création d'une scène sur l'interface (1)
     * 
     * @param ButtonManager Utile aux évènements d'interaction utilisateur
     * @param path          Fichier à importer pour initialiser la scène
     */ 
    HomeDesign(ButtonManager& bm, std::string path);

    ~HomeDesign();

private:

    /**
     * @brief Méthode appelée lorsque les boutons d'insertion de meuble sont pressés
     * 
     * @param int   Représente le numéro du bouton (entre 1 et 3)
     */ 
    void insertFurniture(int style);

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

    void editSurface()

    void saveScene();

    void exportImage();

}
