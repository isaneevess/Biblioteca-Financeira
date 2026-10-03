#include <math.h>
#include "financiamento.h"

int simular_fin(double valor, double entrada, double taxa, int n, Simulacao *resultado) {
    if (valor > 0 && entrada >= 0 && entrada < valor && taxa >= 0 && n > 0) {
        double financiado = valor - entrada;
        double parcela;
        if (taxa == 0) {
            parcela = financiado / n;
        }
        else {
            parcela = financiado * taxa / (1.0 - pow(1.0 + taxa, -n));
        }
        double total_pago = entrada + parcela * n;
        double total_juros = total_pago - valor;
        double percentual = (total_juros / financiado) * 100;

        resultado->parcela = parcela;
        resultado->total_pago = total_pago;
        resultado->total_juros = total_juros;
        resultado->percentual = percentual;

        return 0; 
    }
    else {
        return -1;
    }

}

int valor_financiavel(double parcela, double taxa, int n, double *resultado) {
    if (parcela > 0 && taxa >= 0 && n > 0) {
        double financiado;
        if (taxa == 0) {
            financiado = parcela * n;
        }
        else {
            financiado = parcela * (1.0 - pow(1.0 + taxa, -n)) / taxa;
        }

        *resultado = financiado;
        return 0; 
    }
    else {
        return -1;
    }

}

int amortizacao(double divida, double parcela, double extra, double taxa, Amortizacao *resultado) {
    if (divida > 0 && parcela > 0 && extra >= 0 && taxa >= 0 && (parcela + extra > divida * taxa)) {
        int meses = 0;
        double juros = 0, total_juros = 0;
        for (double saldo = divida; saldo > 0.005; meses++) {
            juros = saldo * taxa;
            saldo += juros - (parcela + extra);
            total_juros += juros;
        }

        resultado->meses = meses;
        resultado->total_juros = total_juros;
        return 0;
    }
    else {
        return -1;
    }
}