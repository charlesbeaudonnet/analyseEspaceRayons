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

# depanage à domicile du jeudi 20 avril

Problème :

-Soit on a le compileur installé sur le pc qui est trop vieux

-Soit on a un compilateur récent mais qui ne trouve pas les librairies GL


Solution :

-pour le compilateur trop vieux on peut rien faire

-pour le compilateur qui trouve pas les lib,

on a la méthode brute : copier coller dans son dossier de lib toutes les libs qu'il ne trouve pas


y'a aussi la méthode maline (qu'on utilise pas parce que yolo) :

dire au ld de linuxbrew d'aller chercher les librairies dans le fichier où elles sont.

info ici : http://stackoverflow.com/questions/9922949/how-to-print-the-ldlinker-search-path

-------------------

Pour l'instant ça a l'air de marcher mais il faut copier les dépendances des lib GL maintenant

-fichier des lib de base : /usr/lib64

-fichier des lib de linuxbrew : /home/21603498/.linuxbrew/lib64


Librairies qu'on a déplacé :

- openGL :

toutes les lib qui commence par libGL avec la commande (depuis le dossier des lib de linuxbrew) : 

[21603498@u3202-08 lib64]$ cp /usr/lib64/libGL* .

- les dépendances de 

    openGL :

libXdamage.so.1, needed by /home/21603498/.linuxbrew/lib/libGL.so, not found (try using -rpath or -rpath-link)

/home/21603498/.linuxbrew/bin/ld: warning: libXfixes.so.3, needed by /home/21603498/.linuxbrew/lib/libGL.so, not found (try using -rpath or -rpath-link)

/home/21603498/.linuxbrew/bin/ld: warning: libXxf86vm.so.1, needed by /home/21603498/.linuxbrew/lib/libGL.so, not found (try using -rpath or -rpath-link)

/home/21603498/.linuxbrew/bin/ld: warning: libdrm.so.2, needed by /home/21603498/.linuxbrew/lib/libGL.so, not found (try using -rpath or -rpath-link)

/home/21603498/.linuxbrew/bin/ld: warning: libselinux.so.1, needed by /home/21603498/.linuxbrew/lib/libGL.so, not found (try using -rpath or -rpath-link)

    Qt5.7 :

/home/21603498/.linuxbrew/bin/ld: warning: libgthread-2.0.so.0, needed by /home/21603498/Qt5.7.0/5.7/gcc_64/lib/libQt5Core.so.5.7.0, not found (try using -rpath or -rpath-link)

/home/21603498/.linuxbrew/bin/ld: warning: libglib-2.0.so.0, needed by /home/21603498/Qt5.7.0/5.7/gcc_64/lib/libQt5Core.so.5.7.0, not found (try using -rpath or -rpath-link)


[21603498@u3202-08 lib64]$ cp /usr/lib64/libglapi.so.0 .

[21603498@u3202-08 lib64]$ cp /usr/lib64/libXdamage.so.1 .

[21603498@u3202-08 lib64]$ cp /usr/lib64/libXfixes.so.3 .

[21603498@u3202-08 lib64]$ cp /usr/lib64/libXxf86vm.so.1 .

[21603498@u3202-08 lib64]$ cp /usr/lib64/libdrm.so.2 .

[21603498@u3202-08 lib64]$ cp /usr/lib64/libselinux.so.1 .



[21603498@u3202-08 lib64]$ cp /usr/lib64/libgthread-2.0.so.0 .

[21603498@u3202-08 lib64]$ cp /usr/lib64/libglib-2.0.so.0 .



- les dépendances de libselinux.so.1 :

[21603498@u3202-08 lib64]$ cp /usr/lib64/libpcre.so.1 .



Tout a l'air de compiler bien mais nouveau problème :

Y'a pas l'exécutable, il est sensé être dans un dossier "bin",

mais y'a pas de dossier bin.



-------------------



ptetre jeter un coup d'oeil à :

-Wl,-rpath,::::::::::: -lGLU -lGL -pthread



(c'est les arguments donnés au compilateur juste avant qu'il plante)



-------------------



Commandes sympa :



lscpu : 

pour avoir des infos sur le CPU



VERBOSE=1 make :

pour avoir un make un peu plus explicite



make -j4 :

pour paralléliser le make (utiliser les 4 coeurs du CPU)



export :

je sais pas : rtfm



ls -l /lib64/*