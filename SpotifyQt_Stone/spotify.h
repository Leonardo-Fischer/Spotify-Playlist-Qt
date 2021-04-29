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

//-------------CONSTANTES----------------//

//Constantes de configuração
#define CONFIGURATION_FILE "config.ini"

//Constantes de erro
#define ERROR_NO_SSL_SUPPORT 100
#define ERROR_NO_ERROR 0

//---------------------------------------//

//-----------CHAVES DO SPOTIFY-----------//

const QByteArray SPOTIFY_CLIENT_ID = "INSIRA SEU ClientID AQUI";
const QByteArray SPOTIFY_CLIENT_SECRET = "INSIRA SEU Client Secret AQUI";


class Spotify : public QObject
{
    Q_OBJECT

public:
    Spotify();

    int connectSpotify();

private:

    void setupConnection();
    void granted();
    void authenticationStatusChanged (QAbstractOAuth::Status status);

    QString ClientID;
    QString ClientIDSecret;

    QOAuth2AuthorizationCodeFlow spotify;
    bool isGranted;

};

#endif // SPOTIFY_H
