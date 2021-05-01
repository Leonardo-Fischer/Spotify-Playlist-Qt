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
        spotify.setAuthorizationUrl(QUrl(SPOTIFY_AUTHORIZATION_URL));
        spotify.setAccessTokenUrl(QUrl(SPOTIFY_ACCESS_TOKEN_URL));
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

//Função acionada quando o status muda durante o processo de autenticacao
void Spotify::authenticationStatusChanged(QAbstractOAuth::Status status)
{
    QString stat;

    if (status == QAbstractOAuth::Status::Granted)
        stat = "Granted";

    if (status == QAbstractOAuth::Status::TemporaryCredentialsReceived)
        stat = "TemporaryCredentialsReceived";

    qDebug() << "Auth Status changed: " + stat;
}

//Função acionada quando o 'granted' SIGNAL é disparado durante o processo de autenticacao
void Spotify::granted()
{
   qDebug() << "Signal granted received";
   //qDebug() << "Token: " + spotify.token();
   isGranted = true;
}


//Função que busca a música no Spotify
void Spotify::searchFor(QString searchMusic, QListWidget *listResults)
{
    QString type = "type=track"; //especifica o tipo de busca que será feita, neste caso busca apenas pela música
    QUrl requestURL ("https://api.spotify.com/v1/search?q=" + searchMusic + "&" + type);

    searchDictionary.clear(); //Dicionário que mapeia os itens resultantes da busca

    auto reply = spotify.get(requestURL);

    connect(reply, &QNetworkReply::finished, [=]()
    {
        const auto responseData = reply->readAll(); //recebe toda a resposta do GET request
        //qDebug() << responseData;

        const auto documentJSON = QJsonDocument::fromJson(responseData); //cria um documento JSON
        const auto objectJSON = documentJSON.object(); //retorna o objeto do documento
        //qDebug() << objectJSON.keys(); //imprime a chave que engloba outros dados no formato JSON (como se fosse um dicionário), neste caso: ("tracks")

        QJsonObject filteredObjectJSON = objectJSON["tracks"].toObject();
        //qDebug() << filteredObjectJSON; //imprime as chave que englobam outros dados no formato JSON, neste caso: ("href", "items", "limit", "next", "offset", "previous", "total")

        QJsonArray arrayJSON = filteredObjectJSON["items"].toArray(); //container que recebe os dados dentro de "items"
        int itemCounter = 0;

        foreach(const QJsonValue &valueJSON, arrayJSON)
        {
            QJsonObject valueObjectJSON = valueJSON.toObject();
            //qDebug() << "Name: " + valueObjectJSON["name"].toString() + ", URL: " + valueObjectJSON["preview_url"].toString();

            if(valueJSON["preview_url"].toString() != "") //verificação necessária pois algumas 'preview_url' podem vir vazias, assim estas não aparecerão no resultado da busca
            {
                itemCounter++;
                listResults->addItem(valueObjectJSON["name"].toString());
                searchDictionary.insert(itemCounter, valueJSON["preview_url"].toString());//ordena os itens dentro do dicionário
            }
        }

    });







}
