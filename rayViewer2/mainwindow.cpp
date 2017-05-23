#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mouselabel.h"

#include <QMessageBox>
#include <QDebug>
#include <QStandardItemModel>
#include <assert.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->imgLabel, SIGNAL(mousePressed()), this, SLOT(mousePressed()));
    connect(ui->imgLabel, SIGNAL(mouseMoved()), this, SLOT(mouseMoved()));
    connect(ui->imgLabel, SIGNAL(mouseOut()), this, SLOT(mouseOut()));
    ui->ObjectTable->hide();
}

MainWindow::~MainWindow(){
    delete ui;
}

/**
 * @brief on_butImg_clicked EventHandler du clic sur le bouton "import image"
 * permet d'ouvrir un dialogue de choix d'image, ouvrir l'image et chercher des logs
 */
void MainWindow::on_butImg_clicked(){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if(QString::compare(fileName, QString()) != 0) {
        QImage image;

        if(image.load(fileName)) {
            //Chargement de l'image dans le mouselabel
            ui->imgLabel->loadImg(image);
            //affichage du nom et des dimensions de l'image
            ui->loadedImg->setText(QString("%1 ; %2x%3").arg(fileName).arg(ui->imgLabel->imgX).arg(ui->imgLabel->imgY));

            //On vide les widget de l'applications qui pourraient contenir des données de l'image précédente
            ui->pathPicker->clear();
            clearDataTable();
            clearObjectTable();
            clearColorInfos();
            ui->ObjetCheckBox->setChecked(false);

            //On cherche un fichier de logs du même nom que l'image et on gère les cas d'erreur
            logs = new QFile(fileName.left(fileName.length() - 4).append(".txt"));
            if((fileOpened = logs->open(QIODevice::ReadOnly | QIODevice::Text))){
                parser = new ParserAPI(logs);

                if(!parser->isCorrectlySet()){
                    fileOpened = false;
                    ui->logLabel->setText("Le format du fichier log est invalide");
                }else{
                    ui->logLabel->setText("Fichier de logs trouvé, cliquez sur un pixel pour voir ses informations.");
                }
            }else{
                ui->logLabel->setText("Aucun fichier txt du même nom trouvé.");
            }

        } else {
            //Si on arrive pas a charger l'image on affiche un message d'erreur
            QMessageBox msgBox;
            msgBox.setText("Erreur lors de l'ouverture de l'image");
            msgBox.exec();
        }
    }
}

/**
 * @brief mousePressed EventHandler du signal mousePressed du mouselabel
 * permet de mettre a jour les données de rayons du pixel sélectionné
 */
void MainWindow::mousePressed(){
    //le mouselabel n'envoie cet évènement que si les coordonnées sont valides
    //donc on peut les récupérer sans les tester
    int x = ui->imgLabel->mx;
    int y = ui->imgLabel->my;

    ui->pixLabel->setText(QString("Pixel %1 %2").arg(x).arg(y));

    if(fileOpened) {
        //On recupère la ligne dans le fichier.
        rawData = parser->getRaw(x,y);
        ui->logLabel->setText("");
        ui->pathPicker->clear();
        for(int i = 0; i < rawData.size(); i++) {
            //On l'affiche dans logLabel
            ui->logLabel->append(parser->parseRaw(rawData.at(i)));
            //On remplis le pathPicker avec les chemins
            ui->pathPicker->addItem(QString("chemin %1").arg(i + 1));
        }
        if(ui->ObjetCheckBox->isChecked())
            ui->pathPicker->addItem("Tous");
    }
}

/**
 * @brief mouseMoved EventHandler du signal mouseMoved du mouselabel
 * permet de déplacer le label qui indique la position de la souris sur l'image
 */
