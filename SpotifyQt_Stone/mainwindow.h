#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlaylist> //classe que permite a criação de playlists
#include <QMediaPlayer> //classe que permite a reprodução de mídia
#include "spotify.h"
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_buscar_clicked();

    void on_pushButton_limpar_clicked();

    void on_pushButton_adicionar_clicked();

    void on_pushButton_remover_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    Spotify *spotify;

};

void connectSpotify(Spotify * s, MainWindow *w);


#endif // MAINWINDOW_H
