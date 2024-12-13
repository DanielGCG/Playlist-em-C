
//inicializa playlist
void inicializarPlaylist(PLAYLIST* p)
{
	p->inicio = NULL;
	p->fim = NULL;
}

//retorna tamanho da playlist
int tamanhoPlaylist(PLAYLIST* p)
{
	PONT pos = p->inicio;
	int tam = 0;
	
	while(pos != NULL)
	{
		tam++;
		pos = pos->prox;
	}
	
	return tam;
}

//insere nova música em estrutura de PLAYLIST
void enQueue(PLAYLIST* p, MUSICA track)
{
	PONT novo = (PONT)malloc(sizeof(ELEMENTO));
	novo->track = track;
	novo->prox = NULL;
	
	if(p->inicio == NULL)p->inicio = novo;
	else p->fim->prox = novo;
	p->fim = novo;
}

//gera estrutura PLAYLIST do registro 
void passarParaStruct(FILE* arqR, PLAYLIST* p)
{
	//dados locais
	long int tam;
	MUSICA temp;
	
	//tamanho do registro
	fseek(arqR, 0, SEEK_END);
	tam = ftell(arqR);
	
	//pos inicial
	rewind(arqR);
	
	//receber musicas salvas no registro
	while(!feof(arqR) && ftell(arqR) != tam)
	{
		fread(&temp, sizeof(MUSICA), 1, arqR);
		enQueue(p, temp);
	}
}

//reinicializa struct PLAYLIST
void reinicializarPlaylist(PLAYLIST* p)
{
	PONT pos = p->inicio;
	
	while(pos != NULL)
	{
		PONT apagar = pos;
		pos = pos->prox;
		free(apagar);
	}
	
	p->inicio = NULL;
	p->fim = NULL;
}

//verifica se playlist já existe no registro
int existeNoRegistro(FILE* arqP, char nome[])
{
	//dados locais
	char temp[PLAYLIST_MAX];

	//pos inicial
	rewind(arqP);
	
	//checagem
	while(!feof(arqP))
	{
		fread(&temp, sizeof(temp), 1, arqP);
		if(!strcmp(temp, nome))return 1;
	}
	
	//não está no registro
	return 0;
}

//remove 1 música da playlist
void removerDaPlaylist(FILE* arqR, FILE* arqE, char path[], int ID)
{
	//dados locais
	long int tam;
	MUSICA temp;

	//reinicializar edit.dat
	fclose(arqE);
	arqE = fopen("edit.dat", "wb+");
	
	//erro
	if(arqE == NULL)
	{
		printf("\tErro na criação de <edit.dat>.\n");
		return;
	}

	//tamanho de arqR
	fseek(arqR, 0, SEEK_END);
	tam = ftell(arqR);

	//pos inicial de arqR
	rewind(arqR);
	
	//passagem para edit.dat
	while(!feof(arqR) && ftell(arqR) != tam)
	{
		fread(&temp, sizeof(MUSICA), 1, arqR);
		if(temp.id == ID)continue;
		fwrite(&temp, sizeof(MUSICA), 1, arqE);
	}
	
	//"reinicializar" registro
	fclose(arqR);
	arqR = fopen(path, "wb");
	
	//erro
	if(arqR == NULL)
	{
		printf("\tErro ao \"reinicializar\" registro em: %s.\n", path);
		return;
	}

	//tamanho de edit.dat
	fseek(arqE, 0, SEEK_END);
	tam = ftell(arqE);

	//pos inicial edit.dat
	rewind(arqE);
	
	//passagem para registro
	while(!feof(arqE) && ftell(arqE) != tam)
	{
		fread(&temp, sizeof(MUSICA), 1, arqE);
		fwrite(&temp, sizeof(MUSICA), 1, arqR);
	}
	
	//salvar registro
	fclose(arqR);
	arqR = fopen(path, "rb+");
	
	//erro
	if(arqR == NULL)
	{
		printf("\tErro ao \"reinicializar\" registro em: %s.\n", path);
		return;
	}
}

