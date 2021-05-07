//------------------Desafio Stone 07/05/2021-----------------------//
//---Projeto desenvolvido por Leonardo Fischer Bastos dos Santos---//
//-----------------------------------------------------------------//

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow application;
    application.show();
    return a.exec();
}
