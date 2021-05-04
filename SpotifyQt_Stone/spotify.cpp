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

        const auto documentJSON = QJsonDocument::fromJson(responseData);        //cria um documento JSON
        const auto objectJSON = documentJSON.object();      //retorna o objeto do documento
        //qDebug() << objectJSON.keys(); //imprime a chave que engloba outros dados no formato JSON (como se fosse um dicionário), neste caso: ("tracks")

        QJsonObject filteredObjectJSON = objectJSON["tracks"].toObject();
        //qDebug() << filteredObjectJSON; //imprime as chave que englobam outros dados no formato JSON, neste caso: ("href", "items", "limit", "next", "offset", "previous", "total")

        QJsonArray arrayJSON = filteredObjectJSON["items"].toArray();       //container que recebe os dados dentro de "items"

        int itemCounter = 0;        //contador que servirá de índice para os itens que aparecerão como resultado

        foreach(const QJsonValue &valueJSON, arrayJSON)
        {
            QJsonObject valueObjectJSON = valueJSON.toObject();
            //qDebug() << "Name: " + valueObjectJSON["name"].toString() + ", URL: " + valueObjectJSON["preview_url"].toString();

            if(valueJSON["preview_url"].toString() != "")       //verificação necessária pois algumas 'preview_url' podem vir vazias, assim estas não aparecerão no resultado da busca
            {
                listResults->addItem(valueObjectJSON["name"].toString());
                searchDictionary.insert(itemCounter, valueJSON["preview_url"].toString());      //insere os itens dentro do dicionário referenciados conforme o valor do contador
                itemCounter++;
            }
        }

        //qDebug() << searchDictionary.keys(); //imprime as chaves que servem de índice para as URLs
        //qDebug() << searchDictionary.values(); //imprime as URLs inseridas no dicionários (teste para verificar se elas tinham sido inseridas)

    });
}

//Função que insere a música na playlist
void Spotify::addMusic(QListWidget *listResults, QListWidget *listPlaylist, QMediaPlaylist *executePlaylist)
{
    if (positionOnPlaylist == -1)       //caso em que todos os itens da playlist foram removidos
        positionOnPlaylist = 0;

    //qDebug() << listResults->currentRow();        //imprime a posição da musica selecionada dentro da ListWidget que contém o resultado da busca

    listPlaylist->addItem(listResults->currentItem()->text());      //adiciona a música selecionada na playlist
    playlistDictionary.insert(positionOnPlaylist, searchDictionary.value(listResults->currentRow()));       //adiciona a URL da música selecionada na playlist
    //executePlaylist->addMedia(QUrl(playlistDictionary.value(listPlaylist->currentRow())));

    //qDebug() << playlistDictionary.values();      //imprime a URL do item adicionado à Playlist
    //qDebug() << positionOnPlaylist;       //imprime a posição do item na Playlist

    positionOnPlaylist++;

}

//Função que remove a música da playlist
void Spotify::removeMusic(QListWidget *listPlaylist)
{
    //qDebug() << playlistDictionary;


    int index = 0;

    if (listPlaylist->currentRow() == -1) //caso em que o botão de remover é apertado sem o cursor estar acionado na ListWidget
        listPlaylist->setCurrentRow(0);
    else if (playlistDictionary.isEmpty()) //caso em que o dicionário está vazio, condição necessária para que não haja índices negativos caso o usuário continue a apagar a lista sem haver itens
    {
        positionOnPlaylist = 0;
        playlistDictionary.clear();
    }
    else if (listPlaylist->currentRow() == 0) //quando a música a ser apagada da lista é a primeira (índice 0)
    {
        playlistDictionary.remove(listPlaylist->currentRow()); //remove a URL da música selecionada do dicionário de URLs
        listPlaylist->takeItem(listPlaylist->currentRow()); //remove a música selecionada na playlist

        for(index = 1; index <= positionOnPlaylist; index++)
        {
            playlistDictionary.insert(index-1,playlistDictionary.value(index));
            playlistDictionary.remove(index);
        }
    }
    else if (listPlaylist->currentRow() < positionOnPlaylist) //quando a música a ser apagada não é a primeira nem a última da lista
    {
        index = listPlaylist->currentRow();

        playlistDictionary.remove(listPlaylist->currentRow()); //remove a URL da música selecionada do dicionário de URLs
        listPlaylist->takeItem(listPlaylist->currentRow()); //remove a música selecionada na playlist

        while (index < positionOnPlaylist)
        {
            playlistDictionary.insert(index,playlistDictionary.value(index+1));
            playlistDictionary.remove(index+1);
            index++;
        }
    }
    else if (listPlaylist->currentRow() == positionOnPlaylist) //quando a música a ser apagada é a última da lista
    {
        playlistDictionary.remove(listPlaylist->currentRow());//remove a URL da música selecionada do dicionário de URLs
        listPlaylist->takeItem(listPlaylist->currentRow()); //remove a música selecionada na playlist
    }


    positionOnPlaylist--; //atualiza a quantidade de músicas dentro da playlist


    //qDebug() << playlistDictionary; //para verificar se as URLs foram ordenadas e/ou removidas corretamente no dicionário

}

//Função que executa a música
void Spotify::playMusic(QListWidget *listPlaylist, QMediaPlaylist *executePlaylist, QMediaPlayer *executePlayer)
{
    if (listPlaylist->currentRow() == -1) //caso em que o botão de executar a playlist é apertado sem o cursor estar acionado na ListWidget
        listPlaylist->setCurrentRow(0);

    executePlaylist->clear(); // apaga toda a QMediaPlaylist para poder construir uma nova

    for (int i = 0; i <= positionOnPlaylist; i++) // loop para inserir as URLs dentro da QMediaPlaylist conforme ordem atual, desta forma a playlist sempre estará atualizada
    {
        executePlaylist->addMedia(QUrl(playlistDictionary.value(i)));
    }
    executePlaylist->setPlaybackMode(QMediaPlaylist::Sequential); // seta o modo de execução da playlist para 'Sequencial', as músicas serão executadas a partir da selecionada na playlist até o final dela

    executePlayer->setPlaylist(executePlaylist); // o objeto QMediaPlayer recebe a playlist para poder executar
    executePlayer->playlist()->setCurrentIndex(listPlaylist->currentRow()); // seta o índice da playlist para que a música seja executada a partir da que foi selecionada
    executePlayer->play(); // toca a playlist
}

//Função que pausa a música
void Spotify::pauseMusic(QMediaPlayer *executePlayer)
{
    executePlayer->pause();
}
