#include "mouselabel.h"
#include <QDebug>
#include <QPainter>
#include <QPoint>

/**
 * @brief MouseLabel constructeur
 * @param parent
 */
MouseLabel::MouseLabel(QWidget *parent) : QLabel(parent)
{

}

/**
 * @brief loadImg permet de charger une image dans le mouse label
 * @param image image à charger
 */
void MouseLabel::loadImg(QImage image) {
    imgX = image.width();
    imgY = image.height();
    posx = 0;
    posy = 0;
    zoom = 1.;
    picOpened = true;
    imageOrig = image;
    //detach permet de "cloner" l'image
    //c'est peut être pas nécessaire, on a pas testé
    imageOrig.detach();
    setText("");
}

/**
 * @brief mousePressEvent fonction qui gère le clic de souris
 * elle envoie un signal a mainwindow si le clic a été fait sur l'image
 * @param ev
 */
void MouseLabel::mousePressEvent(QMouseEvent *ev)
{
    //ne fait rien si pas d'image ouverte
    if(picOpened) {
        //gestion du clic gauche pour la selection d'un pixel
        if (ev->button() == Qt::LeftButton && !RCDown) {
            int x, y;

            //On calcule la position du clic par rapport à l'image
            x = (ev->x() - posx) / zoom;
            y = (ev->y() - posy) / zoom;

            //si le clic est sur l'image, on le signale à mainwindow
            if(x >= 0 && y >= 0 && x < imgX && y < imgY) {
                mx = x;
                my = y;

                emit mousePressed();
            }
        //gestion du clic droit pour déplacer l'image
        } else if (ev->button() == Qt::RightButton) {
            this->setCursor(Qt::OpenHandCursor);
            RCDown = true;
            RCPos = mapFromGlobal(ev->globalPos());

        }
    }
}

/**
 * @brief mouseMoveEvent Override de l'event handler du déplacement de la souris
 * permet de déplacer l'image avec le clic droit et d'afficher les coordonnées de la souris sur l'image
 * @param ev
 */
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
            posy += diff.y();
            processPos();

            //on repeint le widget avec les nouvelles coordonnées de l'image
            repaint();
        }
    }

    //on enregistre la nouvelle position de l'image
    RCPos = pos;

    //si la souris est sur l'image et le clic droit pas enfoncé
    //on envoie le signal pour afficher la position de la souris sur l'image
    //sinon on envoie le signal pour le cacher
    if(!RCDown && pos.x() > posx && pos.x() < posx + imgX * zoom && pos.y() > posy && pos.y() < posy + imgY * zoom) {
        emit mouseMoved();
    } else {
        emit mouseOut();
    }
}

/**
 * @brief leaveEvent Override de l'event handler de la sortie de la souris
 * permet de cacher les coordonnées de la souris sur l'image lorsqu'elle quitte le widget
 */
void MouseLabel::leaveEvent(QEvent *)
{
    emit mouseOut();
}

/**
 * @brief mouseReleaseEvent Override de l'event handler du relachement de clic de souris
 */
void MouseLabel::mouseReleaseEvent(QMouseEvent *ev) {
    if (ev->button() == Qt::RightButton) {
        this->setCursor(Qt::CrossCursor);
        RCDown = false;
    }
}

/**
 * @brief paintEvent Override de l'event handler du paintEvent
 * pour dessiner l'image à un certain endroit et avec un certain zoom
 * @param ev
 */
void MouseLabel::paintEvent(QPaintEvent *ev) {
    if(picOpened) {
        QPixmap newMap = QPixmap::fromImage(imageOrig);

        //if faut utiliser le painter dans le paintEvent
        //car toute autre modification va creer un nouveau paintEvent et creer une boucle infinie
        QPainter painter(this);
        //dessiner l'image
        painter.drawPixmap(posx, posy, imgX * zoom, imgY * zoom, newMap);
        painter.end();
    }
    //appel du paintEvent original pour dessiner le cadre du widget
    QLabel::paintEvent(ev);
}

/**
 * @brief wheelEvent Override de l'event handler de la roulette
 * permet de zoomer sur l'image.
 * @param ev
 */
void MouseLabel::wheelEvent(QWheelEvent *ev) {
    //ne fais rien s'il n'y a pas d'image ouverte
    if(picOpened) {
        //on enregistre l'endroit où le scroll a été effectué dans le cadre
        scrollx = ev->x();
        scrolly = ev->y();

        //zoomx et zoomy sont les coordonnées du pixel de la photo sur lequel
        //le scroll a ete effectue
        if (ev->delta() > 0) {
            zoom *= zoomRatio;
            zoomx = (ev->x() - posx) * zoomRatio;
            zoomy = (ev->y() - posy) * zoomRatio;
        } else
            if(zoom > 1) {
                zoom /= zoomRatio;
                zoomx = (ev->x() - posx) / zoomRatio;
                zoomy = (ev->y() - posy) / zoomRatio;
            } else {
                zoomx = (ev->x() - posx);
                zoomy = (ev->y() - posy);
            }

        //on met a jour la position de l'image
        posx = -zoomx + scrollx;
        posy = -zoomy + scrolly;
        processPos();

        //on repeint le widget
        repaint();
    }
}

/**
 * @brief processPos fonction qui vérifie que la position de l'image est valide
 * lorsque l'image est plus petite que le mouselabel, on l'empèche de sortir,
 * à l'inverse, lorsqu'elle est plus grande que le widget, on l'empèche de trop sortir, avec une marge de 100 pixels
 */
void MouseLabel::processPos() {
    if (imgX * zoom < width())
        posx = qBound(0, posx, (int) (width() - imgX * zoom) );
    else
        posx = qBound( (int) (-imgX * zoom + (width() - 100) ), posx, 100);

    if (imgY * zoom < height())
        posy = qBound(0, posy, (int) (height() - imgY * zoom) );
    else
        posy = qBound( (int) (-imgY * zoom + (height() - 100) ), posy, 100);
}
