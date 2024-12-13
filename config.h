//tamanho máximo
#define NOME_MAX 30
#define ARTISTA_MAX 30
#define ALBUM_MAX 30
#define PLAYLIST_MAX 30

/*
	antes de alterar qualquer _MAX
		-> rm musicas.dat
		-> rm playlists.dat
		-> rm ./registro/(asteristico)
		-> alterar _MAX e NMAX (se preciso) e salvar config
		-> gcc -Wall play.c -o play.out
*/

#define NMAX 40

/*
	coloque ^^ o maior valor _MAX
	é usado para o display extenso
*/

//formatação
#define BOLD "\e[1m"
#define RESET "\033[0m \e[m"
#define GREEN "\033[0;32m" 
#define CYAN "\033[0;36m"

//tempo de animação
#define DELAY 10000

//struct MUSICA
typedef struct {
	char nome[NOME_MAX];
	char artista[ARTISTA_MAX];
	char album[ALBUM_MAX];
	int inclusao;
	int duracao;
	long int id;
} MUSICA;

//structs da PLAYLIST
typedef struct aux {
	MUSICA track;
	struct aux* prox;
} ELEMENTO;

typedef ELEMENTO* PONT;

typedef struct {
	PONT inicio;
	PONT fim;
} PLAYLIST;












