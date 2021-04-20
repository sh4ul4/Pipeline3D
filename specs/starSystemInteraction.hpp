class starSystemInteraction
{
private:
    std::vector<TextBox> texts; // Tous les textes visibles sur l'interface utilisateur
    std::vector<Button> buttons; // Tous les boutons visibles sur l'interface utilisateur
    // Ajouter des switchs dans un vector
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
};
