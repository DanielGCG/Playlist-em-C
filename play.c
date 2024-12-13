#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "display.h"
#include "musics.h"
#include "playlists.h"
#include "credits.h"

//modo interativo
int modoGUI(FILE* arqP, FILE* arqM, FILE* arqE, FILE* arqI, char comando[])
{
	//sair do modo interativo
	if(!strcmp(comando, "exit"))
	{
		printf("\n");
		return 0;
	}
	
	//adicionar música
	if(!strcmp(comando, "addm"))
	{
		adicionarAoCatalogo(arqM, arqI);
		return 1;
	}
	
	//remover música
	if(!strcmp(comando, "delm"))
	{
		removerDoCatalogo(arqP, arqM, arqE, arqI);
		return 1;
	}
	
	//criar playlist
	if(!strcmp(comando, "addp"))
	{
		registrarNovaPlaylist(arqP, arqI);
		return 1;
	}
	
	//remover playlist
	if(!strcmp(comando, "delp"))
	{
		removerPlaylist(arqP, arqE, arqI);
		return 1;
	}
	
	//adicionar música à playlist
	if(!strcmp(comando, "addmp"))
	{
		adicionarMusicasPlaylist(arqP, arqM, arqI);
		return 1;
	}
	
	//remover música da playlist
	if(!strcmp(comando, "delmp"))
	{
		removerMusicasPlaylist(arqM, arqP, arqE, arqI);
		return 1;
	}
	
	//ordena playlist
	if(!strcmp(comando, "ord"))
	{
		ordenarPlaylist(arqP, arqI);
		return 1;
	}
	
	//help
	if(!strcmp(comando, "help"))
	{
		displayHUB(arqP, arqM, arqI);
		displayComandos();
		return 1;
	}
	
	//show
	if(!strcmp(comando, "show"))
	{
		displayExtenso(arqP, arqI);
		return 1;
	}

    //credits
	if(!strcmp(comando, "credits"))
	{
		credits();
		return 1;
	}
	
	//comando inválido
	printf(BOLD "\n\tComando inválido!\n\n" RESET);
	return 1;
}

//função principal
int main(int argc, char **argv)
{
	//dicionário de dados
	FILE *musicas, *icones, *edit, *playlists;
	
	//abrir arquivo musicas.dat
	musicas = fopen("musicas.dat", "rb+");
	
	//não existe ainda
	if(musicas == NULL)
	{
		//criar arquivo musicas.dat
		musicas = fopen("musicas.dat", "wb+");
		
		//erro
		if(musicas == NULL)
		{
			printf("\tErro na criação de <musicas.dat>.\n");
			return -1;
		}
	}
	
	//abrir arquivo playlists.dat
	playlists = fopen("playlists.dat", "rb+");
	
	//não existe ainda
	if(playlists == NULL)
	{
		//criar arquivo playlists.dat
		playlists = fopen("playlists.dat", "wb+");
		
		//erro
		if(playlists == NULL)
		{
			printf("\tErro na criação de <playlists.dat>.\n");
			return -1;
		}
	}
	
	//abrir arquivo edit.dat
	edit = fopen("edit.dat", "wb+");
	
	//erro
	if(edit == NULL)
	{
		printf("\tErro na criação de <edit.dat>.\n");
		return -1;
	}
	
	//abrir arquivo icons.txt
	icones = fopen("icons.txt", "r");
	
	//erro
	if(icones == NULL)
	{
		printf("\tErro na leitura de <icons.txt>.\n");
		return -1;
	}
	
	//entrou modo [GUI]
	if(argc != 2)
	{
		//dados locais
		char comando[30];
	
		//interface
		displayHUB(playlists, musicas, icones);
		displayComandos();
		
		//comando
		printf(RESET BOLD "\n\tPara sair: exit\n\tExecutar :" RESET);
		scanf("%[^\n]", &comando[0]);
		getc(stdin);
		
		while(modoGUI(playlists, musicas, edit, icones, comando))
		{
			fseek(stdin, -sizeof(char), SEEK_END);
			getc(stdin);
			comando[0] = '\0';
			printf(RESET BOLD "\tPara sair: exit\n\tExecutar :" RESET);
			scanf("%[^\n]", &comando[0]);
			getc(stdin);
		}
	}
	
	//adicionar músicas ao catálogo
	if(argc == 2 && !strcmp(argv[1], "-addm"))adicionarAoCatalogo(musicas, icones);
	
	//remover músicas do catálogo
	if(argc == 2 && !strcmp(argv[1], "-delm"))removerDoCatalogo(playlists, musicas, edit, icones);
	
	//adicionar playlist ao registro
	if(argc == 2 && !strcmp(argv[1], "-addp"))registrarNovaPlaylist(playlists, icones);
	
	//remover playlist (nome do registro e arquivo binário)
	if(argc == 2 && !strcmp(argv[1], "-delp"))removerPlaylist(playlists, edit, icones);
	
	//adicionar música à playlist
	if(argc == 2 && !strcmp(argv[1], "-addmp"))adicionarMusicasPlaylist(playlists, musicas, icones);
	
	//remover música de playlist
	if(argc == 2 && !strcmp(argv[1], "-delmp"))removerMusicasPlaylist(musicas, playlists, edit, icones);
	
	//ordenar músicas da playlist
	if(argc == 2 && !strcmp(argv[1], "-ord"))ordenarPlaylist(playlists, icones);
	
	//mostrar playlist em modo extenso
	if(argc == 2 && !strcmp(argv[1], "-show"))displayExtenso(playlists, icones);

    //creditos
	if(argc == 2 && !strcmp(argv[1], "-credits"))credits();
	
	
	//saída
	fclose(playlists);
	fclose(musicas);
	fclose(icones);
	fclose(edit);
	
	return 0;
}

	
