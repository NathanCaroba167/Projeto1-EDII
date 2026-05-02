//
// Created by natha on 06/04/2026.
//

#include "../include/habitante.h"
#include "../unity/unity.h"

#include "../include/pm.h"

#define PM_SIMPLES "/tmp/t_pm_simples.pm"
#define PM_COM_M "/tmp/t_pm_com_m.pm"
#define BASE_HF "/tmp/t_pm_hf"

Pm pm  = NULL;
HashFileConfig config = NULL;
HashFile hf  = NULL;

void setUp(void) {
    FILE* f = fopen("/dev/null", "w");
    fprintf(f,"p 111.111.111-11 Ana Silva F 01/01/1990\n");
    fprintf(f, "p 222.222.222-22 Bruno Costa M 15/06/1985\n");
    fclose(f);

    f = fopen(PM_COM_M, "w");
    fprintf(f, "p 333.333.333-33 Carla Souza F 20/03/1992\n");
    fprintf(f, "p 444.444.444-44 Davi Lima M 07/11/1978\n");
    fprintf(f, "m 333.333.333-33 cep05 S 45 apto-2\n");
    fprintf(f, "m 444.444.444-44 cep07 N 10 casa\n");
    fclose(f);
}
void tearDown(void) {
    if (hf)  { fecharHashFile(hf); hf  = NULL; }
    if (pm)  { eliminarPm(pm); pm  = NULL; }
    if (config) { eliminarHashFileConfig(config); config = NULL; }
    remove(PM_SIMPLES);
    remove(PM_COM_M);
    remove(BASE_HF ".hf");
    remove(BASE_HF ".hfc");
}

void teste_processar_deveRetornarNumeroDeHabitantesInseridos(void) {
    pm = criarPm(PM_SIMPLES);
    TEST_ASSERT_EQUAL_INT(2, criarHabitantesEMoradores(pm, hf));
}

void teste_processar_habitantesDevemEstarNoHashfile(void) {
    pm = criarPm(PM_SIMPLES);
    criarHabitantesEMoradores(pm, hf);
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "111.111.111-11", NULL));
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "222.222.222-22", NULL));
}

void teste_processar_dadosPessoaisDevemEstarCorretos(void) {
    pm = criarPm(PM_SIMPLES);
    criarHabitantesEMoradores(pm, hf);

    char buffer[512];
    buscarHashFile(hf, "111.111.111-11", buffer);
    Habitante hab = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(hab);

    TEST_ASSERT_EQUAL_STRING("111.111.111-11", getCPFHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("Ana", getNomeHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("Silva", getSobrenomeHabitante(hab));
    TEST_ASSERT_EQUAL_INT('F', getSexoHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("01/01/1990", getNascHabitante(hab));

    eliminarHabitante(hab);
}

void teste_processar_habitanteInseridoDeveSerSemTeto(void) {
    pm = criarPm(PM_SIMPLES);
    criarHabitantesEMoradores(pm, hf);

    char buffer[512];
    buscarHashFile(hf, "111.111.111-11", buffer);
    Habitante hab = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(hab);
    TEST_ASSERT_TRUE(!isMorador(hab));

    eliminarHabitante(hab);
}

void teste_processar_com_m_habitanteDeveSerMorador(void) {
    pm = criarPm(PM_COM_M);
    criarHabitantesEMoradores(pm, hf);

    char buffer[512];
    buscarHashFile(hf, "333.333.333-33", buffer);
    Habitante hab = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(hab);
    TEST_ASSERT_TRUE(isMorador(hab));

    eliminarHabitante(hab);
}

void teste_processar_com_m_enderecoDeveEstarCorreto(void) {
    pm = criarPm(PM_COM_M);
    criarHabitantesEMoradores(pm, hf);

    char buffer[512];
    buscarHashFile(hf, "333.333.333-33", buffer);
    Habitante hab = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(hab);

    TEST_ASSERT_EQUAL_STRING("cep05", getCEPEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_INT('S', getFaceEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_INT(45, getNumeroEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("apto-2", getComplementoHabitante(hab));

    eliminarHabitante(hab);
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO PM \n");
    printf("==========================================\n");

    RUN_TEST(teste_processar_deveRetornarNumeroDeHabitantesInseridos);
    RUN_TEST(teste_processar_habitantesDevemEstarNoHashfile);
    RUN_TEST(teste_processar_dadosPessoaisDevemEstarCorretos);
    RUN_TEST(teste_processar_habitanteInseridoDeveSerSemTeto);
    RUN_TEST(teste_processar_com_m_habitanteDeveSerMorador);
    RUN_TEST(teste_processar_com_m_enderecoDeveEstarCorreto);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}