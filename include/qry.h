//
// Created by natha on 09/10/2025.
//

#ifndef QRY_H
#define QRY_H

#include "hashfile.h"

/*
 Módulo encarregado de ler um arquivo (.qry) e executar os comandos com as operações de abrir e ler
 */

/*
 Ponteiro do tipo FILE Arquivo
 Ponteiro char Nome
 */
typedef FILE* Arquivo;
typedef char* Nome;

/// @brief Abre o arquivo .qry para leitura.
/// @param arquivo Ponteiro para o nome do arquivo que será aberto.
/// @return Ponteiro para o arquivo que será aberto.
Arquivo abrirQry(Nome arquivo);

/// @brief Lê comandos do (.qry) e executa os comandos, registrando resultados.
/// @param svg Ponteiro para o arquivo SVG (para visualização dinâmica).
/// @param txt Ponteiro para o arquivo TXT (para reportar resultados).
/// @param qry Ponteiro para o arquivo .qry que será lido.
/// @param hfQuadras Ponteiro para o hashfile de quadras.
/// @param hfHabitantes Ponteiro para o hashfile de habitantes.
/// @warning
void LerComandosExecutar(Arquivo svg,Arquivo txt,Arquivo qry,HashFile hfQuadras,HashFile hfHabitantes);

#endif //QRY_H