#include "metas.h"; 
#include<stdio.h>
#include<math.h>

double meta_percentual_atingido(double meta, double valor_atual){
    double percentual = 0.0; 
    percentual = (valor_atual/meta)*100; 
    return percentual; 
}

double meta_aporte_mensal(double meta, double taxa, int periodos){
    double aporte_mensal = 0.0; 
    aporte_mensal = meta * taxa / pow(1+taxa, periodos-1);
    return aporte_mensal;
}

int meta_prazo_necessario(double meta, double valor_atual, double aporte_mensal, double taxa_mensal, double *meses){
    *meses = log((meta * taxa_mensal + aporte_mensal) /
             (valor_atual * taxa_mensal + aporte_mensal)) / log(1.0 + taxa_mensal);
    return 1; 
}
