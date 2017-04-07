#include <GL/glut.h>
#include "viewer.h"

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    
    Viewer * v= new Viewer(1080,720,"HelloWorld!");
    v->line(Point(50,300),Point(350,400));
    v->line(Point(50,300),Point(300,50));
    v->line(Point(350,400),Point(300,50));
    glutMainLoop();
    return 0;
}