//
// Created by natha on 09/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/geo.h"

#define TAMANHO_MAX_BUFFER 256
#define COR_MAX 32
#define MAX_CAMINHO_GEO 512

typedef struct{
    double sw;
    char corP[COR_MAX];
    char corB[COR_MAX];
}estilo; // Corresponde ao Estilo da Quadra

estilo* criarEstiloQuadra(double sw, char* corP, char* corB) {
    estilo* e = (estilo*)malloc(sizeof(estilo));
    if (e == NULL) {
        printf("Erro ao alocar memória ao criarEstilo!\n");

        perror("Motivo do erro");
        exit(1);
    }

    e->sw = sw;

    strncpy(e->corP,corP,COR_MAX - 1);
    e->corP[COR_MAX - 1] = '\0';
    strncpy(e->corB,corB,COR_MAX - 1);
    e->corB[COR_MAX - 1] = '\0';

    return e;
}

void eliminarEstiloQuadra(estilo* e) {
    free(e);
}

typedef struct {
    char path[MAX_CAMINHO_GEO];
    estilo* estilo;
    int numeroQuadras;
}geoStruct;

Geo criarGeo(Nome path) {
    geoStruct* g = calloc(1, sizeof(geoStruct));
    if (g == NULL) {
        exit(1);
    }
    strncpy(g->path, path, MAX_CAMINHO_GEO - 1);
    g->path[MAX_CAMINHO_GEO - 1] = '\0';

    g->estilo = criarEstiloQuadra(1.0, "white", "black");

    g->numeroQuadras = -1;
    return g;
}

int criarQuadrasNoHash(Geo g, HashFile hfQuadras, Arquivo svg) {
    if (g == NULL || hfQuadras == NULL) {
        printf("ERRO: Argumentos NULL em criarQuadrasNoHash.\n");
        return -1;
    }

    geoStruct* geo = (geoStruct*)g;

    FILE* arquivoGeo = fopen(geo->path, "r");
    if (arquivoGeo == NULL) {
        return -1;
    }

    char buffer[TAMANHO_MAX_BUFFER];
    char bufferRegistro[1024];
    int inseridas = 0;

    while(fgets(buffer,sizeof(buffer),arquivoGeo) != NULL) {
        // Remove quebras de linha e trata linhas vazias
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            continue;
        }

        char* comando = strtok(buffer," ");
        if (comando == NULL) {
            continue;
        }

        if (strcmp(comando, "cq") == 0) {
            char* sw_temp = strtok(NULL," ");
            char* corP = strtok(NULL," ");
            char* corB = strtok(NULL," ");

            if (sw_temp && corP && corB) {
                double sw = atof(sw_temp);

                if (geo->estilo != NULL) {
                    eliminarEstiloQuadra(geo->estilo);
                }

                geo->estilo = criarEstiloQuadra(sw, corP, corB);
            }
        }else if (strcmp(comando, "q") == 0) {
            char* cep  = strtok(NULL," ");
            char* x_temp = strtok(NULL," ");
            char* y_temp = strtok(NULL," ");
            char* w_temp = strtok(NULL," ");
            char* h_temp = strtok(NULL," ");

            if (cep && x_temp && y_temp && w_temp && h_temp) {
                double x = atof(x_temp);
                double y = atof(y_temp);
                double w = atof(w_temp);
                double h = atof(h_temp);

                Quadra q = criarQuadra(cep, x, y, w, h);
                if (geo->estilo != NULL) {
                    setCorPQuadra(q, geo->estilo->corP);
                    setCorBQuadra(q, geo->estilo->corB);
                    setSWQuadra(q, geo->estilo->sw);
                }
                if (inserirHashFile(hfQuadras,q) == 0) {
                    inseridas++;

                    if (svg != NULL) {
                        desenharQuadraSVG(svg, q);
                    }
                }

                eliminarQuadra(q);
            }
        }else {
            printf("Comando desconhecido: '%s'\n", comando);
        }
    }
    fclose(arquivoGeo);
    geo->numeroQuadras = inseridas;
    return inseridas;
}

void eliminarGeo(Geo geo) {
    if (geo == NULL) {
        return;
    }
    geoStruct* g = (geoStruct*)geo;
    eliminarEstiloQuadra(g->estilo);
    free(g);
}