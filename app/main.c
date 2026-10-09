#include <stdio.h>
#include "juros.h"
#include "metas.h"
#include "financiamento.h"
#include "tempo.h"



void demonstrar_juros(void) {
    double principal = 1000.0;
    double taxa = 0.05;
    int periodos = 3;

    printf("--- Modulo Juros ---\n");
    printf("Principal: R$ %.2f | Taxa: %.1f%% | Periodos: %d\n", principal, taxa * 100, periodos);
    printf("Juros simples:   R$ %.2f\n", juros_simples(principal, taxa, periodos));
    printf("Juros compostos: R$ %.2f\n", juros_compostos(principal, taxa, periodos));
    printf("Juros continuos: R$ %.2f\n\n", juros_continuos(principal, taxa, (double) periodos));
}

void demonstrar_metas(){

    printf("--- Modulo Metas ---\n");
}

int main(void) {
    printf("=== Demonstracao da Biblioteca Financeira ===\n\n");
    demonstrar_juros();
    demonstrar_juros();
    return 0;
}