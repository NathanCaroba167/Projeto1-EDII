//
// Created by natha on 21/03/2026.
//

#ifndef HASHFILE_H
#define HASHFILE_H

#include <stddef.h>

/*
 Módulo encarregado de gerenciar um Hashfile Dinâmino (Hashing Extensível) em disco(arquivo binário),
 com operações de criar, abrir, fechar, inserir, buscar, remover, atualizar e dump.
 O módulo é genérico: opera sobre registros de tamanho fixo sem conhecer o tipo concreto.
 A localização da chave dentro do registro é configurada via HashFileConfig (offsetof/sizeof)

/* Códigos de retorno das operações*/
#define HF_OK 0

#define HF_ERROR -1


/*
 Ponteiro structs opaco.
 */
typedef void* HashFileConfig;
typedef void* HashFile;

typedef char* Nome;

/// @brief Cria uma configuração para o uso do HashFile
/// @param record_size Tamanho total de um registro em bytes
/// @param key_offset Deslocamento em bytes da chave dentro do registro
/// @param key_size Tamanho da chave em bytes
/// @param dir_offset Deslocamento em bytes do início do diretorio dentro do arquivo .hfc
/// @param bucket_capacity Número máximo de registros por bucket
/// @return Ponteiro para a configuração criada
/// @warning
HashFileConfig criarHashFileConfig(size_t record_size,size_t key_offset,size_t key_size,size_t dir_offset,int bucket_capacity);

/// @brief Libera a memória alocada de uma configuração
/// @param config Ponteiro para a configuração a ser eliminada
/// @warning
void eliminarHashFileConfig(HashFileConfig config);

/// @brief Cria um novo HashFile Extensível em disco separando o hash em dois arquivos, arquivo de cabeçalho e arquivo de buckets
/// @param arquivoPath Caminho do arquivo hashfile que será criado
/// @param config Tamanhos para busca no hashfile
/// @return Ponteiro para o hashfile criado
/// @warning
HashFile criarHashFile(Nome arquivoPath, HashFileConfig config);

/// @brief Abre um HashFile existente em disco para leitura e escrita
/// @param arquivoPath Caminho do arquivo hashfile que será aberto
/// @param config Configuração compatível com o arquivo hashfile
/// @return Ponteiro para o hashfile aberto
/// @warning
HashFile abrirHashFile(Nome arquivoPath, HashFileConfig config);

/// @brief Fecha o hashfile e libera todos os recursos associados
/// @param hashFile HashFile que será fechado
/// @warning 
void fecharHashFile(HashFile hashFile);

/// @brief Insere um novo registro no sistema de hash.
/// @param hf Ponteiro do hashfile analisado.
/// @param registro Ponteiro para o registro a ser inserido.
/// @warning
void inserirHashFile(HashFile hf, void* registro);

/// @brief Pega um registro pela chave (codigo) no hashfile.
/// @param hf Ponteiro para o hashfile analisado.
/// @param chave Ponteiro para a chave a ser buscada.
/// @param registro_clone Ponteiro onde o registro encontrado será copiado.
/// @return
/// @warning
int getHashFile(HashFile hf, void* chave, void* registro_clone);

/// @brief Remove um registro do sistema de hash conforme a chave.
/// @param hf Ponteiro para o hashfile analisado.
/// @param chave Ponteiro para a chave do registro a remover.
/// @param registro_clone Ponteiro onde o registro removido será copiado
/// @return
/// @warning
int removerHashFile(HashFile hf, void* chave, void* registro_clone);

/// @brief Atualiza um registro existente no hashfile
/// @param hf Ponteiro para o hashfile analisado
/// @param registro_novo Ponteiro para o novo registro
/// @return
/// @warning
int atualizarHashFile(HashFile hf, void* registro_novo);

/// @brief Gera o arquivo de dump (.hfd)
/// @param hf Ponteiro para o hashfile analisado
/// @param arquivoPath Caminho do arquivo dump a ser gerado
/// @return
/// @warning
int dumpHashFile(HashFile hf, Nome arquivoPath);

/// @brief Pega a profundidade global do diretório sem modifica-lo.
/// @param hf Ponteiro para o hashfile analisado.
/// @return Um inteiro que representa a profundidade global do diretório.
/// @warning
int getProfundidadeGlobalHF(HashFile hf);

/// @brief Pega o número total de buckets do hashfile sem modifica-lo.
/// @param hf Ponteiro para o hashfile analisado.
/// @return Um inteiro que representa o número de buckets do hashfile.
/// @warning
int getNumeroBucketsHF(HashFile hf);

/// @brief Pega o número total de registros do hashfile sem modifica-lo.
/// @param hf Ponteiro para o hashfile analisado.
/// @return Um inteiro que representa o número de registros do hashfile.
/// @warning
int getNumeroRegistrosHF(HashFile hf);

#endif //HASHFILE_H