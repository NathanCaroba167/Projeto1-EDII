//
// Created by natha on 21/03/2026.
//

#ifndef HASHFILE_H
#define HASHFILE_H

#include <stdbool.h>

/*
 Módulo encarregado de executar e coordenar um hash file extensível com operações de criar, inserir, buscar, remover e liberar
 */

/*
 Ponteiro structs opaca.
 */
typedef struct Diretorio Diretorio;
typedef struct Conteudo Conteudo;

/// @brief Cria hash extensível, iniciando o diretorio com profundidade global 0 e 1 bucket vazio.
/// @return Ponteiro para o diretorio alocado.
/// @warning
Diretorio* criarHash();

/// @brief Insere um novo registro no sistema de hash.
/// @param dir Ponteiro para o diretorio.
/// @param cont Ponteiro para o conteúdo a ser inserido.
/// @warning
void inserirHash(Diretorio* dir, Conteudo* cont);

/// @brief Remove o registro do sistema de hash.
/// @param dir Ponteiro para o diretorio.
/// @param cont Ponteiro para o conteúdo a ser removido.
/// @warning
void removerHash(Diretorio* dir, Conteudo* cont);

/// @brief Busca um registro pela chave (codigo).
/// @param dir Ponteiro para o diretorio.
/// @param codigo Chave a ser buscada.
/// @param cont_clone Ponteiro onde o registro encontrado será copiado.
/// @return True se encontrou e False caso contrario.
/// @warning
bool getHash(Diretorio* dir, int codigo, Conteudo** cont_clone);

/// @brief Cria o conteúdo com os atributos indicados.
/// @param codigo Identificador do conteúdo criado.
/// @return Ponteiro para o conteúdo criado.
/// @warning
Conteudo* criarConteudo(int codigo);

/// @brief Pega a profundidade global do diretório sem modifica-lo.
/// @param dir Ponteiro para o diretório analisado.
/// @return Um inteiro que representa a profundidade global do diretório.
/// @warning
int getProfundidadeGlobal(Diretorio* dir);

/// @brief Pega o tamanho do diretório sem modifica-lo.
/// @param dir Ponteiro para o diretório analisado.
/// @return Um inteiro que representa o tamanho do diretório.
/// @warning
int getTamanhoDiretorio(Diretorio* dir);

/// @brief Pega o contador do bucket sem modifica-lo.
/// @param dir Ponteiro para o diretório analisado.
/// @param indice Indice do bucket onde o contador será pego.
/// @return Um inteiro que representa o contador do bucket.
/// @warning
int getContadorBucket(Diretorio* dir, int indice);

/// @brief Pega o código do conteúdo sem modifica-lo.
/// @param cont Ponteiro para o conteúdo analisado.
/// @return Um inteiro que representa o código do conteúdo.
/// @warning
int getCodigoConteudo(Conteudo* cont);

/// @brief Libera toda a memória alocada (diretorio e buckets).
/// @warning
void liberarHash(Diretorio* dir);

#endif //HASHFILE_H