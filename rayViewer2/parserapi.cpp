#include "parserapi.h"

#include <QDebug>
#include <QColor>
#include <QDateTime>
#include <QtGlobal>

#include <iostream>
parserAPI::parserAPI(QFile *file){
    QString line;
    char * endLine;
    m_matrix = NULL;
    if(!file->isOpen()){
        if (!(file->open(QIODevice::ReadOnly))){
            return;
        }
    }
    ts = new QTextStream(file);

    //La première ligne contient les options de logs
    QString OptionTester = "Ppno";
    line = ts->readLine();
    for(int i = 0; i < OptionTester.size(); i++){
        if(line.contains(OptionTester[i])){
            switch (i+1) {
            case 1:
                option += "pd";
                break;
            case 2:
                option += "p";
                break;
            case 3:
                option += "N";
                break;
            case 4:
                option += "O";
                break;
            default:
                qInfo() << " Options parsing gone wrong";
                return;
                break;
            }
        }
    }
    line = ts->readLine();
    if(!line[1].isLetter())
        return;

    //La troisième ligne contient la taille de la matrice
    line = ts->readLine();
    int i = 1;
    int row = 0, col = 0;
    char * lineChar = line.toLocal8Bit().data();
    //Recupere la largeur
    for(    ; i < line.size() && line[i] != ','; i++){
        if(lineChar[i] > '9' || lineChar[i] < '0'){
            qInfo() << "Matrix size non valid";
            return;
        }
        row = 10 * row + lineChar[i] - '0';
    }
    //Recupere la hauteur
    for( ++i ; i < line.size() && line[i] != ']'; i++){
        if(lineChar[i] > '9' || lineChar[i] < '0'){
            qInfo() << "Matrix size non valid";
            return;
        }
        col = 10 * col + lineChar[i] - '0';
    }

    //Creation de la matrice
    m_matrix = new Matrix(row,col);

    //qint64 position = ts->pos();
    ts->seek(file->size()-20);
    line = ts->readLine();
    qint64 tpTo = 0;
    QStringList strList = line.split(" ");
    tpTo = strList[strList.size() - 2].toInt(NULL,10);
    ts->seek(tpTo);

    //On parcours le fichier jusqu'au marqueur "MATRIX"
    do{
        line = ts->readLine();
    }while(line[0] != 'M' && !ts->atEnd());

    if( line[0] == 'M'){
        //On charge la matrice
        for(int i = 0; i < row; i++){
            line = ts->readLine();
            m_matrix->set(i,0,strtol(line.toLocal8Bit().data(),&endLine,10));
            for(int j = 1; j < col; j++){
                m_matrix->set(i,j,strtol(endLine,&endLine,10));
            }
        }
    }

}

bool parserAPI::isCorrectlySet(){
    bool isSet = (m_matrix != NULL);
    if(isSet){
        qsrand(QDateTime::currentMSecsSinceEpoch());
        int testedSize = m_matrix->col * m_matrix->row / 10;
        for(int i = 0; i < testedSize; i++){
            isSet &= (m_matrix->get(qrand() % m_matrix->row,qrand() % m_matrix->col) != 0);
        }
    }
    return isSet;
}

parserAPI::~parserAPI() {
    delete m_matrix;
}

QString parserAPI::cutNumber(QString path){
    QString result = "";
    QString tail = "";
    int offset;
    bool fromBreak;
    //Parcours de la chaine
    for(int i = 0; i < path.size(); i++){
        result += path[i];

        //Si on croise un point on recupère au plus 4 char
        if(path[i] == '.'){
            i++;//Pour pas doubler le .
            for(offset = 0; offset < precision; offset++){
                tail += path[i + offset];
                //Si on atteint le fin d'un vecteur ou point -> ) ou ] ou alors une autre dimension -> ,
                if(path[i + offset] == ',' || path[i + offset] == ']' || path[i + offset] == ')'){
                    fromBreak = true;
                    break;
                }
            }
            //On stock tout dans result et on met a jour le pointeur de char ( i )
            result += tail;
            i += offset;
            tail = "";// reset de tail

            //Si on est pas sortit par le break alors on a potentiellement plus de decimales que voulut
            //donc on les "elimines"
            if(!fromBreak){
                while(path[i] != ',' && path[i] != ']' && path[i] != ')'){
                    i++;
                }
                //On rajoute le token de fin "," ou "]" ou ")"
                result += path[i];
            }
            fromBreak = false;//reset de fromBreak;

        }//path[i] == '.'
    }//whole string

    return result;
}