//atualiza playlists após remover música do catálogo
void atualizarPlaylists(FILE* arqP, FILE* arqE, int ID)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	long int tam;
	FILE* arqR;
	
	//quantas playlists atualizar
	fseek(arqP, 0, SEEK_END);
	tam = ftell(arqP);
	
	//pos inicial
	rewind(arqP);
	
	//iteração por playlists
	while(!feof(arqP) && ftell(arqP) != tam)
	{
		//ler playlist para alterar
		fread(&nomeP, sizeof(nomeP), 1, arqP);
		
		//path do registro
		snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
		
		//abrir registro
		arqR = fopen(path, "rb+");
		
		//erro de registro
		if(arqR == NULL)
		{
			printf("\tErro ao acessar registro em: %s.\n", path);
			continue;
		}
		
		//checar se música está na playlist
		if(existeNoCatalogo(arqR, ID))
		{
			removerDaPlaylist(arqR, arqE, path, ID);
			printf(GREEN BOLD "\t%s.bat" RESET BOLD " atualizada!\n" RESET, nomeP);
		}
	}
}

//registra nome de playlists em playlists.dat
void registrarNovaPlaylist(FILE* arqP, FILE* arqI)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	FILE* novoRegistro;
	int qtd;
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayPlaylists(arqP);
	
	//quantas playlists
	printf(BOLD "\n\tDeseja criar quantas playlists? " RESET);
	scanf("%d", &qtd);
	
	//registrar
	while(qtd > 0)
	{
		//limpar stdin
		getc(stdin);
		
		//nome
		printf(BOLD "\n\tNome da playlist: " RESET);
		scanf("%[^\n]", &nomeP[0]);
		
		//limite de caracteres
		if(strlen(nomeP) > PLAYLIST_MAX)
		{
			printf(BOLD "\tO nome não pode exceder" GREEN " %d" RESET BOLD "caracteres!\n" RESET, PLAYLIST_MAX);
			continue;
		}
		
		//verificar se ja existe
		if(existeNoRegistro(arqP, nomeP))
		{
			printf(BOLD "\tEssa playlist já existe!\n" RESET);
			continue;
		}
		
		//path para registrar
		snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
		printf(BOLD "\tRegistrado em   : " RESET "%s\n", path);
		
		//registrar no path definido
		novoRegistro = fopen(path, "wb");
		
		//erro de registro
		if(novoRegistro == NULL)
		{
			printf("\tErro no registro de <%s.dat>.\n", nomeP);
			continue;
		}
		
		//registrar nome em playlists.dat
		fwrite(&nomeP, sizeof(nomeP), 1, arqP);
		qtd--;
	} 
	
	//novo registro
	displayPlaylists(arqP);
	printf("\n");
	
	//fechar registro
	fclose(novoRegistro);
}

//remove arquivo binário do registro e nome de playlists.dat
void removerPlaylist(FILE* arqP, FILE* arqE, FILE* arqI)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	char temp[PLAYLIST_MAX];
	fseek(arqP, 0, SEEK_END);
	long int tam = ftell(arqP);
	int qtd;
	
	//pos inicial
	rewind(arqP);
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayPlaylists(arqP);
	
	//quantas playlists
	printf(BOLD "\n\tDeseja remover quantas playlists? " RESET);
	scanf("%d", &qtd);
	
	//remover
	while(qtd > 0)
	{
		//limpar stdin
		getc(stdin);
		
		//nome
		printf(BOLD "\n\tNome da playlist: " RESET);
		scanf("%[^\n]", &nomeP[0]);
		
		//limite de caracteres
		if(strlen(nomeP) > PLAYLIST_MAX)
		{
			printf(BOLD "\tO nome não pode exceder" GREEN " %d" RESET BOLD "caracteres!\n" RESET, PLAYLIST_MAX);
			continue;
		}
		
		//verificar se ja existe
		if(!existeNoRegistro(arqP, nomeP))
		{
			printf(BOLD "\tEssa playlist não existe!\n" RESET);
			continue;
		}
		
		//path para remover
		snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
		printf(BOLD "\tRemovido        : " RESET "%s\n", path);
		
		//remover no path definido
		if(remove(path) != 0)
		{
			printf("\tErro na remoção de <%s.bat>.\n", nomeP);
			return;
		}
		
		//remover nome de playlists.dat (igual removerDoCatalogo)
		
		//reinicializar edit.dat
		fclose(arqE);
		arqE = fopen("edit.dat", "wb+");
		
		//erro
		if(arqE == NULL)
		{
			printf("\tErro na criação de <edit.dat>.\n");
			return;
		}
		
		//pos inicial
		rewind(arqP);
		
		//passagem para edit.dat
		while(!feof(arqP) && ftell(arqP) != tam)
		{
			fread(&temp, sizeof(temp), 1, arqP);
			if(!strcmp(temp, nomeP))continue;
			fwrite(&temp, sizeof(temp), 1, arqE);
		}
		
		//"reinicializar" playlists.dat
		fclose(arqP);
		arqP = fopen("playlists.dat", "wb");
		
		//erro
		if(arqP == NULL)
		{
			printf("\tErro ao \"reinicializar\" <playlists.dat>.\n");
			return;
		}

		//tamanho de edit.dat
		fseek(arqE, 0, SEEK_END);
		tam = ftell(arqE);
	
		//pos inicial edit.dat
		rewind(arqE);
		
		//passagem para playlists.dat
		while(!feof(arqE) && ftell(arqE) != tam)
		{
			fread(&temp, sizeof(temp), 1, arqE);
			fwrite(&temp, sizeof(temp), 1, arqP);
		}
		
		//salvar alterações
		fclose(arqP);
		arqP = fopen("playlists.dat", "rb+");
		
		//erro
		if(arqP == NULL)
		{
			printf("\tErro ao \"reinicializar\" <playlists.dat>.\n");
			return;
		}

		qtd--;
	}
	
	//novo registro
	displayPlaylists(arqP);
	printf("\n");
}

