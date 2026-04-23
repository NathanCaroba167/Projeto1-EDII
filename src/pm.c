//
// Created by natha on 06/04/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/hashfile.h"
#include "../include/habitante.h"
#include "../include/pm.h"

#define TAMANHO_MAX_BUFFER 256
#define MAX_CAMINHO_PM 512

typedef struct {
    char path[MAX_CAMINHO_PM];
    int numeroHabitantes;
    int numeroMoradores;
}pmStruct;

Pm criarPm(Nome path) {
    pmStruct* p = calloc(1, sizeof(pmStruct));
    if (p == NULL) {
        exit(1);
    }
    strncpy(p->path, path, MAX_CAMINHO_PM - 1);
    p->path[MAX_CAMINHO_PM - 1] = '\0';

    p->numeroHabitantes = -1;
    p->numeroMoradores = -1;
    return p;
}

int criarHabitantesEMoradores(Arquivo p, HashFile hfHabitantes) {
    if (p == NULL || hfHabitantes == NULL) {
        printf("ERRO: Argumentos NULL em criarQuadrasNoHash.\n");
        return -1;
    }

    pmStruct* pm = (pmStruct*)p;

    FILE* arquivoPm = fopen(pm->path, "r");
    if (arquivoPm == NULL) {
        return -1;
    }

    char buffer[TAMANHO_MAX_BUFFER];
    char bufferRegistro[1024];
    int inseridos = 0;
    int moradores = 0;

    while(fgets(buffer,sizeof(buffer),arquivoPm) != NULL) {
        // Remove quebras de linha e trata linhas vazias
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            continue;
        }

        char* comando = strtok(buffer," ");
        if (comando == NULL) {
            continue;
        }

        if (strcmp(comando, "p") == 0) {
            char* cpf = strtok(NULL," ");
            char* nome = strtok(NULL," ");
            char* sobrenome = strtok(NULL," ");
            char* sexo_temp = strtok(NULL," ");
            char* nascimento = strtok(NULL," ");

            if (cpf && nome && sobrenome && sexo_temp && nascimento) {
                char sexo = sexo_temp[0];

                Habitante hab = criarHabitante(cpf, nome, sobrenome, sexo, nascimento);
                inserirHashFile(hfHabitantes, hab);
            }
        }else if (strcmp(comando, "m") == 0) {
            char* cpf = strtok(NULL," ");
            char* cep = strtok(NULL," ");
            char* face_temp = strtok(NULL," ");
            char* num_temp = strtok(NULL," ");
            char* complemento = strtok(NULL," ");

            if (cpf && cep && face_temp && num_temp && complemento) {

                /* Avança espaços iniciais do complemento*/
                while (*complemento == ' ') {
                    complemento++;
                }

                char face = face_temp[0];
                int num = atoi(num_temp);

                buscarHashFile(hfHabitantes, cpf, bufferRegistro);
                atualizarHashFile(hfHabitantes, bufferRegistro);
                moradores++;

                eliminarHabitante(hab);
            }
        }else {
            printf("Comando desconhecido: '%s'\n", comando);
        }
    }
    fclose(arquivoPm);
    pm->numeroHabitantes = inseridos;
    pm->numeroMoradores = moradores;
    return inseridos;
}

void eliminarPm(Pm pm) {
    if (pm == NULL) {
        return;
    }
    pmStruct* p = (pmStruct*)pm;
    free(p);
}
