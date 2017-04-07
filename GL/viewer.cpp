#include <GL/glut.h>
#include "viewer.h"

Viewer::Viewer(unsigned int w, unsigned int h, char const* title){
      width=w;
      height=h;
      glutInitDisplayMode(GLUT_SINGLE);
      glutInitWindowSize(width, height);
      glutInitWindowPosition(100, 100);
      glutCreateWindow(title);
      glClear(GL_COLOR_BUFFER_BIT);
}

void Viewer::line(Point p, Point p2){
  glLineWidth(1);
  glColor3f(1.0,.0,.0);
  float w= width/2.0, h=height/2.0;
  glBegin(GL_LINES);
    glVertex2f(((float)p.x-w)/(w), ((float)p.y-h)/h);
    glVertex2f(((float)p2.x-w)/w, ((float)p2.y-h)/h);
  glEnd();
  glFlush();
}