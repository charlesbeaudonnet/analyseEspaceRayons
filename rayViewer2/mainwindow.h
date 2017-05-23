#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include "parserapi.h"

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
    ParserAPI  * parser = NULL;
    int selectedPath = 0;
    QVector<QString> rawData;

private slots:
    void on_butImg_clicked();
    void mousePressed();
    void mouseMoved();
    void mouseOut();

    void on_precisionSpin_valueChanged(int arg1);
    void on_rawLevelSpin_valueChanged(int arg1);
    void on_pathPicker_currentIndexChanged(int index);
    void on_ObjetCheckBox_stateChanged(int arg1);

private:
    void clearDataTable();
    void clearObjectTable();
    void clearColorInfos();
    QString extractData(QString str, int *pos, const char info);
    void initDataTable(QString line, int optionNum);
    void initObjectTable(QString line);
    void initObjectTable(QVector<QString> dataVec);
    void initColorInfos(QString line);
    void initDisplay(QString line);
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
