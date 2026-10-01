#ifndef METAS_H
#define METAS_H
#define META_OK                  0
#define META_ERRO_PARAMETRO     -1
#define META_ERRO_INALCANCAVEL  -2
#define META_ERRO_PONTEIRO_NULO -3
/**
 * @brief Calcula o percentual da meta que já foi atingido.
 *
 * A fórmula é: percentual = (valor_atual / meta) * 100. 
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
 * Se a taxa for 0, o aporte é meta / periodos.

 *
 * @param meta O valor que se deseja acumular. (Deve ser maior que 0)
 * @param taxa A taxa de juros mensal (em decimal). Por exemplo, 0.01 para 1%. (Deve ser maior ou igual a 0)
 * @param periodos O prazo, em meses. (Deve ser maior que 0)
 * @return O valor a depositar por mês. Retorna -1.0 se algum dos parâmetros for inválido.
 */
double meta_aporte_mensal(double meta, double taxa, int periodos);

/**
 * @brief Calcula quantos meses são necessários para atingir a meta.
 *
 * Considera um saldo inicial e depósitos no fim de cada mês, com juros compostos.
 * Se o valor atual já for maior ou igual à meta, o prazo é 0.
 * O resultado pode ser fracionário (por exemplo, 47.3 meses).
 *
 * @param meta O valor que se deseja alcançar. (Deve ser maior que 0)
 * @param valor_atual O valor já acumulado. (Deve ser maior ou igual a 0)
 * @param aporte_mensal O valor depositado por mês. (Deve ser maior ou igual a 0)
 * @param taxa_mensal A taxa de juros mensal (em decimal). Por exemplo, 0.01 para 1%.
 * @param meses Ponteiro onde o prazo, em meses, será gravado. Não pode ser NULL.
 * Em caso de erro, o conteúdo apontado não é modificado.
 * @return META_OK em caso de sucesso;
 * META_ERRO_PARAMETRO se algum parâmetro numérico for inválido;
 * META_ERRO_INALCANCAVEL se a meta nunca puder ser atingida;
 * META_ERRO_PONTEIRO_NULO se meses for NULL.
 */
int meta_prazo_necessario(double meta, double valor_atual, double aporte_mensal,
                          double taxa_mensal, double *meses);

#endif