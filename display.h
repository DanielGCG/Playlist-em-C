//display da logo do programa
void displayLogo(FILE* arqI)
{
	//dados locais
	char linha[255];

	//pos inicial
	rewind(arqI);

	//formatação
	printf(GREEN BOLD);
	
	//logo do progama
	for(int l = 0; l != 12; l++)
	{
		fgets(linha, 255, arqI);
		printf("%s", linha);
		usleep(DELAY);
	} 
	
	//desfazer formatação
	printf(RESET);
}

//display de músicas do catálogo
void displayMusicas(FILE* arqM)
{
	//dados locais
	fseek(arqM, 0, SEEK_END);
	long int fim = ftell(arqM);
	MUSICA temp;
	
	//pos inicial
	rewind(arqM);
	
	//catálogo
	printf(CYAN BOLD "\tcatálogo:" RESET);
	
	//se estiver vazia
	if(fim == 0)
	{
		printf(CYAN BOLD "[vazio]\n\n" RESET);
		return;
	}
	
	//borda superior
	printf(CYAN BOLD "\n\n\t╔");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╦");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╗\n" RESET);
	
	//meio
	while(!feof(arqM) && ftell(arqM) != fim)
	{
		int alg = 1;
		fread(&temp, sizeof(MUSICA), 1, arqM);
		for(int num = temp.id; num >= 10; alg++, num /= 10);
		printf(CYAN BOLD "\t║" RESET " %s %*s" CYAN BOLD "║" RESET, temp.nome, NOME_MAX - ((int)strlen(temp.nome)), " ");
		printf(CYAN BOLD " ID:" RESET "%ld %*s" CYAN BOLD "║\n" RESET, temp.id, NOME_MAX - (alg + 4), " ");
		
	}

	//borda inferior
	printf(CYAN BOLD "\t╚");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╩");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╝\n\n" RESET);
}

//display de playlists registradas
void displayPlaylists(FILE* arqP)
{
	//dados locais
	fseek(arqP, 0, SEEK_END);
	long int fim = ftell(arqP);
	char nome[PLAYLIST_MAX];
	int quebraLinha = 0;
	
	//pos inicial
	rewind(arqP);
	
	//playlists
	printf(GREEN BOLD "\n\tplaylists:" RESET);
	
	//vazia
	if(fim == 0)
	{
		printf(GREEN BOLD "[vazio]\n" RESET);
		return;
	}
	
	//borda superior
	printf(GREEN BOLD "\n\n\t╔");
	for(int i = 0; i != PLAYLIST_MAX+4; i++)printf("═");
	printf("╦");
	for(int i = 0; i != PLAYLIST_MAX+4; i++)printf("═");
	printf("╗\n" RESET);
	
	//meio
	while(!feof(arqP) && ftell(arqP) != fim)
	{
		quebraLinha++;
		fread(&nome, sizeof(nome), 1, arqP);
		if(quebraLinha % 2 != 0)
		{
			printf(GREEN BOLD "\t║" RESET " %s %*s" GREEN BOLD "║" RESET, nome, (PLAYLIST_MAX+1) - ((int)strlen(nome)), " ");
		}
		if(quebraLinha % 2 == 0)
		{
			printf(" %s %*s" GREEN BOLD "║\n" RESET, nome, (PLAYLIST_MAX+1) - ((int)strlen(nome)), " ");
		}
	} quebraLinha++;
	
	if(quebraLinha % 2 == 0)printf("%*s" GREEN BOLD "║\n" RESET, PLAYLIST_MAX+3, " ");
	
	//borda inferior
	printf(GREEN BOLD "\t╚");
	for(int i = 0; i != PLAYLIST_MAX+4; i++)printf("═");
	printf("╩");
	for(int i = 0; i != PLAYLIST_MAX+4; i++)printf("═");
	printf("╝\n" RESET);
}

//quase que literalmente a mesma coisa que displayMusicas
void displayMusicasPlaylist(FILE* arqR, char nome[])
{
	//dados locais
	fseek(arqR, 0, SEEK_END);
	long int fim = ftell(arqR);
	MUSICA temp;
	
	//pos inicial
	rewind(arqR);
	
	//playlist
	printf(GREEN BOLD "\n\tplaylist (%s.dat):" RESET, nome);
	
	//se estiver vazia
	if(fim == 0)
	{
		printf(GREEN BOLD "[vazia]\n\n" RESET);
		return;
	}
	
	//borda superior
	printf(GREEN BOLD "\n\n\t╔");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╦");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╗\n" RESET);
	
	//meio
	while(!feof(arqR) && ftell(arqR) != fim)
	{
		int alg = 1;
		fread(&temp, sizeof(MUSICA), 1, arqR);
		for(int num = temp.id; num >= 10; alg++, num /= 10);
		printf(GREEN BOLD "\t║" RESET " %s %*s" GREEN BOLD "║" RESET, temp.nome, NOME_MAX - ((int)strlen(temp.nome)), " ");
		printf(GREEN BOLD " ID:" RESET "%ld %*s" GREEN BOLD "║\n" RESET, temp.id, NOME_MAX - (alg + 4), " ");
	}

	//borda inferior
	printf(GREEN BOLD "\t╚");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╩");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╝\n\n" RESET);
}

