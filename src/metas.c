#include <math.h>
#include <stddef.h>
#include "metas.h"
#include "juros.h"

double meta_percentual_atingido(double meta, double valor_atual)
{
    /* TODO: repensar se valor_atual negativo deve ser aceito */
    if (meta <= 0.0 || valor_atual < 0.0)
        return -1.0;

    return (valor_atual / meta) * 100.0;
}

double meta_aporte_mensal(double meta, double valor_atual, double taxa, int periodos)
{
    double fator;
    double faltante;
 
    if (meta <= 0.0 || valor_atual < 0.0 || taxa < 0.0 || periodos <= 0)
        return -1.0;
 
    fator = juros_compostos(1.0, taxa, periodos);   
    faltante = meta - valor_atual * fator;          
 
    if (faltante <= 0.0)
        return 0.0;                                 
 
    if (taxa == 0.0)
        return faltante / periodos;
 
    return faltante * taxa / (fator - 1.0);
}
 

int meta_prazo_necessario(double meta, double valor_atual, double aporte_mensal,
                          double taxa_mensal, double *meses)
{
    double denominador;

    if (meses == NULL)
        return META_ERRO_PONTEIRO_NULO;

    if (meta <= 0.0 || valor_atual < 0.0 || aporte_mensal < 0.0 || taxa_mensal < 0.0)
        return META_ERRO_PARAMETRO;

    if (valor_atual >= meta) {
        *meses = 0.0;
        return META_OK;
    }

    if (taxa_mensal == 0.0) {
        if (aporte_mensal == 0.0)
            return META_ERRO_INALCANCAVEL;
        *meses = (meta - valor_atual) / aporte_mensal;
        return META_OK;
    }

    denominador = valor_atual * taxa_mensal + aporte_mensal;
    if (denominador <= 0.0)
        return META_ERRO_INALCANCAVEL;

    *meses = log((meta * taxa_mensal + aporte_mensal) / denominador)
             / log(1.0 + taxa_mensal);
    return META_OK;
}