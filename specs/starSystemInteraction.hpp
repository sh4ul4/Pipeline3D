class starSystemInteraction
{
private:
    Window window; // Fenêtre
    TextBox title;
    Keyboard keyboard; // Toutes les touches du clavier
    Mouse mouse; // Prend en charge la souris
    ButtonManager buttons;
    std::vector<TextInput> inputs;
    std::vector<TextBox> texts; // Tous les textes visibles sur l'interface utilisateur

public:
    // Constructeur
    starSystemInteraction();

    // En cliquant sur le bouton 'create', on créer un nouvel astre. On récupère d'abord les infos renseignées par l'utilisateur
    void addLuminary();
    // Supprime une planète
    void deleteLuminary(int i);
    // Change la visibilité d'une planète
    void changeVisibility();
    // Sauvegarde les planètes dans un fichier .obj
    void save();
    // Charge un fichier .obj
    void load();
    // Réinitialise le système stellaire
    void reset();
    // Arrête ou reprend la simulation
    void stopAndStart();
};