//display todos os atributos das músicas da playlist
void displayMusicasPlaylistExtenso(FILE* arqR, char nome[])
{
	//dados locais
	fseek(arqR, 0, SEEK_END);
	long int fim = ftell(arqR);
	MUSICA temp;
	
	//pos inicial
	rewind(arqR);
	
	//playlist
	printf(GREEN BOLD "\n\tplaylist (%s.dat):" RESET, nome);
	
	//se estiver vazia
	if(fim == 0)
	{
		printf(GREEN BOLD "[vazia]\n\n" RESET);
		return;
	}
	
	//borda superior
	printf(GREEN BOLD "\n\n\t╔");
	for(int i = 0; i != NMAX+12; i++)printf("═");
	printf("╗\n" RESET);
	
	//meio
	while(!feof(arqR) && ftell(arqR) != fim)
	{
		//para formatar números
		int alg = 1;
		
		//música
		fread(&temp, sizeof(MUSICA), 1, arqR);
		
		//nome
		printf(GREEN BOLD "\t║ Nome    :" RESET "%s %*s" GREEN BOLD "║\n", temp.nome, NMAX - ((int)strlen(temp.nome)), " ");
		//artista
		printf("\t║ Artista :" RESET "%s %*s" GREEN BOLD "║\n", temp.artista, NMAX - ((int)strlen(temp.artista)), " ");
		//album
		printf("\t║ Album   :" RESET "%s %*s" GREEN BOLD "║\n", temp.album, NMAX - ((int)strlen(temp.album)), " ");
		//duração
		for(int num = temp.duracao; num >= 10; alg++, num /= 10);
		printf("\t║ Duração :" RESET "%d seg %*s" GREEN BOLD "║\n", temp.duracao, NMAX - (alg + 4), " ");
        alg = 1;

		//id
		for(int num = temp.id; num >= 10; alg++, num /= 10);
		printf("\t║ ID      :" RESET "%ld %*s" GREEN BOLD "║\n", temp.id, NMAX - (alg), " ");
        alg = 1;
		//inclusão
		for(int num = temp.inclusao; num >= 10; alg++, num /= 10);
		printf("\t║ Inclusão:" RESET "%d %*s" GREEN BOLD "║\n", temp.inclusao, NMAX - (alg), " ");
        
		//borda intermediária
		if(ftell(arqR) != fim)
		{
			printf(GREEN BOLD "\t╠");
			for(int i = 0; i != NMAX+12; i++)printf("═");
			printf("╣\n" RESET);
		}
	}

	//borda inferior
	printf(GREEN BOLD "\t╚");
	for(int i = 0; i != NMAX+12; i++)printf("═");
	printf("╝\n\n" RESET);
}

//display comandos disponíveis
void displayComandos()
{
	//cabeçário
	printf(BOLD "\t[GUI]\t\t[funcionalidade]\t\t[terminal]\n\n");
	usleep(DELAY);

	//-help
	printf("\t[1]:" RESET " help\t");
	printf(GREEN BOLD "║" RESET "-modo interface [GUI]\t\t" GREEN BOLD "║" RESET "./play.out\n");
	usleep(DELAY);
		
	//-add
	printf(BOLD"\t[2]:" RESET " addm\t");
	printf(GREEN BOLD "║" RESET "-adiciona música ao catálogo\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -addm\n" RESET);
	usleep(DELAY);
	
	//-del
	printf(BOLD "\t[3]:" RESET " delm\t");
	printf(GREEN BOLD "║" RESET "-remove música do catálogo\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -delm\n" RESET);
	usleep(DELAY);
	
	//-addp
	printf(BOLD "\t[4]:" RESET " addp\t");
	printf(GREEN BOLD "║" RESET "-cria nova playlist\t\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -addp\n" RESET);
	usleep(DELAY);

	//-delp
	printf(BOLD "\t[5]:" RESET " delp\t");
	printf(GREEN BOLD "║" RESET "-remove playlist salva\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -delp\n" RESET);
	usleep(DELAY);
	
	//-addmp
	printf(BOLD "\t[6]:" RESET " addmp\t");
	printf(GREEN BOLD "║" RESET "-adiciona música à playlist\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -addmp\n" RESET);
	usleep(DELAY);
	
	//-delmp
	printf(BOLD "\t[7]:" RESET " delmp\t");
	printf(GREEN BOLD "║" RESET "-remove música da playlist\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -delmp\n" RESET);
	usleep(DELAY);
	
	//-ord
	printf(BOLD "\t[8]:" RESET " ord\t");
	printf(GREEN BOLD "║" RESET "-ordena playlist\t\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -ord\n" RESET);
	usleep(DELAY);
	
	//-show
	printf(BOLD "\t[9]:" RESET " show\t");
	printf(GREEN BOLD "║" RESET "-mostra playlist\t\t" GREEN BOLD "║" RESET "./play.out" GREEN BOLD " -show\n\n" RESET);
	usleep(DELAY);
}


