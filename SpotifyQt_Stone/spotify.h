#ifndef SPOTIFY_H
#define SPOTIFY_H

//----------------------------------BIBLIOTECAS--------------------------------------//

#include <QObject>
#include <QByteArray>
#include <QDesktopServices>

//Bibliotecas para execução das músicas
#include <QMediaPlaylist> //classe que permite a criação de playlists
#include <QMediaPlayer> //classe que permite a reprodução de mídia

//Bibliotecas para autenticação com o servidor do Spotify
#include <QOAuth2AuthorizationCodeFlow>
#include <QtNetworkAuth>
#include <QSslSocket>

//Biblioteca para trabalhar com List Wigets
#include <QListWidgetItem>

//Biblioteca para trabalhar com arquivos JSON
#include <QJsonDocument>


#include "filehandler.h"

//----------------------------------------------------------------------------------//

//-----------------------------------DEFINIÇÕES-------------------------------------//

//Definições de erro
#define ERROR_NO_SSL_SUPPORT 100
#define ERROR_NO_ERROR 0

//----------------------------------------------------------------------------------//
//---------------------------CHAVES E URLS DO SPOTIFY---------------------------------//

const QByteArray SPOTIFY_CLIENT_ID = "INSIRA AQUI SEU CLIENT ID";
const QByteArray SPOTIFY_CLIENT_SECRET = "INSIRA AQUI SEU SECRET ID";
const QByteArray SPOTIFY_AUTHORIZATION_URL = "https://accounts.spotify.com/authorize";
const QByteArray SPOTIFY_ACCESS_TOKEN_URL = "https://accounts.spotify.com/api/token";

//----------------------------------------------------------------------------------//


class Spotify : public QObject
{
    Q_OBJECT

public:

    Spotify();
    ~Spotify();

    int connectSpotify();

    void searchFor(QString searchMusic, QListWidget *listResults);
    void addMusic(QListWidget *listResults, QListWidget *listPlaylist);
    void removeMusic(QListWidget *listPlaylist);
    void playMusic(QListWidget *listPlaylist, QMediaPlaylist *executePlaylist, QMediaPlayer *executePlayer);
    void pauseMusic(QMediaPlayer *executePlayer);
    void createPlaylistFile(QListWidget *listPlaylist);
    void loadPlaylistFile(QListWidget *listPlaylist);

private:

    void granted();
    void authenticationStatusChanged (QAbstractOAuth::Status status);


    QOAuth2AuthorizationCodeFlow spotify;
    bool isGranted;

    QMap<int, QString> searchDictionary;
    QMap<int, QString> playlistDictionary;

    int positionOnPlaylist;

    FileHandler *playlistFile;
};

#endif // SPOTIFY_H
