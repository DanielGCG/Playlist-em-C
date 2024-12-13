void credits() {
    system("reset");
    FILE *arq;
    char caracter;

    arq = fopen("credits.txt", "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    while ((caracter = fgetc(arq)) != EOF) {
        putchar(caracter);
    }

    fclose(arq);
}

