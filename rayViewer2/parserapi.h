#ifndef PARSERAPI_H_
#define PARSERAPI_H_

#include <QObject>
#include "matrix.h"

using namespace std;

class parserAPI : QObject {


public:

    Matrix * m_matrix = NULL;
    QTextStream * ts;
    QString option;
    int precision = 5;
    int rawLevel = 2;//2 simplifiedRaw,  1 cleanRaw, 0 hardcoreRaw
    int selectedPath = 0;

    parserAPI(){}
    parserAPI(QFile * file);
    bool isCorrectlySet();
    unsigned long long int getPos(int x, int y);
    void setStreamAt(int x, int y);
    QString getPath(int x, int y);
    QString getDir(int x, int y);
    QString getNorm(int x, int y);
    QVector<QString> getRaw(int x, int y);
    QString parseRaw(QString line);
    QString parseDir(QString line);
    QString parseNorm(QString line);
    QString parsePath(QString line);
    QString parseColor(QString line);
    QColor parseQColor(QString line);
    QString extractData(QString str, int *pos, const char info);
    QString cutNumber(QString path);
    QColor getTrueColor(QVector<QString> rawData);

    virtual ~parserAPI();
private:

};

#endif /* PARSERAPI_H_ */
