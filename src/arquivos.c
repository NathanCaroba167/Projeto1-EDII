//
// Created by natha on 16/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../include/arquivos.h"

char* getNomeBase(char* nomeArq) {
    // Busca a última barra ('/') para ignorar o caminho do diretório
    char* barra = strrchr(nomeArq, '/');
    char* Nome = (barra != NULL) ? barra + 1 : nomeArq;

    // Busca o último ponto ('.') para encontrar a extensão
    char* ponto = strrchr(Nome, '.');
    int tamanhoBase;

    if (ponto == NULL) {
        // Não há extensão, o nome base é o nome inteiro
        tamanhoBase = strlen(Nome);
    }else {
        // O tamanho base é a distância entre o ínicio do Nome e o ponto
        tamanhoBase = ponto - Nome;
    }

    // Aloca memória para a string resultante
    char* nomeBase = (char*) malloc(tamanhoBase + 1);
    if (nomeBase == NULL) {
        printf("Erro ao alocar memória de nomeBase!\n");

        perror("Motivo do erro");
        exit(1);
    }

    // Copia a substring e adiciona o terminador nulo
    strncpy(nomeBase, Nome, tamanhoBase);
    nomeBase[tamanhoBase] = '\0';

    return nomeBase;
}