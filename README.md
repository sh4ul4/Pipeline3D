# Moteur de rendu graphique 3D (Pipeline 3D)

Projet IN608 de licence 3 Informatique. 

## Contenu de l'archive

Cette archive contient :
- Le compte rendu `.pdf` de 10 pages de notre projet ;
- Un listing commenté `.pdf` de l'ensemble des fichiers composant le programme ;
- Un Manuel d'utilisation `.pdf` pour le mail test de Pipeline, les applications Systeme stellaire et Aménagement intérieur ;
- Le répertoire `headers/` contenant les fichiers sources du programme ;
- Le répertoire `main/` contenant le fichier main test de la Pipeline ;
- Le répertoire `HomeDesign/` contenant les fichiers de l'application Aménagement Interieur ;
- Le répertoire `StarSystem/` contenant les fichiers de l'application Systeme stellaire ;
- Le fichier `Doxyfile` responsable de la génération de la documentation ; 
- Un mécanisme de construction `Makefile` ;
- Les répertoires `fonts/`, `OBJ/`, `HM-Res` contiennent des ressources nécessaires à la Pipeline :
- Ainsi que ce présent fichier `README.md`


## Commandes et utilisation

1. Exécuter le main de test de Pipeline :

```sh
 make
```

2. Exécuter l'application Aménagement intérieur :

```sh
make homedesign
```

3. Exécuter l'application Système Stellaire :

```sh
make starsystem
```

4. Générer la documentation

```sh
make documentation
```