//adiciona músicas à playlist
void adicionarMusicasPlaylist(FILE* arqP, FILE* arqM, FILE* arqI)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	MUSICA adicionar, temp;
	long int tam;
	int qtd, id;
	FILE* arqR;
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayPlaylists(arqP);
	
	//tamanho de playlists.dat
	fseek(arqP, 0, SEEK_END);
	tam = ftell(arqP);
	
	//verificar se existe playlists para alterar
	if(tam == 0)
	{
		printf(BOLD "\tNão há playlists para alterar!\n\n" RESET);
		return;	
	}
	
	//alterar? qual playlist
	printf(BOLD "\n\tAlterar playlist (+)     : " RESET);
	scanf("%[^\n]", nomeP);
	
	//checar se playlist existe
	if(!existeNoRegistro(arqP, nomeP))
	{
		printf(BOLD "\tEssa playlist não existe!\n\n" RESET);
		return;
	}
	
	//path para alterar
	snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
	printf(BOLD "\tAcessando                : " RESET "%s\n", path);
	
	//alterar no path definido
	arqR = fopen(path, "rb+");
	
	//erro de acesso de registro
	if(arqR == NULL)
	{
		printf("\tErro ao acessar registro <%s.dat>.\n", nomeP);
		return;
	}
	
	//display playlist atual
	displayMusicasPlaylist(arqR, nomeP);
	
	//tamanho de arqR
	fseek(arqR, 0, SEEK_END);
	tam = ftell(arqR);
	
	//checa se há músicas ainda não incluidas
	fseek(arqM, 0, SEEK_END);
	if(tam == ftell(arqM))
	{
		printf(BOLD "\tInclui todas as músicas!\n" RESET);
		return;
	}

	//adicionar quantas
	printf(BOLD "\tDeseja adicionar quantas músicas? " RESET);
	scanf("%d", &qtd);
	
	//músicas disponíveis
	printf("\n");
	displayMusicas(arqM);
	
	//adicionar
	while(qtd > 0)
	{
		//novo tamanho de arqR
		fseek(arqR, 0, SEEK_END);
		tam = ftell(arqR);
		
		//checa se há músicas ainda não incluídas
		fseek(arqM, 0, SEEK_END);
		if(tam == ftell(arqM))
		{
			printf(BOLD "\n\tInclui todas as músicas do catálogo!\n" RESET);
			displayMusicasPlaylist(arqR, nomeP);
			return;
		}
	
		//id da música
		printf(BOLD "\tadicionar " RESET CYAN BOLD "(ID): " RESET);
		scanf("%d", &id);
		
		//checa se existe
		if(!existeNoCatalogo(arqM, id))
		{
			printf(BOLD "\n\tCódigo inválido!\n\n" RESET);
			continue;
		}
		
		//checa se já está incluída
		if(existeNoCatalogo(arqR, id))
		{
			printf(BOLD "\n\tJá faz parte da playlist!\n\n" RESET);
			continue;
		}
		
		//"extrai" música do catálogo
		extrairDoCatalogo(arqM, &adicionar, id);
		
		//pos inicial
		rewind(arqR);
		
		//calcular ordem de inclusão
		adicionar.inclusao = 0;
		while(!feof(arqR) && ftell(arqR) != tam)
		{
			fread(&temp, sizeof(MUSICA), 1, arqR);
			if(temp.inclusao > adicionar.inclusao)adicionar.inclusao = temp.inclusao;
		} adicionar.inclusao += 1;
		
		//salvar no registro
		fwrite(&adicionar, sizeof(MUSICA), 1, arqR);
		qtd--;
	}
	
	//playlist alterada
	displayMusicasPlaylist(arqR, nomeP);
	
	//fechar registro
	fclose(arqR);
}

