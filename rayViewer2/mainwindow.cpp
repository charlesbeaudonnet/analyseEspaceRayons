#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mouselabel.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->imgLabel, SIGNAL(mousePressed()), this, SLOT(mousePressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if(QString::compare(fileName, QString()) != 0) {
        QImage image;
        bool valid = image.load(fileName);
        if(valid) {
            imgX = image.width();
            imgY = image.height();

            image = image.scaledToWidth(800, Qt::FastTransformation);
            ui->imgLabel->setPixmap(QPixmap::fromImage(image));

            ui->loadedImg->setText(QString("%1 ; %2x%3").arg(fileName).arg(imgX).arg(imgY));

            //ui->logLabel->setText(fileName.left(fileName.length() - 4).append(".txt"));
            //QFile file(fileName.left(fileName.length() - 4).append(".txt"));
            logs = new QFile(fileName.left(fileName.length() - 4).append(".txt"));
            if(!logs->open(QIODevice::ReadOnly | QIODevice::Text)) {
                fileOpened = false;
                ui->logLabel->setText("Aucun fichier txt du même nom trouvé.");
            } else {
                fileOpened = true;
                ui->logLabel->setText("Fichier de logs trouvé, cliquez sur un pixel pour voir ses informations.");
            }
        } else {
            QMessageBox msgBox;
            msgBox.setText("Erreur lors de l'ouverture de l'image");
            msgBox.exec();
        }
    }
}

void MainWindow::mousePressed()
{
    int x, y;
    QString output = "";
    x = ui->imgLabel->mx * ((double) imgX / 800.0);
    y = ui->imgLabel->my * ((double) imgX / 800.0) + (imgY - imgX) / 2;
    if(y < 0) {
        y = 0;
    } else if(y >= imgY) {
        y = imgY - 1;
    }

    ui->pixLabel->setText(QString("Pixel %1 %2").arg(x).arg(y));

    if(fileOpened) {
        while(!output.contains(QString("[%1,%2]").arg(x).arg(y))){
            if(!logs->atEnd())
                output = logs->readLine();
            else
                logs->reset();
        }
        output.replace("]", "]\n");
        output.replace(";", ";\n");
        ui->logLabel->setText(output);
    }
}
