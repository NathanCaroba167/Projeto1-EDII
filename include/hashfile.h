//
// Created by natha on 21/03/2026.
//

#ifndef HASHFILE_H
#define HASHFILE_H

#include <stddef.h>
#include <stdint.h>

/*
 Módulo encarregado de gerenciar um Hashfile Dinâmino (Hashing Extensível) em disco(arquivo binário),
 com operações de criar, abrir, fechar, inserir, buscar, remover, atualizar e dump.
 O módulo é genérico: opera sobre registros de tamanho fixo sem conhecer o tipo concreto.
 A localização da chave dentro do registro é configurada via HashFileConfig (offsetof/sizeof) */

/* Códigos de retorno das operações*/
#define HF_OK 0
#define HF_ERROR (-1)
#define HF_NAO_ENCONTRADO (-2)

typedef void (*FuncaoFormatoRegistroDump)(void* registro, char* buffer, size_t tamanhoBuffer);

/*
 Ponteiro void*.
 */
typedef void* HashFileConfig;
typedef void* HashFile;

/*typedef FILE* Arquivo;*/
typedef char* Nome;

/// @brief Cria uma configuração para o uso do HashFile
/// @param record_size Tamanho total de um registro em bytes
/// @param key_offset Deslocamento em bytes da chave dentro do registro
/// @param key_size Tamanho da chave em bytes
/// @param capacidade_bucket Número máximo de registros por bucket
/// @param funcaoFormato Ponteiro para funcao que ira definir o formato do registro no (.hfd).
/// @return Ponteiro para a configuração criada
/// @warning
HashFileConfig criarHashFileConfig(size_t record_size,size_t key_offset,size_t key_size,int capacidade_bucket, FuncaoFormatoRegistroDump funcaoFormato);

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
/// @return 0 para insercao correta , -1 para insercao incorreta
/// @warning
int inserirHashFile(HashFile hf, void* registro);

/// @brief Busca um registro pela chave (codigo) no hashfile.
/// @param hf Ponteiro para o hashfile analisado.
/// @param chave Ponteiro para a chave a ser buscada.
/// @param registro_clone Ponteiro onde o registro encontrado será copiado.
/// @return True se encontrou e False caso contrario.
/// @warning
int buscarHashFile(HashFile hf, void* chave, void* registro_clone);

/// @brief Remove um registro do sistema de hash conforme a chave.
/// @param hf Ponteiro para o hashfile analisado.
/// @param chave Ponteiro para a chave do registro a remover.
/// @param registro_clone Ponteiro onde o registro removido será copiado
/// @return 0 para remocao correta , -1 para remocao incorreta
/// @warning
int removerHashFile(HashFile hf, void* chave, void* registro_clone);

/// @brief Atualiza um registro existente no hashfile
/// @param hf Ponteiro para o hashfile analisado
/// @param registro_novo Ponteiro para o novo registro
/// @return 0 para atualizacao correta , -1 para atualizacao incorreta
/// @warning
int atualizarHashFile(HashFile hf, void* registro_novo);

/// @brief Gera o arquivo de dump (.hfd)
/// @param hf Ponteiro para o hashfile analisado
/// @param arquivoPath Caminho do arquivo dump a ser gerado
/// @return 0 para um dump correto , -1 para um dump incorreto
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

/// @brief Percorre todos os registros do hashfile chamando um callback para para registro.
/// @param hf Ponteiro para o hashfile analisado.
/// @param callback Função chamada para cada registro do hashfile.
/// @param ponteiro Ponteiro auxiliar passado para função de callback.
/// @return Número de registros visitados.
/// @warning
int percorrerHash(HashFile hf, void(*callback)(void* registro, void* ponteiro), void* ponteiro);

#endif //HASHFILE_H