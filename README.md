# Spotify-Playlist-Qt
Projeto desenvolvido em Qt (C++) que utiliza a Spotify Web API, cria e executa uma Playlist

## Funcionalidades
1. Busca por músicas (amostras de 30 segundos)
2. Adiciona/exclui músicas na playlist
3. Executa/pára músicas da playlist

## Observações/Instruções de Uso
* Para execução do projeto é preciso ter instalado o Software QtCreator em sua máquina, sendo necessário também a instalação do componente "Qt Network Authorization" (caso ainda não esteja instalado).
* Para abrir o projeto com o QtCreator, abra o arquivo "SpotifyQt_Stone.pro".
* No arquivo header "spotify.h" estão fornecidas as minhas próprias chaves Client ID e Client Secret, as mantive para facilitar a execução do programa.
* Uma vez criada a playlist, ela fica salva em um arquivo "playlist.txt" que se encontrará na pasta 'build' gerada no momento de execução.
* Quando executado pela primeira vez, a aplicação o redirecionará para uma página de autenticação do Spotify, onde é solicitado seu login e senha. Apenas o Spotify tem acesso a seus dados de login e senha, a aplicação não os salva e nem tem acesso a eles.
* Aplicação testada em sistemas operacionais Windows e Linux (desenvolvida para Desktop).
* No momento não há projeto de testes.



[Acesse meu Github!](https://github.com/Leonardo-Fischer)
