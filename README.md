# analyseEspaceRayons

# pour telecharger le git
git clone https://github.com/charlesbeaudonnet/analyseEspaceRayons

# pour utiliser brew (pour compiler sur les machines de la fac)
PATH="$HOME/.linuxbrew/bin:$PATH"

# pour compiler le code
cd analyseEspaceRayons/executable

cmake ../pbrt-v3

make -j4

# exemple d'execution depuis executable
./pbrt --quick ../../../pbrt-v3-scenes/dragon/f8-4a.pbrt

# à faire:-
- [ ] Bien analyser le code
- [ ] Afficher les données stockées dans la structure des rayons
- [x] Faire un executable pour afficher les arbres avec OpenGL
  - [x] Makefile
  - [x] Fonction de rasterization de ligne à partir de 2 points dans l'espace
  - [ ] autres..

g++ helloworld.cpp -o testGL -lglut -lGL -std=c++11

# compte rendu 7 avril

Q&A:
Q: Comment fonctionne sur pbrt la lumière ?
A: La lumière n'a pas de coordonnées, on utilise la direction d'un rayon pour savoir s'il touche un objet ou la lumière

Q: Encore des problème de compilation de Radium, problème de linkage de librairies.
A: Essayer hors du dossier "Documents". Si Radium est vraiment impossible a compiler, utiliser tungsten ou g3d.

Q: C'est quoi la variable beta utilisée un peu partout dans le code ?
A: C'est (presque sûr) un coefficient d'importance pour savoir si ça vaut le coup de continuer a tracer le rayon ou pas.

-Récupération du livre de pbrt v1
-Prise de contact avec sofian
-Mise en place d'une réunion hebdomadaire le lundi qui commence le mardi 18 avril à 14h30

Ce qu'on a fait jusque là :
Récupération des chemins des rayons pour plusieurs integrators différents sur pbrt.

# compte rendu 18 avril

-D'autres problèmes de compilation de Radium :
Le programme de test utilise un librairie indisponible. Solution : désactiver les tests.

-On arrive pas non plus a compiler tungsten ni g3d.

Prochaine réu : lundi 1 mai à 10h

