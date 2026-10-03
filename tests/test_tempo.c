#include <stdio.h>
#include <math.h>
#include "Tempo.h"

static int total_testes = 0;
static int testes_falhos = 0;


static int quase_igual(double a, double b) {
    const double k = 0.01;
    return fabs(a - b) < k;
} //função para margem de erro

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

    printf("Executando testes do modulo Tempo...\n\n");

    //dias_entre_datas()
    //teste da diferença entre datas
    verificar(
        "dias_entre_datas com datas validas",
        dias_entre_datas(
            (Data){1, 10, 2026},
            (Data){15, 10, 2026}
        ) == 14
    );
    //caso ano bissexto
    verificar(
        "dias_entre_datas com ano bissexto",
        dias_entre_datas(
            (Data){28, 2, 2028},
            (Data){1, 3, 2028}
        ) == 2
    );
    //datas iguais
    verificar(
        "dias_entre_datas com datas iguais",
        dias_entre_datas(
            (Data){10, 10, 2026},
            (Data){10, 10, 2026}
        ) == 0
    );
    //data inválida
    verificar(
        "dias_entre_datas com data invalida",
        dias_entre_datas(
            (Data){31, 2, 2026},
            (Data){1, 3, 2026}
        ) == -1
    );

    //meses_para_quitar()
    //divida sem juros
    verificar(
        "meses_para_quitar sem juros",
        meses_para_quitar(1000.0, 500.0, 0.0) == 2
    );

    //com juros
    verificar(
        "meses_para_quitar com juros",
        meses_para_quitar(1000.0, 600.0, 0.10) == 2
    );

    //pagamento não cobre a dívida
    verificar(
        "meses_para_quitar com pagamento insuficiente",
        meses_para_quitar(1000.0, 50.0, 0.10) == -1
    );

    //valor_antecipado()
    //cálculo padrão
    verificar(
        "valor_antecipado com valores validos",
        quase_igual(
            valor_antecipado(1000.0, 0.01, 30),
            990.099
        )
    );

    //caso sem antecipação
    verificar(
        "valor_antecipado com zero dias",
        quase_igual(
            valor_antecipado(1000.0, 0.01, 0),
            1000.0
        )
    );

    //entrada inválida
    verificar(
        "valor_antecipado com taxa negativa",
        valor_antecipado(1000.0, -0.01, 30) == -1.0
    );



    //valor_atrasado()
    //calculo padrão
    verificar(
        "valor_atrasado com valores validos",
        quase_igual(
            valor_atrasado(1000.0, 0.01, 30),
            1010.0
        )
    );

    //entrada inválida
    verificar(
        "valor_atrasado com parcela negativa",
        valor_atrasado(-1000.0, 0.01, 30) == -1.0
    );






    printf(
        "\n%d de %d testes passaram.\n",
        total_testes - testes_falhos,
        total_testes
    );

    return testes_falhos > 0 ? 1 : 0;
}