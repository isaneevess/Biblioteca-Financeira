#include <math.h>
#include "juros.h"

double juros_simples(double principal, double taxa, int periodos) {
   if (principal < 0) {
        return JUROS_ERRO_PRINCIPAL_INVALIDO;
    }
    if (taxa < -1.0) {
        return JUROS_ERRO_TAXA_INVALIDA;
    }
    if (periodos < 0) {
        return JUROS_ERRO_PERIODO_INVALIDO;
    }

    return principal * (1.0 + taxa * periodos);
} 

double juros_compostos(double principal, double taxa, int periodos) {
    if (principal < 0) {
        return JUROS_ERRO_PRINCIPAL_INVALIDO;
    }
    if (taxa < -1.0) {
        return JUROS_ERRO_TAXA_INVALIDA;
    }
    if (periodos < 0) {
        return JUROS_ERRO_PERIODO_INVALIDO;
    }

    return principal * pow(1.0 + taxa, periodos);
} 

double juros_continuos(double principal, double taxa, double tempo) {
    if (principal < 0) {
        return JUROS_ERRO_PRINCIPAL_INVALIDO;
    }
    if (tempo < 0) {
        return JUROS_ERRO_TEMPO_INVALIDO;
    }

    return principal * exp(taxa * tempo);
}