//remove músicas da playlist
void removerMusicasPlaylist(FILE* arqM, FILE* arqP, FILE* arqE, FILE* arqI)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	long int tam;
	int qtd, id;
	FILE* arqR;
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayPlaylists(arqP);
	
	//tamanho de playlists.dat
	fseek(arqP, 0, SEEK_END);
	tam = ftell(arqP);
	
	//verificar se existe playlists para alterar
	if(tam == 0)
	{
		printf(BOLD "\tNão há playlists para alterar!\n\n" RESET);
		return;	
	}
	
	//alterar? qual playlist
	printf(BOLD "\n\tAlterar playlist (-)     : " RESET);
	scanf("%[^\n]", nomeP);
	
	//checar se playlist existe
	if(!existeNoRegistro(arqP, nomeP))
	{
		printf(BOLD "\tEssa playlist não existe!\n\n" RESET);
		return;
	}
	
	//path para alterar
	snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
	printf(BOLD "\tAcessando                : " RESET "%s\n", path);
	
	//alterar no path definido
	arqR = fopen(path, "rb+");
	
	//erro de acesso de registro
	if(arqR == NULL)
	{
		printf("\tErro ao acessar registro <%s.dat>.\n", nomeP);
		return;
	}
	
	//tamanho de arqR
	fseek(arqR, 0, SEEK_END);
	tam = ftell(arqR);
	
	//display músicas da playlist registrada
	displayMusicasPlaylist(arqR, nomeP);
	
	//checa se a playlist está vazia
	if(tam == 0)
	{
		printf(BOLD "\tNão há músicas para remover!\n\n" RESET);
		return;
	}
	
	//remover quantas
	printf(BOLD "\tDeseja remover quantas músicas? " RESET);
	scanf("%d", &qtd);
	
	//remoção
	while(qtd > 0)
	{
		//novo tamanho de arqR
		fseek(arqR, 0, SEEK_END);
		tam = ftell(arqR);
		
		//checa se a playlist está vazia
		if(tam == 0)
		{
			printf(BOLD "\tNão há músicas para remover!\n\n" RESET);
			return;
		}
	
		//id da música
		printf(BOLD "\tRemover " RESET GREEN BOLD "(ID): " RESET);
		scanf("%d", &id);
		
		//checa se existe no catálogo
		if(!existeNoCatalogo(arqM, id))
		{
			printf(BOLD "\n\tCódigo inválido!\n\n" RESET);
			continue;
		}
		
		//checar se existe na playlist
		if(!existeNoCatalogo(arqR, id))
		{
			printf(BOLD "\n\tNão está na playlist!\n\n" RESET);
			continue;
		}
		
		//usar edit.dat para remover música da playlist
		removerDaPlaylist(arqR, arqE, path, id);
		qtd--;
	}
		
	//display playlist final
	displayMusicasPlaylist(arqR, nomeP);
	
	//fechar registro
	fclose(arqR);
}



