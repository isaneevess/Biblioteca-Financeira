#include <stdio.h>
#include <math.h>
#include "juros.h"

/* Contador global de testes executados e falhados, usado pelo resumo final. */
static int total_testes = 0;
static int testes_falhos = 0;

/**
 * @brief Compara dois valores double com uma margem de erro aceitável.
 *
 * Comparar double com == é arriscado por causa de imprecisão de ponto
 * flutuante, então usamos uma margem pequena (epsilon).
 */
static int quase_igual(double a, double b) {
    const double epsilon = 0.001;
    return fabs(a - b) < epsilon;
}

/**
 * @brief Executa um teste e imprime o resultado (PASSOU/FALHOU).
 */
static void verificar(const char *nome_teste, int condicao) {
    total_testes++;
    if (condicao) {
        printf("[PASSOU] %s\n", nome_teste);
    } else {
        printf("[FALHOU] %s\n", nome_teste);
        testes_falhos++;
    }
}

int main(void) {
    printf("Executando testes do modulo juros...\n\n");

    /* ---- Casos de entrada válida ---- */

    /* R$ 1000, 5% ao periodo, 3 periodos => 1000 * (1 + 0.05*3) = 1150 */
    verificar(
        "juros_simples com valores validos",
        quase_igual(juros_simples(1000.0, 0.05, 3), 1150.0)
    );

    /* R$ 1000, 5% ao periodo, 3 periodos => 1000 * (1.05)^3 = 1157.625 */
    verificar(
        "juros_compostos com valores validos",
        quase_igual(juros_compostos(1000.0, 0.05, 3), 1157.625)
    );

    /* R$ 1000, taxa continua de 5% ao periodo, 3 periodos => 1000 * e^(0.05*3) = 1161.834... */
    verificar(
        "juros_continuos com valores validos",
        quase_igual(juros_continuos(1000.0, 0.05, 3.0), 1161.834)
    );

    /* Com tempo fracionario: 1000, 5%, 1.5 periodos => 1000 * e^(0.05*1.5) = 1077.884... */
    verificar(
        "juros_continuos com tempo fracionario",
        quase_igual(juros_continuos(1000.0, 0.05, 1.5), 1077.884)
    );

    /* ---- Casos de valor limite / entrada inválida ---- */

    verificar(
        "juros_simples com principal negativo retorna -1",
        juros_simples(-100.0, 0.05, 3) == -1.0
    );

    verificar(
        "juros_compostos com periodos negativo retorna -1",
        juros_compostos(1000.0, 0.05, -1) == -1.0
    );

    verificar(
        "juros_simples com periodos igual a zero retorna o proprio principal",
        quase_igual(juros_simples(1000.0, 0.05, 0), 1000.0)
    );

    verificar(
        "juros_continuos com tempo negativo retorna -1",
        juros_continuos(1000.0, 0.05, -1.0) == -1.0
    );

    verificar(
        "juros_continuos com tempo igual a zero retorna o proprio principal",
        quase_igual(juros_continuos(1000.0, 0.05, 0.0), 1000.0)
    );

    /* ---- Resumo final ---- */
    printf("\n%d de %d testes passaram.\n", total_testes - testes_falhos, total_testes);

    /* código de retorno != 0 se algum teste falhou, para o "make test" detectar */
    return testes_falhos > 0 ? 1 : 0;
}
