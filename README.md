# Project Mansuba

La page de spécification du projet :

<https://www.labri.fr/perso/renault/working/teaching/projets/2022-23-S5-Mansuba.php>

La page du projet sur la forge :

https://thor.enseirb-matmeca.fr/ruby/projects/projetss5

La page des projets en général :

<https://www.labri.fr/perso/renault/working/teaching/projets/projets.php>

# Pour compiler le projet en un executable :

"make" compile un plateau de taille 8*8

"make WIDTH=X WEIGHT=Y" compile un plateau de taille X*Y

# Pour compiler le test et l'executer :

"make test"

# Pour compiler le rapport et l'executer:

"make pdf"

! A compiler deux fois pour avoir les références et la table des matières !

# Pour nettoyer le projet

"make clean"

# Pour executer le projet :

"./project [OPTION D'EXECUTION]"

**Liste des options d'execution** :

(sans option): Partie standard avec victoire simple, MAX_TURN = 2*WORLD_SIZE, et n'affiche que le vainqueur et le nombre de tour

-s X: Change la seed du generateur aleatoire a X

-m X: Regle MAX_TURN a X

-t X: Regle la condition de victoire a simple si X = s ou complexe si X = c

-p: Affiche tous les details de la partie (plateau, mouvements, grille en cours, etc)

-c X: Choisit le mode de jeu en fonction de X

-d X: Rajout un delai de X microsecondes entre chaque coup (utile pour la visualitation en direct de grande partie)

**Detail des modes de jeu** :

- 0 : mode avec toutes les fonctionnalités (capture, liberation, changements de relation, tous les types de pièces);

- 1 : Jeu de dames chinoises

- 2 : Jeu déchecs
