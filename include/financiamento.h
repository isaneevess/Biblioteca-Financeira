/**
 * @file financiamento.h
 * @brief Funções para simulação e análise de financiamentos.
 */

#ifndef FINANCIAMENTO_H 
#define FINANCIAMENTO_H

/**
 * @brief Resultados de uma simulação de financiamento.
 */
typedef struct {
    double parcela; /**< Valor de cada parcela mensal. */
    double total_pago; /**< Entrada + soma de todas as parcelas. */
    double total_juros; /**< Total pago menos o valor do bem. */
    double percentual; /**< Juros sobre o valor financiado, em %. */
} Simulacao;

/**
 * @brief Simula um financiamento com juros compostos.
 * 
 * Calcula o valor da parcela, o total pago, o total de juros pagos e o percentual de juros sobre o valor financiado.
 * 
 * A fórmula para calcular o valor financiado é: financiado = valor - entrada
 * 
 * A fórmula para calcular a parcela com juros é: parcela = financiado * taxa / (1 - (1 + taxa)^-n)
 * 
 * A fórmula para calcular a parcela sem juros é: parcela = financiado / n
 * 
 * A fórmula para calcular o total pago é total_pago = entrada + parcela * n
 * 
 * A fórmula para calcular o total de juros é total_juros = total_pago - valor
 * 
 * A fórmula para calcular o percentual de juros é percentual = (total_juros / financiado) * 100
 * 
 * @param valor O preço total daquilo que será financiado (deve ser maior que 0).
 * @param entrada Parte do valor que será paga à vista (deve ser maior ou igual a 0 e menor que o valor).
 * @param taxa Taxa de juros mensal, em decimal (Ex.: 0.02 ao mês; deve ser maior ou igual a 0).
 * @param n Número de parcelas mensais (deve ser maior que 0).
 * @param resultado Ponteiro para a struct onde serão gravados a parcela, o total pago, o total de juros e o percentual de juros.
 * @return 0 em caso de sucesso, -1 se algum parâmetro for inválido.
 */
int simular_fin(double valor, double entrada, double taxa, int n, Simulacao *resultado);


/**
 * @brief Calcula o maior valor que pode ser financiado com uma dada parcela.
 *
 * A fórmula para calcular o maior valor possível com juros é: financiado = parcela * (1 - (1 + taxa)^-n) / taxa
 * 
 * A fórmula para calcular o maior valor possível sem juros é: financiado = parcela * n
 * 
 * @param parcela A maior parcela possível de pagar por mês (deve ser maior que 0).
 * @param taxa Taxa de juros mensal, em decimal (Ex.: 0.02 ao mês; deve ser maior ou igual a 0).
 * @param n Número de parcelas mensais (deve ser maior que 0).
 * @param resultado Ponteiro para um double onde será gravado o maior valor possível a ser financiado.
 * @return 0 em caso de sucesso, -1 se algum parâmetro for inválido.
 */
int valor_financiavel(double parcela, double taxa, int n, double *resultado);


 /**
 * @brief Resultados de uma amortização.
 */
typedef struct {
    int parcelas; /**< Tempo para quitar a dívida. */
    double total_juros; /**< Quanto de juros foi pago. */
} Amortizacao;

/**
 * @brief Simula o financiamento mês a mês, pagando a parcela normal mais um valor extra. 
 * 
 * A cada mês, calcula os juros sobre o saldo devedor, desconta o pagamento total e repete até a dívida zerar.
 * 
 * No final, devolve em quantas parcelas o financiamento termina e quanto de juros foi pago no total.
 * 
 * @param divida O valor a ser financiado (deve ser maior que 0).
 * @param valor_parcela O valor da parcela mensal (deve ser maior que 0).
 * @param extra O valor extra a ser pago além da parcela (deve ser maior ou igual a 0).
 * @param taxa Taxa de juros mensal, em decimal (Ex.: 0.02 ao mês; deve ser maior ou igual a 0).
 * @param resultado Ponteiro para a struct onde serão gravados em quantas parcelas o financiamento termina e quanto de juros foi pago no total.
 * @return 0 em caso de sucesso, -1 se algum parâmetro for inválido ou se o pagamento mensal não cobrir os juros (a dívida nunca seria quitada).
 */
int amortizacao(double divida, double valor_parcela, double extra, double taxa, Amortizacao *resultado);

#endif
