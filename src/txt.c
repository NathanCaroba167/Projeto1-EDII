//
// Created by natha on 16/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/txt.h"

Arquivo abrirTXT(Nome arquivo) {
    // Abrindo o arquivo para escrita ('w')
    Arquivo txt = fopen(arquivo,"w");
    if (txt == NULL) {
        printf("Erro ao abrir o arquivo txt!\n");

        perror("Motivo do erro");
        exit(1);
    }
    return txt;
}

void escreverHabitante(Arquivo txt, char* cpf, char* nome, char* sobrenome, char sexo, char* nascimento, int morador, char* cep, char face, int numero, char* complemento) {
    if (txt == NULL) {
        return;
    }
    fprintf(txt,"Cpf: %s\n", cpf);
    fprintf(txt,"Nome: %s %s\n", nome, sobrenome);
    fprintf(txt,"Sexo: %c\n", sexo);
    fprintf(txt,"Nascimento: %s\n", nascimento);

    if(morador) {
        fprintf(txt,"Endereco: %s/%c/%d %s\n", cep, face, numero, complemento);
    }else {
        fprintf(txt,"Sem-teto\n");
    }
}

void escreverCenso(Arquivo txt, int totalHabitantes, int totalMoradores, int homens, int mulheres, int moradoresHomens, int moradoresMulheres, int semTeto, int semTetoHomens, int semTetoMulheres) {
    if (txt == NULL) {
        return;
    }

    double proporcaoMorXHab = totalHabitantes > 0 ? (double) totalMoradores / totalHabitantes * 100.0 : 0.0;
    double porcentagemHabitantesHomens = totalHabitantes > 0 ? (double) homens / totalHabitantes * 100.0 : 0.0;
    double porcentagemHabitantesMulheres = totalHabitantes > 0 ? (double) mulheres / totalHabitantes * 100.0 : 0.0;
    double porcentagemMoradoresHomens = totalMoradores > 0 ? (double) moradoresHomens / totalMoradores * 100.0 : 0.0;
    double porcentagemMoradoresMulheres = totalMoradores > 0 ? (double) moradoresMulheres / totalMoradores * 100.0 : 0.0;
    double porcentagemHomensSemTeto = semTeto > 0 ? (double) semTetoHomens / semTeto * 100.0 : 0.0;
    double porcentagemMulheresSemTeto = semTeto > 0 ? (double) semTetoMulheres / semTeto * 100.0 : 0.0;

    fprintf(txt,"Total de Habitantes: %d\n", totalHabitantes);
    fprintf(txt,"Total de Moradores: %d\n", totalMoradores);
    fprintf(txt,"Proporcao Moradores/Habitantes: %.2f\n", proporcaoMorXHab);
    fprintf(txt,"Homens: %d\n", homens);
    fprintf(txt,"Mulheres: %d\n", mulheres);
    fprintf(txt,"Porcentagem(%%) habitantes homens: %.2f%%\n", porcentagemHabitantesHomens);
    fprintf(txt,"Porcentagem(%%) habitantes mulheres: %.2f%%\n", porcentagemHabitantesMulheres);
    fprintf(txt,"Porcentagem(%%) moradores homens: %.2f%%\n", porcentagemMoradoresHomens);
    fprintf(txt,"Porcentagem(%%) moradores mulheres: %.2f%%\n", porcentagemMoradoresMulheres);
    fprintf(txt,"Toral de Sem-Teto: %d\n", semTeto);
    fprintf(txt,"Porcentagem(%%) sem-teto homens: %.2f%%\n", porcentagemHomensSemTeto);
    fprintf(txt,"Porcentagem(%%) sem-teto mulheres: %.2f%%\n", porcentagemMulheresSemTeto);

}
