#include <stdio.h>
#include <math.h>
#include "financiamento.h"

/* Contador global de testes executados e falhados, usado pelo resumo final. */
static int total_testes = 0;
static int testes_falhos = 0;

/**
 * @brief Compara dois valores double com uma margem de erro aceitável.
 */
static int quase_igual(double a, double b) {
    const double epsilon = 0.01; /* 1 centavo */
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
    Simulacao s;
    double pv;
    Amortizacao a;

    printf("Executando testes do módulo financiamento...\n\n");

    /* ---- Casos de entrada válida ---- */

    /* R$ 10000, 2% ao mes, 12 parcelas => parcela ~945.60 */
    verificar(
        "simular_fin com juros e sem entrada",
        simular_fin(10000, 0, 0.02, 12, &s) == 0 && quase_igual(s.parcela, 945.60)
    );

    /* Entrada de 2000 => financia 8000 => parcela ~756.48 */
    verificar(
        "simular_fin com entrada e juros",
        simular_fin(10000, 2000, 0.02, 12, &s) == 0 && quase_igual(s.parcela, 756.48)
    );

    /* Sem juros: 1200 / 12 = 100 */
    verificar(
        "simular_fin sem juros",
        simular_fin(1200, 0, 0.0, 12, &s) == 0 && quase_igual(s.parcela, 100.0)
    );

    /* Parcela de ~945.60 a 2% em 12 meses => ~10000 financiados */
    verificar(
        "valor_financiavel com juros",
        valor_financiavel(945.596, 0.02, 12, &pv) == 0 && quase_igual(pv, 10000.0)
    );

    /* Sem extra quita em 12 meses; com extra de 200 quita em 10 */
    verificar(
        "amortizacao sem extra",
        amortizacao(10000, 945.596, 0, 0.02, &a) == 0 && a.parcelas == 12
    );
    verificar(
        "amortizacao com extra de 200",
        amortizacao(10000, 945.596, 200, 0.02, &a) == 0 && a.parcelas == 10
    );

    /* ---- Casos de valor limite / entrada inválida ---- */

    verificar(
        "simular_fin com prazo zero retorna -1",
        simular_fin(10000, 0, 0.02, 0, &s) == -1
    );

    verificar(
        "simular_fin com entrada igual ao valor retorna -1",
        simular_fin(10000, 10000, 0.02, 12, &s) == -1
    );

    verificar(
        "valor_financiavel com parcela zero retorna -1",
        valor_financiavel(0, 0.02, 12, &pv) == -1
    );

    verificar(
        "amortizacao que nunca quita retorna -1",
        amortizacao(10000, 150, 0, 0.02, &a) == -1
    );

    /* ---- Resumo final ---- */
    printf("\n%d de %d testes passaram.\n", total_testes - testes_falhos, total_testes);

    /* código de retorno != 0 se algum teste falhou */
    return testes_falhos > 0 ? 1 : 0;
}