QString parserAPI::getPath(int x, int y){
    ts->seek(m_matrix->get(x,y));
    QString line = ts->readLine();
    QString pathLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'p'){
            do{
                pathLine += line[++i];
            }while(line[i] != ']');
            pathLine += ';';
        }
    }
    return cutNumber(pathLine);
}

QVector<QString> parserAPI::getRaw(int x, int y){
    ts->seek(m_matrix->get(x,y));
    QVector<QString> lignes;

    QString ligne = ts->readLine();
    while( !ligne.startsWith("}") ){
        lignes.append(ligne);
        ligne = ts->readLine();
    };
    return lignes;
}

unsigned long long int parserAPI::getPos(int x, int y){
    return  m_matrix->get(x,y);
}

void parserAPI::setStreamAt(int x, int y){
    ts->seek(m_matrix->get(x,y));
}

QString parserAPI::getDir(int x, int y){
    ts->seek(m_matrix->get(x,y));
    QString line = ts->readLine();
    QString dirLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'd'){
            do{
                dirLine += line[++i];
            }while(line[i] != ')');
            dirLine += ";";
        }
    }
    return cutNumber(dirLine);
}

QString parserAPI::getNorm(int x, int y){

    ts->seek(m_matrix->get(x,y));
    QString line = ts->readLine();
    QString normLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'n'){
            do{
                normLine += line[++i];
            }while(line[i] != ']');
            normLine +=";";
        }
    }
    return cutNumber(normLine);
}

QString parserAPI::parseDir(QString line){
    QString dirLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'd'){
            do{
                dirLine += line[++i];
            }while(line[i] != ')');
            dirLine += ";";
        }
    }
    return cutNumber(dirLine);
}

QString parserAPI::parseNorm(QString line){
    QString normLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'N'){
            do{
                normLine += line[++i];
            }while(line[i] != ')');
            normLine += ";";
        }
    }
    return cutNumber(normLine);
}

QString parserAPI::parsePath(QString line){
    QString pathLine = "";
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'p'){
            do{
                pathLine += line[++i];
            }while(line[i] != ']');
            pathLine += ';';
        }
    }
    return cutNumber(pathLine);
}

int clamp(int toClamp, int inf, int sup){
    return toClamp < inf ? inf : toClamp > sup ? sup : toClamp;
}

QString getHexCode(int dec){
    char hex[20];
    itoa(dec,hex,16);
    return QString(hex);
}

QString parserAPI::extractData(QString str, int *pos, const char info){
    QString data = "";
    char delim = ')';
    char delim2 = '$';
    int checker = 0;
    if(info == 'p' || info == 'C'){
        delim = ']';
    }
    if(info == 'O'){
        checker = 1;
        delim = 's';
        delim2 = option[0].toLatin1();
    }
    for(    ; *pos < str.size(); (*pos)++ ){
        if(str[*pos] == info){
            while((*pos) < str.size() && str[(*pos) + checker] != delim && str[(*pos) + checker] != delim2){
                data += str[++(*pos)];
            }
            return data;
        }
    }
    return data;
}

