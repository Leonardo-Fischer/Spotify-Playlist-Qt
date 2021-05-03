#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QSettings>
#include <QByteArray>
#include <QDesktopServices>

//Bibliotecas para autenticação com o servidor do Spotify
#include <QOAuth2AuthorizationCodeFlow>
#include <QtNetworkAuth>
#include <QSslSocket>

//Biblioteca para trabalhar com List Wigets
#include <QListWidgetItem>

//Biblioteca para trabalhar com arquivos JSON
#include <QJsonDocument>

//-----------------------------------DEFINIÇÕES-------------------------------------//

//Definições de configuração
#define CONFIGURATION_FILE "config.ini"

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

    int connectSpotify();

    void searchFor(QString searchMusic, QListWidget *listResults);
    void addMusic(QListWidget *listResults, QListWidget *listPlaylist);
    void removeMusic(QListWidget *listPlaylist);

private:

    void granted();
    void authenticationStatusChanged (QAbstractOAuth::Status status);

    QString ClientID;
    QString ClientIDSecret;

    QOAuth2AuthorizationCodeFlow spotify;
    bool isGranted;

    QMap<int, QString> searchDictionary;
    QMap<int, QString> playlistDictionary;

    int positionOnPlaylist = 0;
};

#endif // SPOTIFY_H
