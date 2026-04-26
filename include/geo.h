//
// Created by natha on 09/10/2025.
//

#ifndef GEO_H
#define GEO_H

#include "hashfile.h"
#include "svg.h"

/*
 Módulo encarregado de ler um arquivo (.geo) e inicializar os objetos com as operações de abrir e criar
 */

/*
 Ponteiro do tipo FILE Arquivo
 */
typedef FILE* Arquivo;
typedef char* Nome;
typedef void* Geo;

/// @rief Abre o arquivo .geo para leitura.
/// @param path Caminho para o arquivo (.geo) que será lido.
/// @return Ponteiro para o arquivo que foi aberto.
/// @warning 
Geo criarGeo(Nome path);

/// @brief Cria quadras e estilo lidos do (.geo) e coloca as quadras no hashfile.
/// @param g Ponteiro para o arquivo (.geo) que será lido.
/// @param hfQuadras Ponteiro para o Hashfile que receberá as quadras.
/// @param svg Arquivo svg onde será desenhado as quadras
/// @return Número de quadras inseridas.
/// @warning
int criarQuadrasNoHash(Geo g, HashFile hfQuadras, Arquivo svg);

/// @brief Libera a memória alocada para o geo.
/// @param geo Ponteiro para o geo que será eliminado.
/// @warning
void eliminarGeo(Geo geo);

#endif //GEO_H