//ordenação de playlist
void ordenarPlaylist(FILE* arqP, FILE* arqI)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	PLAYLIST playlist;
	long int tamArq;
	int tam, modo;
	MUSICA temp;
	FILE* arqR;
	PONT pos;
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayPlaylists(arqP);
	
	//tamanho de arqP
	fseek(arqP, 0, SEEK_END);
	tamArq = ftell(arqP);
	
	//checar se há playlists para ordenar
	if(tamArq == 0)
	{
		printf(BOLD "\tNão há playlists para ordenar!\n\n" RESET);
		return;
	}
	
	//ordenar? qual playlist
	printf(BOLD "\n\tordenar playlist (*)     : " RESET);
	scanf("%[^\n]", nomeP);
	
	//checar se playlist existe
	if(!existeNoRegistro(arqP, nomeP))
	{
		printf(BOLD "\tEssa playlist não existe!\n\n" RESET);
		return;
	}
	
	//path para ordenar
	snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
	printf(BOLD "\tAcessando                : " RESET "%s\n", path);
	
	//ordenar no path definido
	arqR = fopen(path, "rb+");
	
	//erro de acesso de registro
	if(arqR == NULL)
	{
		printf("\tErro ao acessar registro <%s.dat>.\n", nomeP);
		return;
	}
	
	//tamanho de arqR
	fseek(arqR, 0, SEEK_END);
	tamArq = ftell(arqR);
	
	//display playlists selecionada
	displayMusicasPlaylist(arqR, nomeP);
	
	//checar se é grande o suficiente
	if(tamArq < sizeof(MUSICA)*2)
	{
		printf(BOLD "\tPlaylist não é grande o suficiente!\n\n" RESET);
		return;
	}
	
	//gerar struct PLAYLIST
	inicializarPlaylist(&playlist);
	passarParaStruct(arqR, &playlist);
	
	//calcular tamanho da playlist
	tam = tamanhoPlaylist(&playlist);
	
	//selecionar modo de ordenação
	printf(GREEN BOLD "\t[1]: " RESET BOLD "(Mais recentes)\n" RESET);
	printf(GREEN BOLD "\t[2]: " RESET BOLD "(Mais antigas)\n" RESET);
	printf(GREEN BOLD "\t[3]: " RESET BOLD "(Mais curtas)\n" RESET);
	printf(GREEN BOLD "\t[4]: " RESET BOLD "(Mais longas)\n" RESET);
	printf(GREEN BOLD "\t[5]: " RESET BOLD "Artista\n" RESET);
	printf(GREEN BOLD "\t[6]: " RESET BOLD "Música\n" RESET);
	printf(BOLD "\n\tSelecione o modo de ordenação: " RESET);
	scanf("%d", &modo);
	
	//ordenar por inclusão (mais recentes)
	if(modo == 1)
	{
		while(tam > 0)
		{
			//posição
			pos = playlist.inicio;
			
			//ordenar 1 elemento
			while(pos->prox != NULL)
			{
				//comparar
				if(pos->track.inclusao < pos->prox->track.inclusao)
				{
					temp = pos->track;
					pos->track = pos->prox->track;
					pos->prox->track = temp;
				} pos = pos->prox;
			} tam--;
		}
	}

	//ordenar por inclusão (mais antigas)
	if(modo == 2)
	{
		while(tam > 0)
		{
			//posição
			pos = playlist.inicio;
			
			//ordenar 1 elemento
			while(pos->prox != NULL)
			{
				//comparar
				if(pos->track.inclusao > pos->prox->track.inclusao)
				{
					temp = pos->track;
					pos->track = pos->prox->track;
					pos->prox->track = temp;
				} pos = pos->prox;
			} tam--;
		}
	}
	
	//ordenar por duração (mais curtas)
	if(modo == 3)
	{
		while(tam > 0)
		{
			//posição
			pos = playlist.inicio;
			
			//ordenar 1 elemento
			while(pos->prox != NULL)
			{
				//comparar
				if(pos->track.duracao > pos->prox->track.duracao)
				{
					temp = pos->track;
					pos->track = pos->prox->track;
					pos->prox->track = temp;
				} pos = pos->prox;
			} tam--;
		}
	}
	//ordenar por duração (mais longas)
	if(modo == 4)
	{
		while(tam > 0)
		{
			//posição
			pos = playlist.inicio;
			
			//ordenar 1 elemento
			while(pos->prox != NULL)
			{
				//comparar
				if(pos->track.duracao < pos->prox->track.duracao)
				{
					temp = pos->track;
					pos->track = pos->prox->track;
					pos->prox->track = temp;
				} pos = pos->prox;
			} tam--;
		}
	}
	
	//ordenar por ordem alfabética do nome do artista
    if (modo == 5) {
        while (tam > 0) {
            // posição
            pos = playlist.inicio;
            while (pos->prox != NULL) {
                // compara para ver quem é "maior" em ordem alfabética
                if (strcasecmp(pos->track.artista, pos->prox->track.artista) > 0) {
                    // ordena caso o primeiro esteja após o segundo em ordem alfabética
                    temp = pos->track;
                    pos->track = pos->prox->track;
                    pos->prox->track = temp;
                } else if (strcasecmp(pos->track.artista, pos->prox->track.artista) == 0) {
                  //se for do mesmo artista, vai por ordem alfabética do nome das músicas
                    if (strcasecmp(pos->track.nome, pos->prox->track.nome) > 0) {
                        temp = pos->track;
                        pos->track = pos->prox->track;
                        pos->prox->track = temp;
                    }
                }
                pos = pos->prox;
            }
            tam--;
        }
    }
	
	//ordenar por ordem alfabética do nome da música
    if (modo == 6) {
        while (tam > 0) {
            // posição
            pos = playlist.inicio;
            while (pos->prox != NULL) {
                // compara para ver quem é "maior" em ordem alfabética
                if (strcasecmp(pos->track.nome, pos->prox->track.nome) > 0) {
                    // ordena caso o primeiro esteja após o segundo em ordem alfabética
                    temp = pos->track;
                    pos->track = pos->prox->track;
                    pos->prox->track = temp;
                } else if (strcasecmp(pos->track.nome, pos->prox->track.nome) == 0) {
                  //se as musicas tiverem o mesmo nome, o nome do artista desempata elas
                    if (strcasecmp(pos->track.artista, pos->prox->track.artista) > 0) {
                        temp = pos->track;
                        pos->track = pos->prox->track;
                        pos->prox->track = temp;
                    }
                }
                pos = pos->prox;
            }
            tam--;
        }
    }
	//pos inicial
	rewind(arqR);
	pos = playlist.inicio;
	
	//repassar para arqR playlist ordenada
	while(pos != NULL)
	{
		fwrite(&pos->track, sizeof(MUSICA), 1, arqR);
		pos = pos->prox;
	}
	
	//display playlist ordenada
	displayMusicasPlaylist(arqR, nomeP);
	
	//reinicializar struct
	reinicializarPlaylist(&playlist);
	
	//fechar registro
	fclose(arqR);
}

