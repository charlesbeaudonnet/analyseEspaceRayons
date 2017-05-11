#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QFile* logs;
    bool fileOpened = false;
    int imgX = 1, imgY = 1;

private slots:
    void on_butImg_clicked();
    void mousePressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
