#include "mouselabel.h"
#include <QDebug>
#include <QPainter>
#include <QPoint>

MouseLabel::MouseLabel(QWidget *parent) : QLabel(parent)
{

}

void MouseLabel::loadImg(QImage image) {
    imgX = image.width();
    imgY = image.height();
    posx = 0;
    posy = 0;
    zoom = 1.;
    picOpened = true;
    imageOrig = image;
    imageOrig.detach();
    setText("");
}

void MouseLabel::mousePressEvent(QMouseEvent *ev)
{
    if(picOpened) {
        if (ev->button() == Qt::LeftButton) {
            int x, y;

            x = (ev->x() - posx) / zoom;
            y = (ev->y() - posy) / zoom;

            if(x >= 0 && y >= 0 && x < imgX && y < imgY) {
                mx = x;
                my = y;

                emit mousePressed();
            }
        } else if (ev->button() == Qt::RightButton) {
            this->setCursor(Qt::OpenHandCursor);
            RCDown = true;
            RCPos = mapFromGlobal(ev->globalPos());

        }
    }
}

void MouseLabel::mouseMoveEvent(QMouseEvent *ev) {
    QPoint pos = mapFromGlobal(ev->globalPos());
    //si le clic droit est enfoncé
    if (RCDown) {
        this->setCursor(Qt::ClosedHandCursor);
        //si la souris a bougé
        if (pos != RCPos) {
            QPoint diff = (pos - RCPos);

            //on deplace l'image d'autant que la souris a bougé
            posx += diff.x();
            if (imgX * zoom < width())
                posx = qBound(0, posx, (int) (width() - imgX * zoom) );
            else
                posx = qBound( (int) (-imgX * zoom + (width() - 100) ), posx, 100);

            posy += diff.y();
            if (imgY * zoom < height())
                posy = qBound(0, posy, (int) (height() - imgY * zoom) );
            else
                posy = qBound( (int) (-imgY * zoom + (height() - 100) ), posy, 100);

            //on repeint le widget
            repaint();
        }
    }

    RCPos = pos;
    if(!RCDown && pos.x() > posx && pos.x() < posx + imgX * zoom && pos.y() > posy && pos.y() < posy + imgY * zoom) {
        emit mouseMoved();
    } else {
        emit mouseOut();
    }
}

void MouseLabel::leaveEvent(QEvent *)
{
    emit mouseOut();
}

void MouseLabel::mouseReleaseEvent(QMouseEvent *) {
    this->setCursor(Qt::CrossCursor);
    RCDown = false;
}

void MouseLabel::paintEvent(QPaintEvent *ev) {
    if(picOpened) {
        //qInfo() << "Painting ...";

        QPixmap newMap = QPixmap::fromImage(imageOrig);

        //if faut utiliser le painter dans le paintEvent
        //car toute autre modification va creer un nouveau paintEvent et creer une boucle infinie
        QPainter painter(this);

        painter.drawPixmap(posx, posy, imgX * zoom, imgY * zoom, newMap);
        painter.end();
    }
    QLabel::paintEvent(ev);
}


void MouseLabel::wheelEvent(QWheelEvent *ev) {
    if(picOpened) {
        //on enregistre l'endroit où le scroll a été effectué dans le cadre
        scrollx = ev->x();
        scrolly = ev->y();

        //zoomx et zoomy sont les coordonnées du pixel de la photo sur lequel
        //le scroll a ete effectue
        if (ev->delta() > 0) {
            zoom *= 1.2;
            zoomx = (ev->x() - posx) * 1.2;
            zoomy = (ev->y() - posy) * 1.2;
        } else
            if(zoom > 1) {
                zoom /= 1.2;
                zoomx = (ev->x() - posx) / 1.2;
                zoomy = (ev->y() - posy) / 1.2;
            } else {
                zoomx = (ev->x() - posx);
                zoomy = (ev->y() - posy);
            }

        posx = -zoomx + scrollx;

        if (imgX * zoom < width())
            posx = qBound(0, posx, (int) (width() - imgX * zoom) );
        else
            posx = qBound( (int) (-imgX * zoom + (width() - 100) ), posx, 100);

        posy = -zoomy + scrolly;

        if (imgY * zoom < height())
            posy = qBound(0, posy, (int) (height() - imgY * zoom) );
        else
            posy = qBound( (int) (-imgY * zoom + (height() - 100) ), posy, 100);

        repaint();
    }
}
