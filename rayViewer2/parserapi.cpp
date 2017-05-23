#include "parserapi.h"

#include <QDebug>
#include <QColor>
#include <QDateTime>
#include <QtGlobal>
#include <iostream>

/**
 * @brief ParserAPI Initialise le parser avec le fichier passé en paramètre
 * @param file
 */
ParserAPI::ParserAPI(QFile *file){
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
    QString OptionTester = "Ppnom";
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
            case 5:
                parseObjectList(line.length() + 1);
                break;
            default:
                qInfo() << " Options parsing gone wrong";
                return;
                break;
            }
        }
    }

    //On se positionnne sur la ligne qui contient la taille de la matrice
    do{
        line= ts->readLine();
    }while(line[1].isLetter());

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

    /*
     * On va à la fin du fichier car la dernière ligne contient
     * la position du dernier pixel dans le fichier car la matrix est écrite apres le dernier pixel.
     * De cette façon on évite de faire (lignes * colonnes * (nombre de rayon - 1))  ts->readLine()
     * et on améliore donc énormément la fluidité du logiciel
     */
    ts->seek(file->size()-30);
    line = ts->readLine();
    qint64 tpTo = 0;
    QStringList strList = line.split(" ");
    //On prend l'avant dernière valeur car le fichier peut contenir un espace à la fin
    for( i = 0; strList[strList.size() - i - 1].size() == 0; i++){}
    tpTo = strList[strList.size() - i - 1].toInt(NULL,10);
    ts->seek(tpTo);

    //On parcours le fichier jusqu'au marqueur "MATRIX"
    do{
        line = ts->readLine();
    }while(line[0] != 'M' && !ts->atEnd());
    //Cette ligne sert à ne pas faire planter le parser au cas où le marqueur n'existe pas
    if( line[0] == 'M'){
        //On charge la matrice
        for(int i = 0; i < row; i++){
            line = ts->readLine();
            if(line.size() == 0)
                break;
            m_matrix->set(i,0,strtol(line.toLocal8Bit().data(),&endLine,10));
            for(int j = 1; j < col; j++){
                m_matrix->set(i,j,strtol(endLine,&endLine,10));
            }
        }
    }
}

/**
 * @brief parseObjectList Initialise la liste des objets de la scene et leur caractéristiques
 *                                   si le fichier de log a été créé avec l'option m de pbrt
 * @param offset
 */
void ParserAPI::parseObjectList(int offset){
    QString line = "";
    QString data ="";
    QString previousData = "";
    int key = 1;
    int i;
    //On fait une première lecture pour initialiser previousData
    line = ts->readLine();
    offset += line.length() + 1;
    //On verifie qu'on a pas recuperé la ligne de la taille de la matrice
    if(line[1].isDigit()){
        //Si oui on remonte le pointeur de fichier
        ts->seek(ts->pos() - line.length() - 1);
        return;
    }
    //On recupère la valeur de l'objet
    for(i = 2; line[i].isDigit(); i++){}
    for(    ; line[i-1] != ')'; i++){
        previousData += line[i];
    }
    //qInfo()<< "Premier traitement ok";
    do{
        data = "";
        line = ts->readLine();
        offset += line.length() + 1;
        //On verifie qu'on a pas recuperé la ligne de la taille de la matrice
        if(line[1].isDigit()){
            ts->seek(offset - line.length() - 1);
            objectMap.insert(key,previousData);
            break;
        }
        //On recupère la valeur de l'objet
        for(i = 2; line[i].isDigit(); i++){}
        for(    ; line[i-1] != ')'; i++){
            data += line[i];
        }
        //Si les deux données consécutive sont différentes alors on stocke la première
        if(QString::compare(data,previousData) != 0){
            objectMap.insert(key,previousData);
            previousData = data;
        }
        key++;

    }while(line[1] == 'O' && !ts->atEnd());
}




QString ParserAPI::getObject(int obj){
    return objectMap.lowerBound(obj).value();
}

/**
 * @brief isCorrectlySet Certifie le bon fonctionnement de l'API
 * @return  true si l'initialisation est correcte
 *          false sinon
 */
