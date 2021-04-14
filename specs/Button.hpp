/**
 * @file Module Interaction utilisateur
 */

/** @brief class ButtonBase
 * La classe ButtonBase permet de prédéfinir les données héritables par la classe Button.
 * Cette classe ne contient ni les corps de fonctions utilisées dans Bouton, ni les paramètres template.
 */
class ButtonBase {
public:
	/** @brief name Nom du bouton. */
	const std::string name;
protected:

	/** @brief mouse Copie de l'input de la souris. */
	Mouse mouse;

	/** @brief clicked Indique si la souris a cliqué dans la zone selectionnable. */
	bool clicked = false;

	/** @brief selected Indique si la souris se trouve dans la zone selectionnable. */
	bool selected = false;

	/** @brief visible Indique si le bouton est visible ou non. */
	bool visible = false;

	/** @brief backgroundColor Couleur de fond du bouton. */
	Color backgroundCol;

	/** @brief contourCol Couleur de contour du bouton. */
	Color contourCol;

	/** @brief textBox Zone de texte optionnelle du bouton. */
	TextBox* textBox = nullptr;

	/** @brief signal Signal est un pointeur vers un booléen qui est mis a true si le bouton est cliqué. */ 
	bool* signal = nullptr;
public:

	/**
	 * @brief Garde le pointeur du booléen en paramètre comme signal.
	 * @param signal Référence vers le booléen.
	 */
	virtual void setSignal(bool& signal);

	/**
	 * @brief Met le pointeur du signal à nullptr.
	 */
	virtual void removeSignal();

	/**
	 * @brief Renvoie true si le bouton est cliqué, false autrement.
	 */
	virtual bool isClicked()const;

	/**
	 * @brief Renvoie true si la souris est au-dessus du bouton, false autrement.
	 */
	virtual bool isSelected()const;

	/**
	 * @brief Lance l'action enregistrée.
	 */
	virtual void playAction();

	/**
	 * @brief Affiche le bouton.
	 * @param renderer Pinceau graphique SDL.
	 */
	virtual void render(SDL_Renderer* renderer) const;

	/**
	 * @brief Vérifie si la souris se trouve sur le bouton avec le input utilisateur mis à jour.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	virtual bool mouseInside(const InputEvent& ie);

	/**
	 * @brief Vérifie si la souris clique sur le bouton avec le input utilisateur mis à jour.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	virtual bool mouseClickInside(const InputEvent& ie);

	/**
	 * @brief Vérifie toutes les données input utilisateur liées à la souris.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	virtual void checkButton(const InputEvent& inputEvent);

protected:
	/**
	 * @brief Constructeur accessible uniquement via les classes dérivées.
	 * @param name Nom unique du bouton.
	 * @param bgCol Couleur d'arrière-plan.
	 * @param contCol Couleur de contour.
	 */
	ButtonBase(const std::string& name, const Color& bgCol, const Color& contCol)
		: name(name), backgroundCol(bgCol), contourCol(contCol);

	// suppression du  constrcuteur de base
	ButtonBase() = delete;
};

/** @brief class Button
 * Extension de la classe ButtonBase qui contient les paramètres template et les corps de fonctions surchargées.
 * @param paramType Type du paramètre pour le pointeur de fonction.
 */
template <class paramType>
class Button : public ButtonBase {
protected:
	/** @brief fun Pointeur de fonction void avec un argument template. */
	void (*fun)(paramType t) = nullptr;

	/** @brief param Paramètre pour la fonction fun. */
	paramType param = NULL;

public:
	/**
	 * @brief Garde le pointeur du booléen en paramètre comme signal.
	 * @param signal Référence vers le booléen.
	 */
	void setSignal(bool& signal);

	/**
	 * @brief Met le pointeur du signal à nullptr.
	 */
	void removeSignal();

	/**
	 * @brief Renvoie true si le bouton est cliqué, false autrement.
	 */
	bool isClicked() const;

	/**
	 * @brief Renvoie true si la souris est au-dessus du bouton, false autrement.
	 */
	bool isSelected() const;

	/**
	 * @brief Prend un pointeur de fonction et son paramètre et l'enregistre.
	 * @param func Pointeur de fonction.
	 * @param parameter Paramètre template.
	 */
	void setAction(void (*func)(paramType p), paramType parameter);

	/**
	 * @brief Prend un pointeur de fonction.
	 * @param func Pointeur de fonction.
	 */
	void setAction(void (*func)(paramType p));

	/**
	 * @brief Prend un paramètre template pour l'assigner à la fonction courante stockée.
	 * @param parameter Paramètre template.
	 */
	void setParam(paramType parameter);

	/**
	 * @brief Lances la fonction stockée dans le pointeur de fonction avec le paramètre donné.
	 * @param parameter Paramètre template.
	 */
	void playAction(paramType param);

	/**
	 * @brief Lances la fonction stockée dans le pointeur de fonction avec le paramètre enregistré.
	 */
	void playAction();

	/**
	 * @brief Affiche le bouton.
	 * @param renderer Pinceau graphique SDL.
	 */
	virtual void render(SDL_Renderer* renderer) const;

