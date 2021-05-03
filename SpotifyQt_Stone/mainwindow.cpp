#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMediaContent>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    playlist = new QMediaPlaylist; //cria o objeto para playlist de mídia

    player = new QMediaPlayer; //cria o objeto para execução de mídia
    connect(player, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(currentMediaChanged(QMediaContent)));

    spotify = new Spotify(); //cria o objeto para interagir com o Spotify
    connectSpotify(spotify, this);

    //loadPlaylist();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void connectSpotify(Spotify * s, MainWindow *w)
{
    int error = s->connectSpotify();

    switch (error)
    {
        case ERROR_NO_SSL_SUPPORT:
            QMessageBox::critical(w, w->tr("Spotify Player"), w->tr("Suporte a SSL não encontrado!"));
            exit(1);
        case ERROR_NO_ERROR:
        default:
            return;

    }
}


//Função acionada quando o botão 'Buscar' é clicado
void MainWindow::on_pushButton_buscar_clicked()
{
    ui->listWidget_musica->clear(); //limpa os resultados da busca anterior
    spotify->searchFor(ui->lineEdit_musica->text(), ui->listWidget_musica);
}


void MainWindow::on_pushButton_limpar_clicked()
{
    ui->lineEdit_musica->clear(); //limpa a busca anterior
}

void MainWindow::on_pushButton_adicionar_clicked()
{
    spotify->addMusic(ui->listWidget_musica, ui->listWidget_playlist);
}

void MainWindow::on_pushButton_remover_clicked()
{
    spotify->removeMusic(ui->listWidget_playlist);
}
