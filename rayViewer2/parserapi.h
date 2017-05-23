#ifndef PARSERAPI_H_
#define PARSERAPI_H_

#include <QVector>
#include "matrix.h"

using namespace std;

class ParserAPI{


public:

    Matrix * m_matrix = NULL;
    QTextStream * ts;
    QString option;
    int precision = 5;
    int rawLevel = 2;//2 simplifiedRaw,  1 cleanRaw, 0 hardcoreRaw
    int selectedPath = 0;
    QMap<int,QString> objectMap;

    ParserAPI(){}
    ParserAPI(QFile * file);
    void parseObjectList(int offset);
    bool isCorrectlySet();
    unsigned long long int getPos(int x, int y);
    void setStreamAt(int x, int y);
    QVector<QString> getRaw(int x, int y);
    QString parseRaw(const QString line);
    QString parseDir(const QString line);
    QString parseNorm(const QString line);
    QString parsePath(const QString line);
    QString parseColor(QString line);
    QColor parseQColor(QString line);
    QString cutNumber(QString path);
    QString getObject(int obj);

private:
    QString extractData(QString str, int *pos, const char info);

    virtual ~ParserAPI();
private:

};

#endif /* PARSERAPI_H_ */
