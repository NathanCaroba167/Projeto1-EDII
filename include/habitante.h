//
// Created by natha on 30/03/2026.
//

#include <stdbool.h>

#ifndef HABITANTE_H
#define HABITANTE_H

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
/// @param morador Flag para identifica se e morador.
/// @return Ponteiro para o habitante criado.
/// @warning
Habitante CriarHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc, bool morador);

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

/// @brief Libera a memória alocada para o habitante, incluindo as strings de nome e sobrenome.
/// @param hab Ponteiro para o habitante que será eliminado.
/// @warning
void eliminarHabitante(Habitante hab);

#endif //HABITANTE_H