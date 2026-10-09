#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include "metas.h"

static int total = 0;
static int falhas = 0;

/* Compara dois doubles com tolerância */
static void verifica_double(const char *nome, double obtido, double esperado, double tol)
{
    total++;
    if (fabs(obtido - esperado) <= tol) {
        printf("[PASSOU] %s\n", nome);
    } else {
        falhas++;
        printf("[FALHOU] %s: obtido %.4f, esperado %.4f\n", nome, obtido, esperado);
    }
}

/* Compara dois inteiros (usado para códigos de status) */
static void verifica_int(const char *nome, int obtido, int esperado)
{
    total++;
    if (obtido == esperado) {
        printf("[PASSOU] %s\n", nome);
    } else {
        falhas++;
        printf("[FALHOU] %s: obtido %d, esperado %d\n", nome, obtido, esperado);
    }
}

static void testa_percentual(void)
{
    printf("\n--- meta_percentual_atingido ---\n");

    verifica_double("25% da meta",         meta_percentual_atingido(200.0, 50.0),  25.0,  1e-6);
    verifica_double("meta exatamente atingida", meta_percentual_atingido(100.0, 100.0), 100.0, 1e-6);
    verifica_double("acima da meta (150%)", meta_percentual_atingido(100.0, 150.0), 150.0, 1e-6);
    verifica_double("valor atual zero",    meta_percentual_atingido(100.0, 0.0),    0.0,   1e-6);

    /* entradas inválidas */
    verifica_double("meta zero",           meta_percentual_atingido(0.0, 50.0),    -1.0,  1e-6);
    verifica_double("meta negativa",       meta_percentual_atingido(-10.0, 5.0),   -1.0,  1e-6);
    verifica_double("valor atual negativo", meta_percentual_atingido(100.0, -1.0), -1.0,  1e-6);
}

static void testa_aporte(void)
{
    double aporte;
    double meses = -999.0;

    printf("\n--- meta_aporte_mensal ---\n");

    verifica_double("do zero: 10000 a 1% em 12 meses", meta_aporte_mensal(10000.0, 0.0, 0.01, 12), 788.49, 0.01);
    verifica_double("com saldo: 10000, saldo 2500",    meta_aporte_mensal(10000.0, 2500.0, 0.01, 12), 566.37, 0.01);
    verifica_double("taxa zero do zero (1200 em 12 meses)", meta_aporte_mensal(1200.0, 0.0, 0.0, 12), 100.0, 1e-6);
    verifica_double("taxa zero com saldo (1200, saldo 600)", meta_aporte_mensal(1200.0, 600.0, 0.0, 12), 50.0, 1e-6);
    verifica_double("1 unico periodo",        meta_aporte_mensal(1000.0, 0.0, 0.05, 1), 1000.0, 1e-6);
    verifica_double("saldo igual a meta",     meta_aporte_mensal(1000.0, 1000.0, 0.01, 12), 0.0, 1e-6);
    verifica_double("saldo rende e passa a meta", meta_aporte_mensal(1000.0, 2000.0, 0.01, 12), 0.0, 1e-6);

    /* consistencia: o aporte calculado deve levar a meta no prazo pedido */
    aporte = meta_aporte_mensal(10000.0, 2500.0, 0.01, 12);
    meta_prazo_necessario(10000.0, 2500.0, aporte, 0.01, &meses);
    verifica_double("aporte calculado atinge a meta em 12 meses", meses, 12.0, 1e-6);

    /* entradas invalidas */
    verifica_double("periodos zero",     meta_aporte_mensal(1000.0, 0.0, 0.01, 0),   -1.0, 1e-6);
    verifica_double("periodos negativo", meta_aporte_mensal(1000.0, 0.0, 0.01, -5),  -1.0, 1e-6);
    verifica_double("meta zero",         meta_aporte_mensal(0.0, 0.0, 0.01, 12),     -1.0, 1e-6);
    verifica_double("taxa negativa",     meta_aporte_mensal(1000.0, 0.0, -0.01, 12), -1.0, 1e-6);
    verifica_double("saldo negativo",    meta_aporte_mensal(1000.0, -1.0, 0.01, 12), -1.0, 1e-6);
}

static void testa_prazo(void)
{
    double meses;
    int st;

    printf("\n--- meta_prazo_necessario ---\n");

    /* casos normais */
    meses = -999.0;
    st = meta_prazo_necessario(50000.0, 2000.0, 500.0, 0.008, &meses);
    verifica_int("caso normal: status", st, META_OK);
    verifica_double("caso normal: meses", meses, 69.81, 0.05);

    meses = -999.0;
    st = meta_prazo_necessario(10000.0, 1000.0, 500.0, 0.0, &meses);
    verifica_int("taxa zero: status", st, META_OK);
    verifica_double("taxa zero: meses", meses, 18.0, 1e-6);

    meses = -999.0;
    st = meta_prazo_necessario(5000.0, 6000.0, 100.0, 0.01, &meses);
    verifica_int("ja atingiu a meta: status", st, META_OK);
    verifica_double("ja atingiu a meta: meses", meses, 0.0, 1e-6);

    meses = -999.0;
    st = meta_prazo_necessario(1000.0, 500.0, 0.0, 0.01, &meses);
    verifica_int("sem aporte, so rendimento: status", st, META_OK);
    verifica_double("sem aporte, so rendimento: meses", meses, 69.66, 0.05);

    /* metas inalcançáveis */
    meses = -999.0;
    st = meta_prazo_necessario(1000.0, 100.0, 0.0, 0.0, &meses);
    verifica_int("sem aporte e sem taxa", st, META_ERRO_INALCANCAVEL);
    verifica_double("sem aporte e sem taxa: meses intacto", meses, -999.0, 1e-6);

    meses = -999.0;
    st = meta_prazo_necessario(1000.0, 0.0, 0.0, 0.01, &meses);
    verifica_int("saldo zero, sem aporte, com taxa", st, META_ERRO_INALCANCAVEL);

    /* entradas inválidas */
    meses = -999.0;
    st = meta_prazo_necessario(0.0, 100.0, 50.0, 0.01, &meses);
    verifica_int("meta zero", st, META_ERRO_PARAMETRO);
    verifica_double("meta zero: meses intacto", meses, -999.0, 1e-6);

    st = meta_prazo_necessario(1000.0, -1.0, 50.0, 0.01, &meses);
    verifica_int("valor atual negativo", st, META_ERRO_PARAMETRO);

    st = meta_prazo_necessario(1000.0, 100.0, -50.0, 0.01, &meses);
    verifica_int("aporte negativo", st, META_ERRO_PARAMETRO);

    st = meta_prazo_necessario(1000.0, 100.0, 50.0, -0.01, &meses);
    verifica_int("taxa negativa", st, META_ERRO_PARAMETRO);

    st = meta_prazo_necessario(1000.0, 100.0, 50.0, 0.01, NULL);
    verifica_int("ponteiro nulo", st, META_ERRO_PONTEIRO_NULO);
}

int main(void)
{
    printf("=== Testes do modulo metas ===\n");

    testa_percentual();
    testa_aporte();
    testa_prazo();

    printf("\n==============================\n");
    printf("Total: %d | Passaram: %d | Falharam: %d\n", total, total - falhas, falhas);

    if (falhas == 0) {
        printf("RESULTADO: APROVADO\n");
        return 0;
    }
    printf("RESULTADO: FALHOU\n");
    return 1;
}