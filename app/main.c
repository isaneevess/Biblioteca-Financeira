#include <stdio.h>
#include "juros.h"
#include "metas.h"
#include "financiamento.h"
#include "tempo.h"



void demonstrar_juros() {
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

    double meta = 10000.0;
    double valor_atual = 2500.0;
    double taxa_mensal = 0.01;
    double aporte = 500.0;
    int prazo = 12;
    double meses = 0.0;
    double aporte_calculado;
    int status;
 
    printf("--- Modulo Metas ---\n");
    printf("Meta: R$ %.2f | Valor atual: R$ %.2f | Taxa: %.1f%% a.m.\n",
           meta, valor_atual, taxa_mensal * 100);
 
    /* 1. Quanto da meta ja foi atingido */
    printf("Percentual atingido: %.1f%%\n\n", meta_percentual_atingido(meta, valor_atual));
 
    /* 2. Quanto depositar por mes para atingir a meta em 'prazo' meses */
    printf("Aporte mensal para atingir a meta em %d meses:\n", prazo);
    printf("  Do zero:         R$ %.2f\n", meta_aporte_mensal(meta, 0.0, taxa_mensal, prazo));
    printf("  Com saldo atual: R$ %.2f\n", meta_aporte_mensal(meta, valor_atual, taxa_mensal, prazo));
    printf("  Sem juros:       R$ %.2f\n\n", meta_aporte_mensal(meta, valor_atual, 0.0, prazo));
 
    /* 3. Em quantos meses a meta e atingida com um aporte fixo */
    status = meta_prazo_necessario(meta, valor_atual, aporte, taxa_mensal, &meses);
    if (status == META_OK)
        printf("Prazo com aporte de R$ %.2f: %.1f meses\n", aporte, meses);
 
    /* 4. Conferencia: o aporte calculado deve levar a meta no prazo pedido */
    aporte_calculado = meta_aporte_mensal(meta, valor_atual, taxa_mensal, prazo);
    status = meta_prazo_necessario(meta, valor_atual, aporte_calculado, taxa_mensal, &meses);
    if (status == META_OK)
        printf("Conferencia: aporte de R$ %.2f atinge a meta em %.1f meses\n\n", aporte_calculado, meses);
 
    /* 5. Meta ja atingida */
    status = meta_prazo_necessario(5000.0, 6000.0, 100.0, taxa_mensal, &meses);
    if (status == META_OK)
        printf("Meta de R$ 5000.00 com R$ 6000.00 guardados: %.1f meses (%.0f%% da meta)\n",
               meses, meta_percentual_atingido(5000.0, 6000.0));
 
    /* 6. Casos de erro tratados pela biblioteca */
    status = meta_prazo_necessario(meta, valor_atual, 0.0, 0.0, &meses);
    if (status == META_ERRO_INALCANCAVEL)
        printf("Sem aporte e sem juros: meta inalcancavel\n");
 
    status = meta_prazo_necessario(-100.0, valor_atual, aporte, taxa_mensal, &meses);
    if (status == META_ERRO_PARAMETRO)
        printf("Meta negativa: parametro invalido\n");
 
    printf("\n");
}

void demonstrar_financiamento() {
    double valor = 1000.0;
    double entrada = 0.0;
    double taxa = 0.05;
    int n = 3;
    double extra = 100.0;

    Simulacao s;
    double maximo;
    Amortizacao sem_extra, com_extra;

    printf("--- Modulo Financiamento ---\n");
    printf("Valor: R$ %.2f | Entrada: R$ %.2f | Taxa: %.1f%% | Parcelas: %d\n", valor, entrada, taxa * 100, n);

    if (simular_fin(valor, entrada, taxa, n, &s) == 0) {
        printf("Parcela:          R$ %.2f\n", s.parcela);
        printf("Total pago:       R$ %.2f\n", s.total_pago);
        printf("Total de juros:   R$ %.2f (%.2f%% do financiado)\n", s.total_juros, s.percentual);

        if (valor_financiavel(s.parcela, taxa, n, &maximo) == 0) {
            printf("Valor financiavel com essa parcela: R$ %.2f\n", maximo);
        }

        if (amortizacao(valor - entrada, s.parcela, 0.0, taxa, &sem_extra) == 0 &&
            amortizacao(valor - entrada, s.parcela, extra, taxa, &com_extra) == 0) {
            printf("Sem extra:        %d parcelas, R$ %.2f de juros\n", sem_extra.parcelas, sem_extra.total_juros);
            printf("Com extra R$ %.2f: %d parcelas, R$ %.2f de juros\n", extra, com_extra.parcelas, com_extra.total_juros);
            printf("Economia:         %d parcelas e R$ %.2f de juros\n",
                   sem_extra.parcelas - com_extra.parcelas, sem_extra.total_juros - com_extra.total_juros);
        }
    } else {
        printf("Parametros invalidos.\n");
    }
    printf("\n");
}

int main(void) {
    printf("=== Demonstracao da Biblioteca Financeira ===\n\n");
    demonstrar_juros();
    demonstrar_metas();
    demonstrar_financiamento();
    return 0;
}