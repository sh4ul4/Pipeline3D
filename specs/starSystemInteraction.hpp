class starSystemInteraction
{
private:
    Window window; // Fenêtre
    TextBox title;
    Keyboard keyboard; // Toutes les touches du clavier
    Mouse mouse; // Prend en charge la souris

    std::vector<TextBox> texts; // Tous les textes visibles sur l'interface utilisateur
    std::vector<Button> buttons; // Tous les boutons de l'interface utilisateur
    std::vector<CheckBox> checkBoxes; // Tous les switchs de l'interface utilisateur
    // Ajouter l'écran où l'on verra le système stellaire

public:
    // Constructeur
    starSystemInteraction();

    // En cliquant sur le bouton 'create', on créer un nouvel astre. On récupère d'abord les infos renseignées par l'utilisateur
    void addLuminary();
    // Supprime une planète
    void deleteLuminary();
    // Change la visibilité d'une planète
    void changeVisibility();
    // Sauvegarde les planètes dans un fichier .obj
    void saveOBJ();
    // Charge un fichier .obj
    void loadOBJ();
    // Réinitialise le système stellaire
    void reset();
    // Arrête ou reprend la simulation
    void stopAndStart();
};
