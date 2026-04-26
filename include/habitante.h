//
// Created by natha on 30/03/2026.
//

#ifndef HABITANTE_H
#define HABITANTE_H

#include <stdbool.h>
#include "hashfile.h"

/*
 Módulo encarregado de coordenar um habitante com operações de criar, get, set e eliminar
 */

/*
 Ponteiro void Habitante
 */
typedef void* Habitante;

/// @brief Cria um habitante com os atributos indicados.
/// @param cpf Identificador único que se refere ao habitante.
/// @param nome Nome do habitante criado.
/// @param sobrenome Sobrenome do habitante criado.
/// @param sexo Sexo do habitante criado.
/// @param nasc Data de nascimento do habitante criado.
/// @return Ponteiro para o habitante criado.
/// @warning
Habitante criarHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc);

/// @brief Pega o CPF do habitante sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Um string que representa o CPF do habitante.
/// @warning
char* getCPFHabitante(Habitante hab);

/// @brief Define o CPF do habitante.
/// @param hab Ponteiro para o habitante onde o CPF será definido.
/// @param cpf CPF que será definido.
/// @warning
void setCPFHabitante(Habitante hab, double cpf);

/// @brief Pega o nome do habitante sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Uma string que representa o nome do habitante.
/// @warning
char* getNomeHabitante(Habitante hab);

/// @brief Define o nome do habitante.
/// @param hab Ponteiro para o habitante onde o nome será definido.
/// @param nome Nome que será definido.
/// @warning
void setNomeHabitante(Habitante hab, char* nome);

/// @brief Pega o sobrenome do habitante sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Uma string que representa o sobrenome do habitante.
/// @warning
char* getSobrenomeHabitante(Habitante hab);

/// @brief Define o sobrenome do habitante.
/// @param hab Ponteiro para o habitante onde o sobrenome será definido.
/// @param sobrenome Sobrenome que será definido.
/// @warning
void setSobrenomeHabitante(Habitante hab, char* sobrenome);

/// @brief Pega o sexo do habitante sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Um char que representa o sexo do habitante.
/// @warning
char getSexoHabitante(Habitante hab);

/// @brief Pega a data de nascimento sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Um string que representa a data de nascimento do habitante.
/// @warning
char* getNascHabitante(Habitante hab);

/// @brief Retorna se o habitante é morador ou sem-teto.
/// @param hab Ponteiro para o habitante analisado.
/// @return True se o habitante é morador, False se o habitante é sem-teto.
/// @warning
bool isMorador(Habitante hab);

/// @brief Define o endereço do habitante.
/// @param hab Ponteiro para o habitante onde o endereço será definido.
/// @param cep CEP que será definido.
/// @param face Face que será definida.
/// @param num Número do endereço que será definido.
/// @param complemento Complemento do endereço que será definido.
/// @warning
void setEnderecoHabitante(Habitante hab, char* cep, char face, int num, char* complemento);

/// @brief Remove o endereço do habitante, tornando-o sem-teto.
/// @param hab Ponteiro para o habitante que será sem-teto.
/// @warning
void removerEnderecoHabitante(Habitante hab);

/// @brief Pega o CEP do endereço do morador sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return String do CEP do endereço.
/// @warning
char* getCEPEnderecoHabitante(Habitante hab);

/// @brief Pega a face do endereço do morador sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Char da face do endereço.
/// @warning
char getFaceEnderecoHabitante(Habitante hab);

/// @brief Pega o número do endereço do morador sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return Inteiro do número do endereço.
/// @warning
int getNumeroEnderecoHabitante(Habitante hab);

/// @brief Pega o complemento do endereço do morador sem modifica-lo.
/// @param hab Ponteiro para o habitante analisado.
/// @return String do complemento do endereço.
/// @warning
char* getComplementoHabitante(Habitante hab);

/// @brief Cria um HashFileConfig configurada para o registro de habitantes.
/// @param capacidadeBucket Capacidade de registros por bucket.
/// @return HashFileConfig configurada para registrar habitantes.
/// @warning
HashFileConfig criarHashFileConfigHabitantes(int capacidadeBucket);

/// @brief Escreve em um buffer um habitante para inserção como registro no hashfile
/// @param hab Ponteiro para o habitante analisado.
/// @param buffer Buffer de registro que receberá o habitante para registro no hashfile.
/// @warning
void habitanteBufferParaRegistro(Habitante hab, void* buffer);

/// @brief Cria um habitante a partir de um buffer de registro lido do hashfile
/// @param buffer Buffer de registro lido do hashfile.
/// @return Ponteiro para o habitante criado.
/// @warning
Habitante registroParaHabitanteBuffer(void* buffer);

/// @brief Libera a memória alocada para o habitante, incluindo as strings de nome e sobrenome.
/// @param hab Ponteiro para o habitante que será eliminado.
/// @warning
void eliminarHabitante(Habitante hab);

#endif //HABITANTE_H