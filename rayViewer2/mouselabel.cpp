#include "mouselabel.h"

MouseLabel::MouseLabel(QWidget *parent) : QLabel(parent)
{

}

void MouseLabel::mousePressEvent(QMouseEvent *ev)
{
    this->mx = ev->x();
    this->my = ev->y();
    emit mousePressed();
}
