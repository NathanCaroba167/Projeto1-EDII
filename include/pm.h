//
// Created by natha on 06/04/2026.
//

#ifndef PM_H
#define PM_H

#include <stdio.h>
#include "hashfile.h"

/*
 Módulo encarregado de ler um arquivo (.pm) e inicializar as pessoas com as operações de abrir e criar
 */
/*
 Ponteiro do tipo FILE Arquivo
 */
typedef FILE* Arquivo;
typedef char* Nome;
typedef void* Pm;

/// @rief Abre o arquivo .pm para leitura.
/// @param arquivo Ponteiro para o nome do arquivo que será aberto.
/// @return Ponteiro para o arquivo que foi aberto.
/// @warning
Pm criarPm(Nome path);

/// @brief Cria habitantes lidos do (.pm) e coloca os habitantes no hashfile.
/// @param pm Ponteiro para o arquivo (.pm) que será lido.
/// @param hfHabitantes Ponteiro para o Hashfile que receberá os habitantes.
/// @warning
int criarHabitantesEMoradores(Arquivo pm, HashFile hfHabitantes);

/// @brief Libera a memória alocada para o pm.
/// @param pm Ponteiro para o pm que será eliminado.
/// @warning
void eliminarPm(Pm pm);

#endif //PM_H
