#include <GL/glut.h>

static unsigned int width = 1080;
static unsigned int height = 720;

void line(unsigned int x, unsigned int y, unsigned int xx, unsigned int yy){  
  glLineWidth(1);
  glColor3f(1.0,.0,.0);
  float w= width/2.0, h=height/2.0;
  glBegin(GL_LINES);
    glVertex2f(((float)x-w)/(w), ((float)y-h)/h);
    glVertex2f(((float)xx-w)/w, ((float)yy-h)/h);
  glEnd();
  glFlush();
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Hello world :D");
    glClear(GL_COLOR_BUFFER_BIT);
    
    line(10,100,20,200);
    
    glutMainLoop();
    return 0;
}