//display extenso
void displayExtenso(FILE* arqP, FILE* arqI)
{
	//dados locais
	char path[PLAYLIST_MAX+20];
	char nomeP[PLAYLIST_MAX];
	long int tamArq;
	FILE* arqR;
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayPlaylists(arqP);
	
	//tamanho de arqP
	fseek(arqP, 0, SEEK_END);
	tamArq = ftell(arqP);
	
	//checar se há playlists para ordenar
	if(tamArq == 0)
	{
		printf(BOLD "\tNão há playlists para mostrar!\n\n" RESET);
		return;
	}
	
	//ordenar? qual playlist
	printf(BOLD "\n\tmostrar playlist (#)     : " RESET);
	scanf("%[^\n]", nomeP);
	
	//checar se playlist existe
	if(!existeNoRegistro(arqP, nomeP))
	{
		printf(BOLD "\tEssa playlist não existe!\n\n" RESET);
		return;
	}
	
	//path para ordenar
	snprintf(path, PLAYLIST_MAX+20, "./registro/%s.dat", nomeP);
	printf(BOLD "\tAcessando                : " RESET "%s\n", path);
	
	//ordenar no path definido
	arqR = fopen(path, "rb+");
	
	//erro de acesso de registro
	if(arqR == NULL)
	{
		printf("\tErro ao acessar registro <%s.dat>.\n", nomeP);
		return;
	}
	
	//display playlist selecionada
	displayMusicasPlaylistExtenso(arqR, nomeP);
}
















