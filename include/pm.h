//
// Created by natha on 06/04/2026.
//

#ifndef PM_H
#define PM_H

#include "hashfile.h"

/*
 Módulo encarregado de ler um arquivo (.pm) e inicializar as pessoas com as operações de abrir e criar
 */
/*
 Ponteiro do tipo FILE Arquivo
 */
typedef FILE* Arquivo;
typedef char* Nome;

/// @rief Abre o arquivo .pm para leitura.
/// @param arquivo Ponteiro para o nome do arquivo que será aberto.
/// @return Ponteiro para o arquivo que foi aberto.
/// @warning
Arquivo abrirPM(Nome arquivo);

/// @brief Cria habitantes lidos do (.pm) e coloca os habitantes no hashfile.
/// @param pm Ponteiro para o arquivo (.pm) que será lido.
/// @param hfHabitantes Ponteiro para o Hashfile que receberá os habitantes.
/// @warning
void criarHabitantes(Arquivo pm,HashFile hfHabitantes);

#endif //PM_H
