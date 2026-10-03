#include <math.h>
#include "Tempo.h"


static int ano_bissexto(int ano) {
    return (ano % 400 == 0) ||
           (ano % 4 == 0 && ano % 100 != 0);
}


static int dias_no_mes(int mes, int ano) { //calculo dias do mes
    switch (mes) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;

        case 4:
        case 6:
        case 9:
        case 11:
            return 30;

        case 2:
            return ano_bissexto(ano) ? 29 : 28;

        default:
            return 0;
    }
}


static int data_valida(Data data) {
    if (data.ano < 1)
        return 0;

    if (data.mes < 1 || data.mes > 12)
        return 0;

    if (data.dia < 1 ||
        data.dia > dias_no_mes(data.mes, data.ano))
        return 0;

    return 1;
}


static long long data_para_dias(Data data) {
    long long dias = 0;

    for (int ano = 1; ano < data.ano; ano++) {
        dias += ano_bissexto(ano) ? 366 : 365;
    }

    for (int mes = 1; mes < data.mes; mes++) {
        dias += dias_no_mes(mes, data.ano);
    }

    dias += data.dia;

    return dias;
}


int dias_entre_datas(Data inicio, Data fim) {   //PÚBLICA
    long long dias_inicio;
    long long dias_fim;
    long long diferenca;

    if (!data_valida(inicio) || !data_valida(fim))
        return -1;

    dias_inicio = data_para_dias(inicio);
    dias_fim = data_para_dias(fim);

    diferenca = dias_fim - dias_inicio;

    if (diferenca < 0)
        diferenca = -diferenca;

    return (int)diferenca;
}


int meses_para_quitar(double divida, double pagamento, double taxa) { //simula o pagamento da divida //PÚBLICA
    int meses = 0;

    if (divida <= 0.0 ||
        pagamento <= 0.0 ||
        taxa < 0.0) {
        return -1;
    }

    /* Caso sem juros */
    if (taxa == 0.0) {
        meses = (int)(divida / pagamento);

        if (divida > meses * pagamento)
            meses++;

        return meses;
    }

    //Se o pagamento não cobre nem os juros do mês a dívida nunca será quitada
    if (pagamento <= divida * taxa)
        return -1;

   //simula mes ames, aplica o juros e paga a divida
    while (divida > 0.0) {
        divida = divida * (1.0 + taxa);
        divida -= pagamento;

        meses++;

        //limite
        if (meses > 1000000)
            return -1;
    }

    return meses;
}

//valor presente VP = VF/(1+i)^t //PÚBLICA
double valor_antecipado(double parcela, double taxa, int dias) {
    double meses;

    if (parcela <= 0.0 ||
        taxa < 0.0 ||
        dias < 0) {
        return -1.0;
    }

    meses = dias / 30.0;

    return parcela / pow(1.0 + taxa, meses);
}

//valor futuro VF = VP x (1+i)^t //PÚBLICA
double valor_atrasado(double parcela, double taxa, int dias) {
    double meses;

    if (parcela <= 0.0 ||
        taxa < 0.0 ||
        dias < 0) {
        return -1.0;
    }

    meses = dias / 30.0;

    return parcela * pow(1.0 + taxa, meses);
}
