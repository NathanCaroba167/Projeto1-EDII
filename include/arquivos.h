//
// Created by natha on 16/10/2025.
//

#ifndef ARQUIVOS_H
#define ARQUIVOS_H

/*
 Módulo encarregado de pegar o nome base de um arquivo a partir da operação de get.
 */

/// @brief Pega o nome base de um arquivo, removendo o caminho do diretório e a extensão.
/// @param nomeArq O caminho completo ou nome do arquivo que será analisado (ex: "dir/arquivo.txt").
/// @return String com o nome base do arquivo alocada (o nome base, ex: "arquivo").
/// @warning A memória para a string resultante é alocada dinamicamente.
char* getNomeBase(char* nomeArq);

#endif //ARQUIVOS_H