	/**
	 * @brief Vérifie si la souris se trouve sur le bouton avec le input utilisateur mis à jour.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	virtual bool mouseInside(const InputEvent& ie);

	/**
	 * @brief Vérifie si la souris clique sur le bouton avec le input utilisateur mis à jour.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	virtual bool mouseClickInside(const InputEvent& ie);

	/**
	 * @brief Vérifie toutes les données input utilisateur liées à la souris.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	void checkButton(const InputEvent& inputEvent);

protected:
	/**
	 * @brief Constructeur accessible uniquement via les classes dérivées.
	 * @param name Nom unique du bouton.
	 * @param bgCol Couleur d'arrière-plan.
	 * @param contCol Couleur de contour.
	 */
	Button(const std::string& name, const Color& bgCol, const Color& contCol)
		: ButtonBase(name,bgCol,contCol);

	// suppression du  constrcuteur de base
	Button() = delete;
};

/** @brief class RectButton
 * Extension de la classe Button qui contient certaines fonctions surchargées pour un bouton rectangulaire.
 * @param paramType Type du paramètre pour le pointeur de fonction.
 */
template <class paramType>
class RectButton : public Button<paramType> {
public:

	/** @brief pos Position haut-gauche de bouton. */
	Point2D pos;

	/** @brief width Largeur du bouton. */
	int width;

	/** @brief height Hauteur du bouton. */
	int height;

	/**
	 * @brief Constructeur d'un bouton rectangulaire.
	 * @param name Nom unique du bouton.
	 * @param bgCol Couleur d'arrière-plan.
	 * @param contCol Couleur de contour.
	 * @param pos Position haut-gauche.
	 * @param width Largeur.
	 * @param height Hauteur.
	 */
	RectButton(const std::string& name, const Color& bgCol, const Color& contCol,
		const Point2D& pos, const int& width, const int& height)
		: Button(name, bgCol, contCol),
		pos(pos), width(width), height(height);

	// suppression du  constructeur de base
	RectButton() = delete;

	/**
	 * @brief Affiche le bouton.
	 * @param renderer Pinceau graphique SDL.
	 */
	void render(SDL_Renderer* renderer) const;

	/**
	 * @brief Vérifie si la souris se trouve sur le bouton avec le input utilisateur mis à jour.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	bool mouseInside(const InputEvent& ie);
};

/** @brief class RoundButton
 * Extension de la classe Button qui contient certaines fonctions surchargées pour un bouton rond.
 * @param paramType Type du paramètre pour le pointeur de fonction.
 */
template <class paramType>
class RoundButton : public Button<paramType> {
public:

	/** @brief pos Position du centre du bouton. */
	Point2D pos;

	/** @brief radius Radius du cercle. */
	int radius;

	/**
	 * @brief Constructeur d'un bouton rond.
	 * @param name Nom unique du bouton.
	 * @param bgCol Couleur d'arrière-plan.
	 * @param contCol Couleur de contour.
	 * @param pos Position du centre.
	 * @param radius Radius.
	 */
	RoundButton(const std::string& name, const Color& bgCol, const Color& contCol,
		const Point2D& pos, const int& radius)
		: Button(name, bgCol, contCol),
		pos(pos), radius(radius);

	// suppression du  constructeur de base
	RoundButton() = delete;

	/**
	 * @brief Affiche le bouton.
	 * @param renderer Pinceau graphique SDL.
	 */
	void render(SDL_Renderer* renderer) const;

	/**
	 * @brief Vérifie si la souris se trouve sur le bouton avec le input utilisateur mis à jour.
	 * @param ie Input utilisateur mis à jour constamment.
	 */
	bool mouseInside(const InputEvent& ie);
};


/** @brief class ButtonManager
 * Permet d'ajouter des boutons sans limite de scope autre que la classe elle-même.
 * Permet de gérer différents types de boutons avec des types de variables différents.
 */
class ButtonManager {
private:
	/** @brief inputEvent Reference vers le InputEvent donné à la création de la classe. */
	const InputEvent& inputEvent;

	/** @brief buttons Vecteur de pointeurs uniques vers les boutons. */
	std::vector<std::unique_ptr<ButtonBase>> buttons;

public:
	/**
	 * @brief Constructeur.
	 * @param inputEvent Event de l'input utilisateur.
	 */
	ButtonManager(const InputEvent& inputEvent) : inputEvent(inputEvent);

	// suppression du  constructeur de base
	ButtonManager() = delete;

private:
	/**
	 * @brief Renvoie true si un bouton avec le nom donné existe déjà, sinon false.
	 * @param name Nom demandé.
	 */
	bool nameUsed(const std::string& name) const;

public:
	/**
	 * @brief Ajoute un bouton rectangulaire dans le vecteur avec les paramètres donnés.
	 */
	template <class paramType>
	void addButton(const std::string& name, const Color& bgCol, const Color& contCol, const Point2D& pos, const int& width, const int& height);

	/**
	 * @brief Ajoute un bouton rond dans le vecteur avec les paramètres donnés.
	 */
	template <class paramType>
	void addButton(const std::string& name, const Color& bgCol, const Color& contCol, const Point2D& pos, const int& radius);

	/**
	 * @brief Retire le bouton qui porte le nom donné du vecteur.
	 * @param name Nom du bouton à supprimer.
	 */
	void removeButton(const std::string& name);

	/**
	 * @brief Appelle les fonctios render() des boutons du vecteur.
	 * @param renderer Pinceau graphique SDL.
	 */
	void renderButtons(SDL_Renderer* renderer) const;

	/**
	 * @brief Appelle les fonctios checkButton() des boutons du vecteur.
	 */
	void checkButtons() const;

	/**
	 * @brief Renvoie une référence vers le bouton qui porte le nom demandé.
	 */
	template <class paramType>
	Button<paramType>& getButton(const std::string& name) const;
};