QString parserAPI::parseRaw(QString line){
    //Mode hardcore activé on retourne la donnée brute
    if(rawLevel == 0)
        return line;

    QString result = "";
    int optNumber = option.size();
    int currentOption = 0;
    int i = 0;
    //On elimine le pixel et on s'arret au premier p trouvé
    for(i = 0; i < line.size() - 1 && line[i] != ']' && line[i] != 'p'; i++){}

    if(line[i + 1] == 'p')
        i++;

    //Le premier point celui de la caméra ne change pas
    for(    ; i < line.size() && line[i] != ')'; i++){
        result += line[i];
    }
    result += line[i] + "\n";

    //On parcourt le reste de la string
    for( i++ ; i < line.size(); i++){

        //On compte le nombre de valeure qu'on à croisé
        if(line[i] == option[currentOption]){
            currentOption++;
        }

        //Si on a tous croisé et qu'on est en fin de ligne on passe au token suivant
        if(currentOption == optNumber && (line[i] == 's' || line[i] == option[0])){
            result += "\n";
            currentOption = line[i] == option[0] ? 1 : 0;
        }

        //Si on croise un C on est à la couleur donc la dernière ligne
        if(line[i] == 'C'){
            //On met à zéro currentOption sinon le crochet de fin est écrit à la ligne à cause du if précédent
            currentOption = 0;
            result += "\n";
        }
        result += line[i];
    }
    //Mode clean activé on retourne la donnée un peu plus claire
    if(rawLevel == 1)
        return result;

    //Mode simplifié activé on retourne la donnée la plus travaillée possible
    return cutNumber(result);
}

QString parserAPI::parseColor(QString line){
    QString colorLine = "";
    QString COLOR = "RGB";
    QString hexValue = "Hex: #";
    int RGBValue[3];
    int color = 0;
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'C'){
            i += 2;
            do{
                if( line[i] == ','){
                    RGBValue[color++] = static_cast<int>(colorLine.toDouble() * 255);
                    colorLine = "";
                    ++i;
                }else{
                    colorLine += line[i++];
                }
            }while(line[i + 1] != ']');
            RGBValue[color++] = static_cast<int>(colorLine.toDouble() * 255);
        }
    }
    colorLine = "";
    for(int i = 0; i < 3; i++){
        colorLine += COLOR[i] + QString(": ") + QString::number(RGBValue[i]) + " ";
        hexValue += getHexCode(clamp(RGBValue[i],0,255));
    }
    return colorLine + "   " + hexValue;
}

QColor parserAPI::parseQColor(QString line){

    QString colorLine = "";
    int RGBValue[3];
    int color = 0;
    for(int i = 0; i < line.size(); i++){
        if(line[i] == 'C'){
            i += 2;
            do{
                if( line[i] == ','){
                    RGBValue[color++] = static_cast<int>(colorLine.toDouble() * 255);
                    colorLine = "";
                    ++i;
                }else{
                    colorLine += line[i++];
                }
            }while(line[i + 1] != ']');
            RGBValue[color++] = static_cast<int>(colorLine.toDouble() * 255);
        }
    }
    for(int i = 0; i < 3; i++){
        RGBValue[i] = qBound(0, RGBValue[i], 255);
    }
    return QColor(RGBValue[0], RGBValue[1], RGBValue[2]);
}

QColor parserAPI::getTrueColor(QVector<QString> rawData){
    double RGBValue[3];
    QString line = "";
    QString colorLine = "";
    int color;
    double nbCol = static_cast<double>(rawData.size());

    for(int  k = 0; k < nbCol; k++){

        line = rawData[k];
        color = 0;
        qInfo() << "Traitement chemin numéro " << k+1;
        for(int i = 0; i < line.size(); i++){
            if(line[i] == 'C'){
                i += 2;
                do{
                    if( line[i] == ','){
                        qInfo() << "Traitement couleur numéro : " << color << " = " << colorLine.toDouble();
                        RGBValue[color++] += colorLine.toDouble();
                        colorLine = "";
                        ++i;
                    }else{
                        colorLine += line[i++];
                    }
                }while(line[i + 1] != ']');
                qInfo() << "Traitement du bleu" << " = " << colorLine.toDouble();
                RGBValue[color++] += colorLine.toDouble();
            }
        }
    }
    return QColor(static_cast<int>((RGBValue[0] / nbCol) * 255) , static_cast<int>((RGBValue[1] / nbCol) * 255), static_cast<int>((RGBValue[2] / nbCol) * 255));
}