bool ParserAPI::isCorrectlySet(){
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

/**
 * @brief ~ParserAPI Destructeur
 */
ParserAPI::~ParserAPI() {
    delete m_matrix;
}

/**
 * @brief cutNumber Coupe tous les nombres de la string passée en entrée à this.precision chiffres après la virgule
 *                  sans aucun arrondi
 * @param path
 * @return Le chemin avec les valeurs numériques tronquées à this.precision chiffres après la virgule
 */
QString ParserAPI::cutNumber(const QString path){
    QString result = "";
    QString tail = "";
    int offset;
    bool fromBreak;
    //Parcours de la chaine
    for(int i = 0; i < path.size(); i++){
        result += path[i];

        //Si on croise un point on recupère au plus precision char
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

/**
 * @brief getRaw Récupère les données des chemins ayant servi à la création du pixel(x,y) passé en paramètre
 * @param x Coordonnée de ligne du pixel
 * @param y Coordonnée de colonne du pixel
 * @return un QVector<QString> où chaque élément est un chemin
 */
QVector<QString> ParserAPI::getRaw(int x, int y){
    ts->seek(m_matrix->get(x,y));
    QVector<QString> lignes;

    QString ligne = ts->readLine();
    while( !ligne.startsWith("}") ){
        lignes.append(ligne);
        ligne = ts->readLine();
    };
    return lignes;
}

/**
 * @brief getPos Récupère la position du premier chemin ayant servi à calculer le pixel(x,y) passé en paramètre
 * @param x Coordonnée de ligne du pixel
 * @param y Coordonnée de colonne du pixel
 * @return  la position du début de ligne des log du pixel(x,y) dans le fichier
 */
unsigned long long int ParserAPI::getPos(int x, int y){
    return  m_matrix->get(x,y);
}

/**
 * @brief setStreamAt Positionne la tête de lecture sur le premier chemin du pixel(x,y)
 * @param x Coordonnée de ligne du pixel
 * @param y Coordonnée de colonne du pixel
 */
void ParserAPI::setStreamAt(int x, int y){
    ts->seek(m_matrix->get(x,y));
}

/**
 * @brief parseDir Transforme un chemin en une suite de direction, si celui-ci ne contient pas de direction alors la chaine vide est renvoyée
 * @param line Les données d'un chemin
 * @return Une QString composée des directions séparées par un point virgule
 */
QString ParserAPI::parseDir(const QString line){
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

/**
 * @brief parseDir Transforme un chemin en une suite de normales, si celui-ci ne contient pas de normales alors la chaine vide est renvoyée
 * @param line Les données d'un chemin
 * @return Une QString composée des normales séparées par un point virgule
 */
QString ParserAPI::parseNorm(const QString line){
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


/**
 * @brief parseDir Transforme un chemin en une suite de sommets, si celui-ci ne contient pas de sommets alors la chaine vide est renvoyée
 * @param line Les données d'un chemin
 * @return Une QString composée des sommets séparées par un point virgule
 */
QString ParserAPI::parsePath(const QString line){
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

/**
 * @brief clamp
 * @param toClamp Valeur à clamper
 * @param inf Borne inférieure
 * @param sup Borne supérieure
 * @return toClamp clampée entre inf et sup
 */
int clamp(int toClamp, int inf, int sup){
    return toClamp < inf ? inf : toClamp > sup ? sup : toClamp;
}

/**
 * @brief getHexCode Transforme un entier décimal en une chaine de caractères corespondante à sa valeur hexadécimale
 * @param dec Un int en valeur décimal
 * @return Une QString contenant la valeur de l'entrée en hexadécimale
 */
QString getHexCode(const int dec){
    char hex[20];
    itoa(dec,hex,16);
    return QString(hex);
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
QString ParserAPI::extractData(QString str, int *pos, const char info){
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
    for(    ; 0<= *pos && *pos < str.size(); (*pos)++ ){
        if(str[*pos] == info){
            while((*pos) < str.size() - checker && str[(*pos) + checker] != delim && str[(*pos) + checker] != delim2){
                data += str[++(*pos)];
            }
            return data;
        }
    }
    return data;
}

/**
 * @brief parseRaw Transforme la donnée brute issue du fichier de log en une donnée plus ou moins raffinée selon this.rawLevel
 * @param line Les données d'un chemin
 * @return Une QString plus ou moins raffinée
 */
QString ParserAPI::parseRaw(QString line){
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

/**
 * @brief parseColor Récupère la couleur du chemin passé en parametre s'il y en a et calcul se valeur en hexadécimale
 * @param line Les données d'un chemin
 * @return Une QString contenant la valeur RGB et hexadecimale de la couleur du chemin
 */
QString ParserAPI::parseColor(QString line){
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

/**
 * @brief parseQColor Transforme un chemin en sa couleur corespondante
 * @param line Les données d'un chemin
 * @return Une QColor representant la culeur du chemin
 */
QColor ParserAPI::parseQColor(QString line){

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
