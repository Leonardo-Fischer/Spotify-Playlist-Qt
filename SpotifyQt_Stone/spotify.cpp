#include "spotify.h"


Spotify::Spotify()
{
    QSettings settings(CONFIGURATION_FILE, QSettings::IniFormat);
    settings.beginGroup("Auth");

    ClientID = SPOTIFY_CLIENT_ID;
    ClientIDSecret = SPOTIFY_CLIENT_SECRET;

}

//Estabelece conexão com o Spotify
int Spotify::connectSpotify()
{
    try
    {
        if (!QSslSocket::supportsSsl())
        {
            throw ERROR_NO_SSL_SUPPORT;
        }

        auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
        replyHandler->setCallbackPath("callback");
        spotify.setReplyHandler(replyHandler);
        spotify.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
        spotify.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));
        spotify.setClientIdentifier(ClientID);
        spotify.setClientIdentifierSharedKey(ClientIDSecret);
        spotify.setScope("user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private");

        connect(&spotify, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
        connect(&spotify, &QOAuth2AuthorizationCodeFlow::statusChanged, this, &Spotify::authenticationStatusChanged);
        connect(&spotify, &QOAuth2AuthorizationCodeFlow::granted, this, &Spotify::granted);
        spotify.grant();
        return ERROR_NO_ERROR;

    }
    catch (const int error)
    {
        return error;
    }
}

//SLOT acionado quando o status muda durante o processo de autenticacao
void Spotify::authenticationStatusChanged(QAbstractOAuth::Status status)
{
    QString stat;

    if (status == QAbstractOAuth::Status::Granted)
        stat = "Granted";

    if (status == QAbstractOAuth::Status::TemporaryCredentialsReceived)
        stat = "TemporaryCredentialsReceived";

    qDebug() << "Auth Status changed: " + stat;
}

//SLOT acionado quando o 'granted' SIGNAL é disparado durante o processo de autenticacao
void Spotify::granted()
{
   qDebug() << "Signal granted received";
   //qDebug() << "Token: " + spotify.token();
   isGranted = true;
}
