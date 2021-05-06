#include "filehandler.h"

FileHandler::FileHandler(QObject *parent) : QObject(parent)
{
    if(file->exists("playlist.txt"))    //checa se o arquivo "playlist.txt" já existe, se existe o exclui para gerar um novo de mesmo nome
    {
        remove("playlist.txt");         //exclui o arquivo "playlist.txt" anterior
    }

    file = new QFile;                   //cria novo arquivo que recebe a playlist atual
    file->setFileName("playlist.txt");  //nomeia arquivo para "playlist.txt"
}


FileHandler::~FileHandler()
{
    file->~QFile(); //destrói o arquivo
}


void FileHandler::writeOnFile(QString musicName, QString musicURL)
{
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))    //abre o arquivo nos modos: escrita, texto e append(para permitir novas adições no arquivo)
    {
        qDebug() << "Erro ao abrir o arquivo!";
        return;
    }

    QTextStream out(file);                          //habilita escrita no arquivo
    out << musicName << "\n" << musicURL << "\n";   //escreve no arquivo

    file->flush();  //limpa o buffer de dados
    file->close();  //fecha o arquivo
}

