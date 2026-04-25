//
// Created by natha on 16/10/2025.
//

#ifndef TXT_H
#define TXT_H

/*
 Módulo encarregado de criar um arquivo (.txt) e reportar os resultados de comandos com as operações de abrir e escrever
 */

/*
 Ponteiro do tipo FILE Arquivo
 Ponteiro char Nome
 */
typedef FILE* Arquivo;
typedef char* Nome;

/// @brief Cria e abre o arquivo TXT para escrita.
/// @param arquivo Ponteiro para o nome do arquivo que será criado e aberto.
/// @return Ponteiro para o arquivo (FILE*) que será criado e aberto.
/// @warning
Arquivo abrirTXT(Nome arquivo);

/// @brief Escreve os dados de um habitante no arquivo TXT.
/// @param txt Ponteiro para o arquivo que será escrito os dados.
/// @param cpf CPF do habitante.
/// @param nome Nome do habitante.
/// @param sobrenome Sobrenome do habitante.
/// @param sexo Sexo do habitante.
/// @param nascimento Data de nascimento do habitante
/// @param morador True se for morador, False se não for morador.
/// @param cep CEP do endereço caso o habitante seja morador.
/// @param face Face do endereço caso o habitante seja morador.
/// @param numero Número do endereço caso o habitante seja morador.
/// @param complemento Complemento do endereço caso o habitante seja morador.
/// @warning
void escreverHabitante(Arquivo txt, char* cpf, char* nome, char* sobrenome, char sexo, char* nascimento, int morador, char* cep, char face, int numero, char* complemento);

/// @brief Escreve as estatísticas do censo no arquivo TXT.
/// @param txt Ponteiro para o arquivo que será escrito os dados.
/// @param totalHabitantes Total de Habitantes da cidade.
/// @param totalMoradores Total de Moradores da cidade.
/// @param homens Total de Homens da cidade.
/// @param mulheres Total de Mulheres da cidade.
/// @param moradoresHomens Total de Moradores Homens da cidade.
/// @param moradoresMulheres Total de Moradores Mulheres da cidade.
/// @param semTeto Total de Sem-tetos da cidade.
/// @param semTetoHomens Total de Sem-tetos Homens da cidade.
/// @param semTetoMulheres Total de Sem-tetos Mulheres da cidade.
/// @warning
void escreverCenso(Arquivo txt, int totalHabitantes, int totalMoradores, int homens, int mulheres, int moradoresHomens, int moradoresMulheres, int semTeto, int semTetoHomens, int semTetoMulheres);

#endif //TXT_H