//
// Created by natha on 13/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/arquivos.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include "../include/habitante.h"
#include "../include/svg.h"
#include "../include/txt.h"
#include "../include/geo.h"
#include "../include/pm.h"
#include "../include/qry.h"

#define MAX_CAMINHO 128
#define MAX_CAMINHO1 256
#define MAX_CAMINHO2 512

#define CAPACIDADE_BUCKET_QUADRAS 4
#define CAPACIDADE_BUCKET_HABITANTES 4

int main(int argc, char* argv[]) {
    // Variáveis de Argumentos
    char* dirEntrada = NULL; // -e
    char* nomeArqGeo = NULL; // -f
    char* nomeArqQry = NULL; // -q
    char* dirSaida = NULL; // -o
    char* nomeArqPm = NULL; // -pm

    // Análise de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            dirEntrada = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            nomeArqGeo = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            dirSaida = argv[i + 1];
            i++;
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            nomeArqQry = argv[i + 1];
            i++;
        }else if (strcmp(argv[i], "-pm") == 0 && i + 1 < argc) {
            nomeArqPm = argv[i + 1];
            i++;
        }else {
            printf("Parametro '%s' inválido!!\n", argv[i]);
        }
    }

    if (nomeArqGeo == NULL || dirSaida == NULL) {
        printf("ERRO: Parâmetros obrigatórios -f (.geo) e -o (diretorio saida1) não fornecidos!!\n");
        return 1;
    }

    // Construção de caminhos geo (necessário para SVG inicial)
    char caminhoCompletoGeo[MAX_CAMINHO];
    if (dirEntrada != NULL) {
        snprintf(caminhoCompletoGeo,MAX_CAMINHO, "%s/%s", dirEntrada, nomeArqGeo);
    } else {
        snprintf(caminhoCompletoGeo,MAX_CAMINHO, "%s", nomeArqGeo);
    }

    char caminhoCompletoPm[MAX_CAMINHO];
    if (nomeArqPm != NULL) {
        if (dirEntrada != NULL) {
            snprintf(caminhoCompletoPm,MAX_CAMINHO, "%s/%s", dirEntrada, nomeArqPm);
        } else {
            strcpy(caminhoCompletoPm,nomeArqPm);
        }
    }

    char* nomeBaseGeo = getNomeBase(nomeArqGeo);
    if (nomeBaseGeo ==  NULL) {
        printf("Erro: nome base do arquivo geo.\n");
        return 1;
    }

    char baseHFQuadras[MAX_CAMINHO1];
    char baseHFHabitantes[MAX_CAMINHO1];
    snprintf(baseHFQuadras, MAX_CAMINHO1, "%s/quadras", dirSaida);
    snprintf(baseHFHabitantes, MAX_CAMINHO1, "%s/habitantes", dirSaida);


    char caminhoHFDQuadras[MAX_CAMINHO2];
    char caminhoHFDHabitantes[MAX_CAMINHO2];
    snprintf(caminhoHFDQuadras, MAX_CAMINHO2, "%s/quadras.hfd", dirSaida);
    snprintf(caminhoHFDHabitantes, MAX_CAMINHO2, "%s/habitantes.hfd", dirSaida);

    char caminhoSaidaSvgInicial[MAX_CAMINHO];
    snprintf(caminhoSaidaSvgInicial,MAX_CAMINHO, "%s/%s.svg", dirSaida, nomeBaseGeo);

    // Construção de caminhos qry (se existir)
    char caminhoCompletoQry[MAX_CAMINHO];
    char caminhoSaidaSvgFinal[MAX_CAMINHO2];
    char caminhoSaidaTxt[MAX_CAMINHO2];
    char caminhoBaseSaida[MAX_CAMINHO1];
    if (nomeArqQry != NULL) {
        if (dirEntrada != NULL) {
            snprintf(caminhoCompletoQry,MAX_CAMINHO, "%s/%s", dirEntrada, nomeArqQry);
        }else {
            strcpy(caminhoCompletoQry, nomeArqQry);
        }

        printf("DEBUG: Tentando abrir o arquivo QRY no caminho: [%s]\n", caminhoCompletoQry);


        char* nomeBaseQry = getNomeBase(nomeArqQry);
        if (nomeBaseQry ==  NULL) {
            free(nomeBaseGeo);
            return 1;
        }
        char nomeCombinado[MAX_CAMINHO];
        snprintf(nomeCombinado,MAX_CAMINHO, "%s-%s", nomeBaseGeo, nomeBaseQry);

        snprintf(caminhoBaseSaida,MAX_CAMINHO1, "%s/%s", dirSaida, nomeCombinado);

        snprintf(caminhoSaidaSvgFinal,MAX_CAMINHO2, "%s.svg", caminhoBaseSaida);
        snprintf(caminhoSaidaTxt,MAX_CAMINHO2, "%s.txt", caminhoBaseSaida);

        free(nomeBaseQry);
    }

    free(nomeBaseGeo);

    HashFileConfig configQuadras = criarHashFileConfigQuadras(CAPACIDADE_BUCKET_QUADRAS);
    HashFileConfig configHabitantes = criarHashFileConfigHabitantes(CAPACIDADE_BUCKET_HABITANTES);

    if (configQuadras == NULL || configHabitantes == NULL) {
        printf("ERRO: ao criar configurações dos hashfiles.\n");
        eliminarHashFileConfig(configQuadras);
        eliminarHashFileConfig(configHabitantes);
        return 1;
    }

    HashFile hfQuadras = criarHashFile(baseHFQuadras, configQuadras);
    HashFile hfHabitantes = criarHashFile(baseHFHabitantes, configHabitantes);

    if (hfQuadras == NULL || hfHabitantes == NULL) {
        printf("ERRO: ao criar hashfiles.\n");
        fecharHashFile(hfQuadras);
        fecharHashFile(hfHabitantes);
        eliminarHashFileConfig(configQuadras);
        eliminarHashFileConfig(configHabitantes);
        return 1;
    }

    // Geração do SVG Inicial
    Arquivo arqSvgInicial = NULL;
    arqSvgInicial = abrirSVG(caminhoSaidaSvgInicial);
    inicializarSVG(arqSvgInicial);

    // Execução do fluxo .geo
    Geo arqGeo = criarGeo(caminhoCompletoGeo);
    if (arqGeo == NULL) {
        printf("ERRO: ao tentar abrir o arquivo .geo");
        return 1;
    }

    criarQuadrasNoHash(arqGeo, hfQuadras, arqSvgInicial);
    fecharSVG(arqSvgInicial);
    eliminarGeo(arqGeo);

    // Execução do fluxo .pm
    if (nomeArqPm != NULL) {
        Pm pm = criarPm(caminhoCompletoPm);
        criarHabitantesEMoradores(pm,hfHabitantes);
        eliminarPm(pm);
    }

    // Execução do fluxo .qry (se especificado)
    if (nomeArqQry != NULL) {
        Arquivo arqQry = NULL;
        arqQry = abrirQry(caminhoCompletoQry);
        Arquivo arqTxt = NULL;
        arqTxt = abrirTXT(caminhoSaidaTxt);
        Arquivo arqSvgFinal = NULL;
        arqSvgFinal = abrirSVG(caminhoSaidaSvgFinal);
        inicializarSVG(arqSvgFinal);
        if (arqQry == NULL) {
            printf("ERRO: ao tentar abrir o arquivo .qry");
            fecharHashFile(hfQuadras);
            fecharHashFile(hfHabitantes);
            eliminarHashFileConfig(configQuadras);
            eliminarHashFileConfig(configHabitantes);
            return 1;
        }

        LerComandosExecutar(arqSvgFinal, arqTxt, arqQry, hfQuadras, hfHabitantes);

        fclose(arqQry);
        fclose(arqTxt);

        fecharSVG(arqSvgFinal);
        fclose(arqSvgFinal);
    }

    dumpHashFile(hfQuadras, caminhoHFDQuadras);
    dumpHashFile(hfHabitantes, caminhoHFDHabitantes);

    // Limpeza da memória
    printf("Limpando a memória ...\n");
    fecharHashFile(hfQuadras);
    fecharHashFile(hfHabitantes);
    eliminarHashFileConfig(configQuadras);
    eliminarHashFileConfig(configHabitantes);

    return 0;
}