void MainWindow::mouseMoved(){
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

/**
 * @brief mouseOut EventHandler du signal mouseOut du mouselabel
 * permet de cacher le label qui indique la position de la souris sur l'image
 */
void MainWindow::mouseOut(){
    ui->labelMousePos->hide();
}

/**
 * @brief extractData Extraire la prochaine donnée à partir de la position pos dans str ayant pour marqueur info
 * @example str = p[200,200,200]d(-0.594070494,-0.468256056,-0.654076755)p[7.04707,47.9115,-12.443]d(-0.489871621,0.240169644,-0.838060021)N(0.140941396,0.958230078,-0.248859003)O1skyC[0.0559621,0.0493958,0.0357635]
 *          pos = 12
 *          info = p
 *          Output = [7.04707,47.9115,-12.443]
 *
 * @param str Une QString
 * @param pos La position du début du traitement de str
 * @param info Le marqueur de la donnée à extraire
 * @return  Une QString contenant la première donnée de marqeur info depuis la position pos dans str
 */
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

/**
 * @brief clearDataTable vide la table qui affiche les données d'un chemin
 */
void MainWindow::clearDataTable(){
    QAbstractItemModel * model = ui->DataTable->model();
    if(model == NULL)
        return;
    model->removeRows(0,model->rowCount());
    model->removeColumns(0,model->columnCount());
    model->deleteLater();
}

/**
 * @brief initDataTable affiche les données d'un chemin dans la dataTable
 * @param path chemin à afficher
 * @param optionNum nombre d'options pour chaque intersections
 */
void MainWindow::initDataTable(QString path, int optionNum){
    clearDataTable();
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
    //model->horizontalHeaderItem(1)->setBackground(QBrush(QColor(150,150,150)));
    //On vide le model de toutes ses lignes
    for(int i = 1; i < model->rowCount(); model->removeRow(i)){}

    int currentPos;
    int nbOption = parser->option.size();
    //On elimine le pixel et on s'arret au premier p trouvé
    for(currentPos = 0; currentPos < line.size() - 1 && line[currentPos] != ']' && line[currentPos] != 'p'; currentPos++){}

    if(line[currentPos + 1] == 'p')
        currentPos++;

    //On s'occupe de la caméra en premier car c'est un cas particulier (2 arg seulement)
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
}

/**
 * @brief clearColorInfos réinitialise les widget donnant des informations sur la couleur
 * et sur le pixel sélectionné
 */
void MainWindow::clearColorInfos(){
    ui->pixLabel->setText("");
    ui->ColorLabel->setText("");
    QPalette palette = ui->COLORBG->palette();
    palette.setColor(ui->COLORBG->backgroundRole(), QColor(0,0,0,0));
    ui->COLORBG->setPalette(palette);
    ui->COLORBG->repaint();
}

/**
 * @brief initColorInfos affiche les informations sur la couleur d'un chemin
 * @param line ligne contenant les infos sur la couleur
 */
void MainWindow::initColorInfos(QString line){
    ui->ColorLabel->setText(parser->parseColor(line));

    //Exraction de la couleur + coloration du carré
    QPalette palette = ui->COLORBG->palette();
    QColor qcouleur = parser->parseQColor(line);
    palette.setColor(ui->COLORBG->backgroundRole(), qcouleur);
    ui->COLORBG->setPalette(palette);
    ui->COLORBG->repaint();
}

/**
 * @brief clearObjectTable vide la table qui affiche des infos sur les objets
 */
void MainWindow::clearObjectTable(){
    QAbstractItemModel * model = ui->ObjectTable->model();
    if(model == NULL)
        return;
    model->removeRows(0,model->rowCount());
    model->removeColumns(0,model->columnCount());
    model->deleteLater();
}

/**
 * @brief initObjectTable remplit la table qui affiche des infos sur les objets
 * @param line chemin contenant les objets à afficher
 */
void MainWindow::initObjectTable(QString line){
    clearObjectTable();
    QStandardItemModel * model = new QStandardItemModel(0,3,ui->DataTable);

    //Initialisation des titres
    QStringList headerData;
    headerData.push_back("Nom");
    headerData.push_back("Matière");
    headerData.push_back("Caractéristiques");
    model->setHorizontalHeaderLabels(headerData);
    headerData.clear();

    bool ok;
    int pos = 0;
    QSet<int> setObj;
    QString data;
    do{
        if((data = extractData(line,&pos,'O')).size() != 0)
            setObj.insert(data.toInt(&ok));
    }while(ok && line.size() > pos);

    //On tri les objets pour les afficher dans l'ordre croissant de leur indice
    QList<int> listObj = setObj.toList();
    qSort(listObj);

    QString objetInfo;
    QString info;
    QList<QStandardItem * > objectList;
    int j;
    for(int i = 0; i < listObj.size(); i++){
        info = "";

        objectList.push_back(new QStandardItem(QString("Objet #%1").arg(listObj[i])));
        objetInfo = parser->getObject(listObj[i] - 1);

        //On recupere la matière
        for( j = 0; j < objetInfo.size() - 1 && objetInfo[j] != '('; j++){
            info += objetInfo[j];
        }
        objectList.push_back(new QStandardItem(info));

        //On recupère ses caracteristiques
        int nbK = 0;
        for( info = "", j++; j < objetInfo.size() - 1 && objetInfo[j] != ')'; j++){
            if(objetInfo[j] == 'K'){
                nbK++;
                if(nbK > 1){
                    info += " ";
                }
            }
            info += objetInfo[j];
        }
        objectList.push_back(new QStandardItem(info));

        model->appendRow(objectList);
        objectList.clear();
    }
    ui->ObjectTable->setModel(model);

}

/**
 * @brief initObjectTable remplit la table qui affiche des infos sur les objets
 * @param dataVec liste des chemins contenant les objets à afficher
 */
void MainWindow::initObjectTable(QVector<QString> dataVec){
    clearObjectTable();
    QStandardItemModel * model = new QStandardItemModel(0,3,ui->DataTable);

    //Initialisation des titres
    QStringList headerData;
    headerData.push_back("Nom");
    headerData.push_back("Matière");
    headerData.push_back("Caractéristiques");
    model->setHorizontalHeaderLabels(headerData);
    headerData.clear();

    bool ok;
    int pos = 0;
    QSet<int> setObj;
    QString line;
    QString data;
    for(int i = 0; i < dataVec.size(); i++){
        line = dataVec[i];
        do{
            if((data = extractData(line,&pos,'O')).size() != 0)
                setObj.insert(data.toInt(&ok));
        }while(ok && line.size() > pos);
    }
    //On tri les objets pour les afficher dans l'ordre croissant de leur indice
    QList<int> listObj = setObj.toList();
    qSort(listObj);

    QString objetInfo;
    QString info;
    QList<QStandardItem * > objectList;
    int j;
    for(int i = 0; i < listObj.size(); i++){
        info = "";
        objectList.push_back(new QStandardItem(QString("Objet #%1").arg(listObj[i])));
        objetInfo = parser->getObject(listObj[i] - 1);

        //On recupere la matière
        for( j = 0; j < objetInfo.size() - 1 && objetInfo[j] != '('; j++){
            info += objetInfo[j];
        }
        objectList.push_back(new QStandardItem(info));

        //On recupère ses caracteristiques
        int nbK = 0;
        for( info = "", j++; j < objetInfo.size() - 1 && objetInfo[j] != ')'; j++){
            if(objetInfo[j] == 'K'){
                nbK++;
                if(nbK > 1){
                    info += " ";
                }
            }
            info += objetInfo[j];
        }
        objectList.push_back(new QStandardItem(info));

        model->appendRow(objectList);
        objectList.clear();
    }
    ui->ObjectTable->setModel(model);
}

/**
 * @brief on_precisionSpin_valueChanged EventHandler du spinner de précision
 * permet de changer la précision des chiffres dans les données
 * @param arg1 nombre de chiffres après la virgule
 */
void MainWindow::on_precisionSpin_valueChanged(int arg1){
    parser->precision = arg1;
    int index = ui->pathPicker->currentIndex();
    ui->logLabel->setText("");
    ui->pathPicker->clear();
    for(int i = 0; i < rawData.size(); i++) {
        ui->logLabel->append(parser->parseRaw(rawData.at(i)));
        ui->pathPicker->addItem(QString("chemin %1").arg(i + 1));
    }
    if(ui->ObjetCheckBox->isChecked())
        ui->pathPicker->addItem(QString("Tous"));

    ui->pathPicker->setCurrentIndex(index);
}

/**
 * @brief on_rawLevelSpin_valueChanged
 * @param arg1
 */
void MainWindow::on_rawLevelSpin_valueChanged(int arg1){
    parser->rawLevel = arg1;
    ui->logLabel->setText("");
    for(int i = 0; i < rawData.size(); i++)
        ui->logLabel->append(parser->parseRaw(rawData[i]));
}

/**
 * @brief on_pathPicker_currentIndexChanged EventHandler de l'évènement du changement du choix de chemin
 * @param index chemin choisi
 */
void MainWindow::on_pathPicker_currentIndexChanged(int index) {
    selectedPath = index;
    if(index >= 0 && index < rawData.size()){
        //on remplit les widget de données en conséquence
        MainWindow::initDataTable(rawData.at(selectedPath),parser->option.size());
        MainWindow::initObjectTable(rawData.at(selectedPath));
        MainWindow::initColorInfos(rawData.at(selectedPath));
    }else{
        if(index == rawData.size()){
            MainWindow::initObjectTable(rawData);
        }
    }
}

/**
 * @brief MainWindow::on_ObjetCheckBox_stateChanged EventHandler du changement d'état du checkbox du mode objet
 * permet d'afficher DataTable ou ObjectTable selon le mode voulu
 * @param arg1 si le checkbox est coché ou non
 */
void MainWindow::on_ObjetCheckBox_stateChanged(int arg1){

    if( arg1 == 0){
        ui->DataTable->show();
        ui->ObjectTable->hide();
        if(ui->pathPicker->count() > rawData.size())
            ui->pathPicker->removeItem(ui->pathPicker->count() - 1);
    }else{
        if(ui->pathPicker->count() != 0)
            ui->pathPicker->addItem("Tous");
        ui->DataTable->hide();
        ui->ObjectTable->show();
    }
}
