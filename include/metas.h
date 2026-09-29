#ifndef METAS_H
#define METAS_H

/**
 * @brief Calcula o percentual da meta que já foi atingido.
 *
 * A fórmula é: percentual = (valor_atual / meta) * 100
 *
 * @param meta O valor que se deseja alcançar. (Deve ser maior que 0)
 * @param valor_atual O valor já acumulado. (Deve ser maior ou igual a 0)
 * @return O percentual atingido (por exemplo, 50.0 para metade da meta).
 * Retorna -1.0 se algum dos parâmetros for inválido.
 */
double meta_percentual_atingido(double meta, double valor_atual);

/**
 * @brief Calcula quanto é preciso depositar por mês para atingir a meta.
 *
 * Considera depósitos no fim de cada mês, com juros compostos.
 * A fórmula é: aporte = meta * taxa / ((1 + taxa) ^ periodos - 1)
 *
 * @param meta O valor que se deseja acumular. (Deve ser maior que 0)
 * @param taxa A taxa de juros mensal (em decimal). Por exemplo, 0.01 para 1%.
 * @param periodos O prazo, em meses. (Deve ser maior que 0)
 * @return O valor a depositar por mês. Retorna -1.0 se algum dos parâmetros for inválido.
 */
double meta_aporte_mensal(double meta, double taxa, int periodos);

/**
 * @brief Calcula quantos meses são necessários para atingir a meta.
 *
 * Considera um saldo inicial e depósitos no fim de cada mês, com juros compostos.
 *
 * @param meta O valor que se deseja alcançar. (Deve ser maior que 0)
 * @param valor_atual O valor já acumulado. (Deve ser maior ou igual a 0)
 * @param aporte_mensal O valor depositado por mês. (Deve ser maior ou igual a 0)
 * @param taxa_mensal A taxa de juros mensal (em decimal). Por exemplo, 0.01 para 1%.
 * @param meses Ponteiro onde o prazo, em meses, será gravado.
 * @return 0 em caso de sucesso, ou um valor diferente de 0 se algum parâmetro for
 * inválido ou se a meta não puder ser atingida.
 */
int meta_prazo_necessario(double meta, double valor_atual, double aporte_mensal,
                          double taxa_mensal, double *meses);

#endif