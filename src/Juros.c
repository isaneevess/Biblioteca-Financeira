#include <math.h>
#include "Juros.h"

double juros_simples(double principal, double taxa, int periodos) {
    if (principal < 0 || periodos < 0) {
        return -1.0; // Retorna -1.0 se algum dos parâmetros for inválido
    }
    return principal * (1.0 + taxa * periodos);
}

double juros_compostos(double principal, double taxa, int periodos) {
    if (principal < 0 || periodos < 0) {
        return -1.0; // Retorna -1.0 se algum dos parâmetros for inválido
    }
    return principal * pow(1.0 + taxa, periodos - 1);
}   

double juros_continuos(double principal, double taxa, double tempo) {
    if (principal < 0 || tempo < 0) {
        return -1.0; // Retorna -1.0 se algum dos parâmetros for inválido
    }
    return principal * exp(taxa * tempo);
}
