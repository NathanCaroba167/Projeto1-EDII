//
// Created by natha on 21/03/2026.
//

#ifndef HASHFILE_H
#define HASHFILE_H

#include <stdbool.h>

/*
 Módulo encarregado de executar e coordenar um hash file extensível com operações de criar, inserir, buscar e liberar
 */

/*
 Ponteiro void Hash
 */
typedef struct Diretorio Diretorio;
typedef struct Conteudo Conteudo;


/// @brief Cria hash extensível, iniciando o diretorio com profundidade global 0 e 1 bucket vazio.
/// @return Ponteiro para o diretorio alocado.
Diretorio* criarHash();

/// @brief Insere um novo registro no sistema de hash.
/// @param dir Ponteiro para o diretorio
/// @param cont Ponteiro para o conteudo a ser inserido.
/// @return
void inserirHash(Diretorio* dir, Conteudo* cont);

/// @brief Busca um registro pela chave (codigo).
/// @param dir Ponteiro para o diretorio.
/// @param codigo Chave a ser buscada.
/// @param cont_clone Ponteiro onde o registro encontrado será copiado.
/// @return True se encontrou e False caso contrario.
bool getHash(Diretorio* dir, int codigo, Conteudo** cont_clone);

/// @brief Libera toda a memória alocada (diretorio e buckets).
/// @return
void liberarHash(Diretorio* dir);

#endif //HASHFILE_H