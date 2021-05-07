//------------------Desafio Stone 07/05/2021-----------------------//
//---Projeto desenvolvido por Leonardo Fischer Bastos dos Santos---//
//-----------------------------------------------------------------//

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

    spotify = new Spotify(); //cria o objeto para interagir com o Spotify
    connectSpotify(spotify, this);

    spotify->loadPlaylistFile(ui->listWidget_playlist);

}

MainWindow::~MainWindow()
{
    spotify->createPlaylistFile(ui->listWidget_playlist);
    delete ui;
}

//Função para se conectar ao servidor do Spotify
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

//Função acionada quando o botão 'Limpar' é clicado
void MainWindow::on_pushButton_limpar_clicked()
{
    ui->lineEdit_musica->clear(); //limpa a busca anterior
}

//Função acionada quando o botão 'Adicionar música à playlist' é clicado
void MainWindow::on_pushButton_adicionar_clicked()
{
    spotify->addMusic(ui->listWidget_musica, ui->listWidget_playlist);
}

//Função acionada quando o botão 'Remover música da playlist' é clicado
void MainWindow::on_pushButton_remover_clicked()
{
    spotify->removeMusic(ui->listWidget_playlist);
}

//Função acionada quando o botão 'Executar playlist' é clicado
void MainWindow::on_pushButton_executar_clicked()
{
    spotify->playMusic(ui->listWidget_playlist, playlist, player);
}

//Função acionada quando o botão 'Pausar playlist' é clicado
void MainWindow::on_pushButton_pausar_clicked()
{
    spotify->pauseMusic(player);
}
