//------------------Desafio Stone 07/05/2021-----------------------//
//---Projeto desenvolvido por Leonardo Fischer Bastos dos Santos---//
//-----------------------------------------------------------------//

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QFile>
#include <QListWidgetItem>
#include <QTextStream>
#include <QDebug>

class FileHandler : public QObject
{
    Q_OBJECT

public:
    explicit FileHandler(QObject *parent = 0);
    virtual ~FileHandler();
    void writeOnFile(QString musicName, QString musicURL);

private:
    QFile *file;
};

#endif // FILEHANDLER_H
