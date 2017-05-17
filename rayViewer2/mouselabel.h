#ifndef MOUSELABEL_H
#define MOUSELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QWheelEvent>
#include <QImage>

class MouseLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MouseLabel(QWidget *parent = 0);

    void loadImg(QImage image);

    void mousePressEvent(QMouseEvent *ev);
    void leaveEvent(QEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *ev);
    void wheelEvent(QWheelEvent *ev);

    int imgX = 0, imgY = 0;
    //img = taille de l'image originale
    int mx = 0, my = 0;
    //m = coordonnées du clic de la souris
    int zoomx = 0, zoomy = 0;
    //zoom = pixel de l'image sur lequel il faut zoomer
    int posx = 0, posy = 0;
    //pos = position du coin en haut a gauche de l'image
    int scrollx = 0, scrolly = 0;
    //scroll = coordonnées de la souris dans le cadre lors du scroll

    float zoom = 1.0;
    bool picOpened = false;
    QImage imageOrig;

    bool RCDown = false;
    //click droit enfonce ou non
    QPoint RCPos;
    //position de la souris lors du clic droit

signals:
    void mousePressed();
    void mouseMoved();
    void mouseOut();

public slots:
};

#endif // MOUSELABEL_H
