#ifndef MOUSELABEL_H
#define MOUSELABEL_H

#include <QLabel>
#include <QMouseEvent>

class MouseLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MouseLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *ev);

    int mx, my;

signals:
    void mousePressed();

public slots:
};

#endif // MOUSELABEL_H
