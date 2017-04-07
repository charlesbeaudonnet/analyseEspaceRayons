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
- [ ] Faire un executable pour afficher les arbres avec OpenGL
  - [ ] Makefile
  - [ ] Fonction de rasterization de ligne à partir de 2 points dans l'espace
  - [ ] autres..

g++ helloworld.cpp -o testGL -lglut -lGL -std=c++11

# compte rendu 7 avril

Q&A:
Q: Qu'est-ce que comment ça marche la lumière ??
A: La lumière a pas de coordonées, on utilise la direction d'un vecteur pour savoir s'il touche un objet ou la lumière

Q: Radium ça compile paaaaaaaaaaaas !
A: Essayer mieux, sinon, utiliser tungsten ou g3d.

Q: c'est quoi beta ?
A: un coef d'importance pour savoir si ça vaut le coup de continuer a tracer le rayon ou pas (probablement, maybe)

-Récupération du livre de pbrt v1

-Prise de contact avec sofian

-Pas de nouvelle tâches assignées

-Mise en place d'une réunion hebdomadaire le lundi qui commence le mardi 18 avril à 14h30

