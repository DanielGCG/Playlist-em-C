void atualizarPlaylists(FILE* arqP, FILE* arqE, int ID);

//gera ID para  música
void gerarID(MUSICA* reg)
{
	//iniciar id
	reg->id = 0;	
	
	//nome
	for(long int i = 0, f = 0; i != strlen(reg->nome); i++, f+=1000)
	reg->id += ((int)reg->nome[i]) * f;		
	
	//artista
	for(int i = 0, f = 0; i != strlen(reg->artista); i++, f+=100)
	reg->id += ((int)reg->artista[i]) * f;
	
	//album
	for(int i = 0, f = 0; i != strlen(reg->album); i++, f+=10)
	reg->id += ((int)reg->album[i]) * f;
	
	//duração
	reg->id += reg->duracao;
}

//verifica se música já existe no catálogo
int existeNoCatalogo(FILE* arqM, int ID)
{
	//dados locais
	long int tam;
	MUSICA temp;

	//tamanho do arquivo
	fseek(arqM, 0, SEEK_END);
	tam = ftell(arqM);

	//pos inicial
	rewind(arqM);
	
	//checagem
	while(!feof(arqM) && ftell(arqM) != tam)
	{
		fread(&temp, sizeof(MUSICA), 1, arqM);
		if(temp.id == ID)return 1;
	}
	
	//não está no catálogo
	return 0;
}

//recebe informações de música adicionada
int inputMusica(MUSICA* input)
{
	//dados locais
	int min, seg;
	
	//input nome
	printf(CYAN BOLD "\tNome         : " RESET);
	scanf("%[^\n]", input->nome);
	
	//checar tamanho de input->nome
	if(strlen(input->nome) > NOME_MAX)
	{
		printf(BOLD "\n\tO nome não pode exceder" CYAN " %d" RESET BOLD "caracteres!\n\n" RESET, NOME_MAX);
		fseek(stdin, 0, SEEK_END);
		getc(stdin);
		return 0;
	}
	
	//input artista
	printf(CYAN BOLD "\tArtista      : " RESET);
	getc(stdin);
	scanf("%[^\n]", input->artista);

	//checar tamanho de input->artista
	if(strlen(input->artista) > ARTISTA_MAX)
	{
		printf(BOLD "\n\tO nome do artista não pode exceder" CYAN " %d" RESET BOLD "caracteres!\n\n" RESET, ARTISTA_MAX);
		fseek(stdin, 0, SEEK_END);
		getc(stdin);
		return 0;
	}

	//input album
	printf(CYAN BOLD "\tAlbum        : " RESET);
	getc(stdin);
	scanf("%[^\n]", input->album);
	
	//checar tamanho de input->album
	if(strlen(input->album) > ALBUM_MAX)
	{
		printf(BOLD "\n\tO nome do album não pode exceder" CYAN " %d" RESET BOLD "caracteres!\n\n" RESET, ALBUM_MAX);
		fseek(stdin, 0, SEEK_END);
		getc(stdin);
		return 0;
	}
	
	//dados de duração
	printf(CYAN BOLD "\tDuração (m)  : " RESET);
	scanf("%d", &min);
	printf(CYAN BOLD "\tDuração (s)  : " RESET);
	scanf("%d", &seg);
	
	//conversão para segundos
	input->duracao = (min * 60) + seg;
	
	//gerar id (fim)
	gerarID(input);
	getc(stdin);
	return 1;
}

//adiciona músicas ao catálogo
void adicionarAoCatalogo(FILE* arqM, FILE* arqI)
{
	//dados locais
	int qtd;
	MUSICA temp;
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayMusicas(arqM);
	
	//adicionar quantas
	printf(BOLD "\tDeseja adicionar quantas músicas? " RESET);
	scanf("%d", &qtd);
	printf("\n");
	getc(stdin);
	
	//cada música
	while(qtd > 0)
	{
		//ultrapassa? limites de caracteres
		if(!inputMusica(&temp))continue;

		//existe? no catalogo
		if(existeNoCatalogo(arqM, temp.id))
		{
			printf(BOLD "\n\tJá faz parte do catálogo!\n\n" RESET);
			continue;
		}
		
		//salvar música
		printf(BOLD "\n\tConcluído!\n\tID: " RESET "%ld\n\n", temp.id);
		fwrite(&temp, sizeof(MUSICA), 1, arqM);
		qtd--;
	}
	//novo catálogo
	displayMusicas(arqM);
}

