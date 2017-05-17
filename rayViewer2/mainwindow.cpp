#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mouselabel.h"

#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->imgLabel, SIGNAL(mousePressed()), this, SLOT(mousePressed()));
    connect(ui->imgLabel, SIGNAL(mouseMoved()), this, SLOT(mouseMoved()));
    connect(ui->imgLabel, SIGNAL(mouseOut()), this, SLOT(mouseOut()));
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
        if(image.load(fileName)) {
            ui->imgLabel->loadImg(image);

            ui->loadedImg->setText(QString("%1 ; %2x%3").arg(fileName).arg(ui->imgLabel->imgX).arg(ui->imgLabel->imgY));

            logs = new QFile(fileName.left(fileName.length() - 4).append(".txt"));
            if((fileOpened = logs->open(QIODevice::ReadOnly | QIODevice::Text))) {
                ui->logLabel->setText("Fichier de logs trouvé, cliquez sur un pixel pour voir ses informations.");
                parser = new parserAPI(logs);
                //Init de la ligne des titres
                if(!parser->isCorrectlySet()){
                    fileOpened = false;
                    ui->logLabel->setText("Le format du fichier log est invalide");
                }
            }else{
                ui->logLabel->setText("Aucun fichier txt du même nom trouvé.");
            }

        } else {
            QMessageBox msgBox;
            msgBox.setText("Erreur lors de l'ouverture de l'image");
            msgBox.exec();
        }
    }
}

void MainWindow::mousePressed(){
    int x, y;
    QString output = "";
    x = ui->imgLabel->mx;
    y = ui->imgLabel->my;

    ui->pixLabel->setText(QString("Pixel %1 %2").arg(x).arg(y));

    if(fileOpened) {
        //On recupère la ligne dans le fichier.
        rawData = parser->getRaw(x,y);
        ui->logLabel->setText("");
        ui->pathPicker->clear();
        for(int i = 0; i < rawData.size(); i++) {
            ui->logLabel->append(parser->parseRaw(rawData.at(i)));
            ui->pathPicker->addItem(QString("chemin %1").arg(i + 1));
        }
    }
}

void MainWindow::mouseMoved()
{
    QPoint mpos = ui->imgLabel->RCPos;
    QString contenu = QString("%1, %2")
            .arg((int) ((mpos.x() - ui->imgLabel->posx) / ui->imgLabel->zoom))
            .arg((int) ((mpos.y() - ui->imgLabel->posy) / ui->imgLabel->zoom));

    ui->labelMousePos->setFixedWidth((int) (contenu.size() * 7) - 2);
    ui->labelMousePos->setText(contenu);
    ui->labelMousePos->move(mpos.x() + 26, mpos.y() + 24);
    ui->labelMousePos->show();
    ui->imgLabel->repaint();
}

void MainWindow::mouseOut()
{
    ui->labelMousePos->hide();
}

QString MainWindow::extractData(QString str, int *pos, const char info){
    QString data = "";
    char delim = ')';
    char delim2 = '$';
    char delim3 = '$';
    int checker = 0;
    if(info == 'p' || info == 'C'){
        delim = ']';
    }
    if(info == 'O'){
        checker = 1;
        delim = 's';
        delim2 = parser->option[0].toLatin1();
        delim3 = 'C';
    }
    for(    ; *pos < str.size(); (*pos)++ ){
        if(str[*pos] == info){
            while((*pos) < str.size()
                  && str[(*pos) + checker] != delim
                  && str[(*pos) + checker] != delim2
                  && str[(*pos) + checker] != delim3){
                data += str[++(*pos)];
            }
            return data;
        }
    }
    return data;
}

void MainWindow::initDataTable(QString path, int optionNum){
    QStandardItemModel * model = new QStandardItemModel(0,optionNum + 1,ui->DataTable);
    QString line = parser->cutNumber(path);
    //Init des titres
    QStringList headerData;
    headerData.push_back("Nom");
    QString arg;
    for(int i = 1; i < model->columnCount(); i++) {
        switch(parser->option[i-1].toLatin1()) {
        case 'p':
            arg = "Point";
            break;
        case 'd':
            arg = "Direction";
            break;
        case 'N':
            arg = "Normale";
            break;
        case 'O':
            arg = "Objet";
            break;
        default:
            arg = "Not Found";
            break;
        }
        headerData.push_back(arg);
    }
    //On set les titres
    model->setHorizontalHeaderLabels(headerData);
    headerData.clear();
    model->horizontalHeaderItem(1)->setBackground(QBrush(QColor(150,150,150)));
    //On vide le model de toutes ses lignes
    for(int i = 1; i < model->rowCount(); model->removeRow(i)){}

    int currentPos;
    int nbOption = parser->option.size();
    //On elimine le pixel et on s'arret au premier p trouvé
    for(currentPos = 0; currentPos < line.size() - 1 && line[currentPos] != ']' && line[currentPos] != 'p'; currentPos++){}

    if(line[currentPos + 1] == 'p')
        currentPos++;

    //On s'occupe de la caméra en premier car c'ets un cas particulier (2 arg seulement)
    QList<QStandardItem *> intersectionData;
    intersectionData.push_back(new QStandardItem("Camera"));
    intersectionData.push_back(new QStandardItem(extractData(line,&currentPos,'p')));
    intersectionData.push_back(new QStandardItem(extractData(line,&currentPos,'d')));
    for(int i = 3; i < nbOption + 1 ; i++){
        intersectionData.push_back(new QStandardItem("---"));
    }
    model->appendRow(intersectionData);
    intersectionData.clear();

    //On remplit la matrice
    int currentData = 0;
    int interNum = 1;
    do{
        //Si currentData == nbOption alors on a trouver toutes les options de l'intersection
        //On peut maintenant la nommer
        if(currentData == 0){
            intersectionData.push_back(new QStandardItem(QString("Intersection #%1").arg(interNum++)));
        }else{
            //On extrait l'option (n o p d etc) de la string
            QString extractedData = extractData(line,&currentPos,parser->option[currentData - 1].toLatin1());

            //Si c'est vide alors on a atteint la dernière option du dernier chemin
            if(extractedData == "")
                break;
            intersectionData.push_back(new QStandardItem(extractedData));
        }
        currentData = (currentData + 1)  % (nbOption + 1);
        if(currentData == 0){
            model->appendRow(intersectionData);
            intersectionData.clear();
        }
    }while(currentPos < line.size());
    ui->DataTable->setModel(model);
    ui->DataTable->setVisible(true);
    ui->ColorLabel->setText(parser->parseColor(line));

    //Exraction de la couleur + coloration du carré
    QPalette palette = ui->COLORBG->palette();
    QColor qcouleur = parser->parseQColor(line);
    palette.setColor(ui->COLORBG->backgroundRole(), qcouleur);
    ui->COLORBG->setPalette(palette);
    ui->COLORBG->repaint();
}

void MainWindow::on_precisionSpin_valueChanged(int arg1){
    parser->precision = arg1;
    MainWindow::mousePressed();
}


void MainWindow::on_rawLevelSpin_valueChanged(int arg1){
    parser->rawLevel = arg1;
    ui->logLabel->setText("");
    for(int i = 0; i < rawData.size(); i++)
        ui->logLabel->setText(parser->parseRaw(rawData[i]));
}

void MainWindow::on_pathPicker_currentIndexChanged(int index) {
    selectedPath = index;
    if(index >= 0)
        MainWindow::initDataTable(rawData.at(selectedPath),parser->option.size());
}
