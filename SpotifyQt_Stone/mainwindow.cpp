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
    connect(player, SIGNAL(currentMediaChanged(QMediaContent)), this, SLOT(currentMediaChanged(QMediaContent))); //havia um 'const'

    spotify = new Spotify(); //cria o objeto para interagir com o Spotify
    connectSpotify(spotify, this);

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
