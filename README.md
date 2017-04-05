# analyseEspaceRayons

# pour telecharger le git
git clone --recursive https://github.com/charlesbeaudonnet/analyseEspaceRayons

# pour utiliser brew (pour compiler sur les machines de la fac)
PATH="$HOME/.linuxbrew/bin:$PATH"

# pour compiler le code
cd analyseEspaceRayons/executable

cmake ../pbrt-v3

make -j4

# à faire:-
- [ ] test
- [ ] testt
