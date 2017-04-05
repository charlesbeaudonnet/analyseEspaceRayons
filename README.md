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
- [ ] test
- [ ] testt
- [ ] Bien analyser le code
- [ ] Afficher les données stockées dans la structure des rayons
- [ ] Faire un executable pour afficher les arbres avec OpenGL
  - [ ] Makefile
  - [ ] Fonction de rasterization de ligne à partir de 2 points dans l'espace
  - [ ] autres..