//display da tela principal
void displayHUB(FILE* arqP, FILE* arqM, FILE* arqI)
{
	//dados locais
	char nomeP[PLAYLIST_MAX];
	long int tamM, tamP;
	char linha[255];
	MUSICA temp;
	
	//limpar terminal
	system("reset");
	
	//logo do programa
	displayLogo(arqI);
	
	//tamanho de musicas.dat
	fseek(arqM, 0, SEEK_END);
	tamM = ftell(arqM);
	
	//tamanho de playlists.dat
	fseek(arqP, 0, SEEK_END);
	tamP = ftell(arqP);
	
	//pos inicial
	rewind(arqM);
	rewind(arqP);
	
	//pos ascii
	fseek(arqI, 772, SEEK_SET);
	
	//arte ascii intercalada
	for(int lin = 0; lin != 21; lin++)
	{
		//borda superior
		if(lin == 0)
		{
			printf(BOLD "\n\t músicas salvas:\t\t  playlists salvas:" RESET GREEN BOLD);
			printf("\n\n\t╔");
			for(int i = 0; i != NOME_MAX+3; i++)printf("═");
			printf("╦");
			for(int i = 0; i != PLAYLIST_MAX+3; i++)printf("═");
			printf("╗");
		}
		
		//meio
		else
		{
			//músicas
			if(!feof(arqM) && tamM != ftell(arqM))
			{
				fread(&temp, sizeof(MUSICA), 1, arqM);
				printf("\t║" RESET BOLD "%s %*s" RESET GREEN BOLD "║", temp.nome, NOME_MAX - ((int)strlen(temp.nome)), " ");
			}
			else printf("\t║" RESET BOLD " %*s" RESET GREEN BOLD "║", NOME_MAX, " ");
			
			//playlists
			if(!feof(arqP) && tamP != ftell(arqP))
			{
				fread(&nomeP, sizeof(nomeP), 1, arqP);
				printf(RESET BOLD "%s %*s" RESET GREEN BOLD "║", nomeP, (PLAYLIST_MAX) - ((int)strlen(nomeP)), " ");
			}
			else printf(" %*s  ║", PLAYLIST_MAX, " ");
		}
		
		//arte ascii
		fgets(linha, 255, arqI);
		printf("\t%s", linha);
		usleep(DELAY);
	}
	
	//músicas restantes
	while(!feof(arqM) && tamM != ftell(arqM))
	{
		fread(&temp, sizeof(MUSICA), 1, arqM);
		printf(BOLD "\t║ %s %*s ║", temp.nome, NOME_MAX - ((int)strlen(temp.nome)), " ");
		
		//se tiver playlist
		if(!feof(arqP) && tamP != ftell(arqP))
		{
			fread(&nomeP, sizeof(nomeP), 1, arqP);
			printf("%s %*s  ║\n", nomeP, (PLAYLIST_MAX) - ((int)strlen(nomeP)), " ");
		}
		else printf("  %*s ║\n", PLAYLIST_MAX, " ");
	}
	
	//playlist restantes
	while(!feof(arqP) && tamP != ftell(arqP))
	{
		//não tem mais música
		printf("\t║  %*s ║", NOME_MAX, " ");
		
		fread(&nomeP, sizeof(nomeP), 1, arqP);
		printf("%s %*s  ║\n", nomeP, (PLAYLIST_MAX) - ((int)strlen(nomeP)), " ");
	}
	
	//borda inferior
	printf("\t╚");
	for(int i = 0; i != NOME_MAX+3; i++)printf("═");
	printf("╩");
	for(int i = 0; i != PLAYLIST_MAX+3; i++)printf("═");
	printf("╝\n\n" RESET);
}
















