#include <string>
#include <vector>
#include "../Window.hpp"
#include "../Textbox.hpp"
#include "../TextInput.hpp"
#include "../Button.hpp"


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
     */ 
    void dragSprite(Mouse mouse);

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
class f_Table: class Furniture {
private:
    // [?]
    // Vector de 4 pieds + 1 corps pour former un meuble ? 
    // Possibilité de les rattacher en une seule forme ?
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
    f_Table(std::string shape_name, float ft_height, float height, float lenght, float large, Bitmap *bmp = nullptr);
}


class HomeDesign {
private:
    /**
     * @brief Nom de la scène donné à l'ouverture de l'application 
     */
    std::string scene_name;

    /**
     * @brief Vector ensemble de 4 (ou plus) rectangles représentant 
     *        des murs délimitant la pièce intérieur
     */ 
    std::vector<Rectangle> walls; 
    
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

    Button b_new;
    Button b_save;
    Button b_import;
    Button b_quit;

public:
    /**
     * @brief Constructeur par défaut, contient la boucle principale de l'application
     */ 
    HomeDesign();

    addFurniture(Furniture f);

    void export(); 

    // [?]

    ~HomeDesign();
}