//remove músicas do catálogo
void removerDoCatalogo(FILE* arqP, FILE* arqM, FILE* arqE, FILE* arqI)
{
	//dados locais
	int qtd, id;
	MUSICA temp;
	long int tam;
	
	//tamanho de arqM 
	fseek(arqM, 0, SEEK_END);
	tam = ftell(arqM);
	
	//interface
	system("reset");
	displayLogo(arqI);
	displayMusicas(arqM);
	
	//parar se vazia
	if(tam == 0)
	{
		printf(BOLD "\tNão há músicas para remover!\n\n" RESET);
		return;
	}
	
	//remover quantas
	printf(BOLD "\tDeseja remover quantas músicas? " RESET);
	scanf("%d", &qtd);
	printf("\n");
	
	//remoção
	while(qtd > 0)
	{
		//checa se está vazia
		if(tam == 0)
		{
			printf(BOLD "\tNão há músicas para remover!\n\n" RESET);
			break;
		}
		
		//reinicializar edit.dat
		fclose(arqE);
		arqE = fopen("edit.dat", "wb+");
		
		//erro
		if(arqE == NULL)
		{
			printf("\tErro na criação de <edit.dat>.\n");
			return;
		}
		
		//id da música
		printf(BOLD "\tremover " RESET CYAN BOLD "(ID): " RESET);
		scanf("%d", &id);
		
		//checa se existe
		if(!existeNoCatalogo(arqM, id))
		{
			printf(BOLD "\n\tcódigo inválido!\n\n" RESET);
			continue;
		}
		
		//pos inicial
		rewind(arqM);
		
		//passagem para edit.dat
		while(!feof(arqM) && ftell(arqM) != tam)
		{
			fread(&temp, sizeof(MUSICA), 1, arqM);
			if(temp.id == id)continue;
			fwrite(&temp, sizeof(MUSICA), 1, arqE);
		}
		
		//"reinicializar" musicas.dat
		fclose(arqM);
		arqM = fopen("musicas.dat", "wb");
		
		//erro
		if(arqM == NULL)
		{
			printf("\tErro ao \"reinicializar\" <musicas.dat>.\n");
			return;
		}

		//tamanho de edit.dat
		fseek(arqE, 0, SEEK_END);
		tam = ftell(arqE);
	
		//pos inicial edit.dat
		rewind(arqE);
		
		//passagem para musicas.dat
		while(!feof(arqE) && ftell(arqE) != tam)
		{
			fread(&temp, sizeof(MUSICA), 1, arqE);
			fwrite(&temp, sizeof(MUSICA), 1, arqM);
		} qtd--;
		
		//salvar alterações
		fclose(arqM);
		arqM = fopen("musicas.dat", "rb+");
		
		//erro
		if(arqM == NULL)
		{
			printf("\tErro ao \"reinicializar\" <musicas.dat>.\n");
			return;
		}
		
		//atualizar playlists
		atualizarPlaylists(arqP, arqE, id);
	}
	
	//novo catálogo
	printf("\n");
	displayMusicas(arqM);
}

//extrai musica selecionada
void extrairDoCatalogo(FILE* arqM, MUSICA* track, int ID)
{
	//dados locais
	fseek(arqM, 0, SEEK_END);
	long int tam = ftell(arqM);
	
	//pos inicial
	rewind(arqM);
	
	//procura e envio
	while(!feof(arqM) && ftell(arqM) != tam)
	{
		fread(track, sizeof(MUSICA), 1, arqM);
		if(track->id == ID)return;
	}
}

