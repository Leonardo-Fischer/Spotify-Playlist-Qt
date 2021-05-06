#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void on_pushButton_executar_clicked();

    void on_pushButton_pausar_clicked();


private:
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    Spotify *spotify;

};

void connectSpotify(Spotify * s, MainWindow *w);


#endif // MAINWINDOW_H
