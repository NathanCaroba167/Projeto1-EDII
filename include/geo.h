//
// Created by natha on 09/10/2025.
//

#ifndef GEO_H
#define GEO_H

#include "hashfile.h"

/*
 Módulo encarregado de ler um arquivo (.geo) e inicializar os objetos com as operações de abrir e criar
 */

/*
 Ponteiro do tipo FILE Arquivo
 */
typedef FILE* Arquivo;
typedef char* Nome;

/// @rief Abre o arquivo .geo para leitura.
/// @param arquivo Ponteiro para o nome do arquivo que será aberto.
/// @return Ponteiro para o arquivo que foi aberto.
/// @warning 
Arquivo abrirGeo(Nome arquivo);

/// @brief Cria quadras e estilo lidos do (.geo) e coloca as quadras no hashfile.
/// @param geo Ponteiro para o arquivo (.geo) que será lido.
/// @param hfQuadras Ponteiro para o Hashfile que receberá as quadras.
/// @param EstiloPonteiro Ponteiro para o Estilo global de quadras (será alocado se o comando 'cq' for encontrado).
/// @param svg Arquivo svg onde será desenhado as quadras
/// @warning
void criarQuadrasNoHash(Arquivo geo,HashFile hfQuadras,Estilo* EstiloPonteiro, Arquivo svg);

#endif //GEO_H