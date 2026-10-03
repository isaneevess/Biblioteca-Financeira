#ifndef TEMPO.H
#define TEMPO.H

/**
 * @brief Calcula a quantidade de dias entre duas datas.
 *
 * A função verifica se as duas datas são válidas e calcula
 * a diferença absoluta entre elas em dias.
 *
 * @param inicio A data inicial.
 * @param fim A data final.
 *
 * @return A quantidade de dias entre as duas datas.
 * Retorna -1 caso alguma das datas seja inválida.
 */
int dias_entre_datas(Data inicio, Data fim);

/**
 * @brief Calcula em quantos meses uma dívida será quitada.
 *
 * A função considera uma dívida com juros compostos mensais
 * e pagamentos fixos realizados ao final de cada mês.
 *
 * @param divida O valor inicial da dívida.
 * Deve ser maior que 0.
 * @param pagamento O valor pago mensalmente.
 * Deve ser maior que 0.
 * @param taxa A taxa de juros mensal em decimal.
 * Deve ser maior ou igual a 0.
 *
 * @return A quantidade de meses necessária para quitar a dívida.
 * Retorna -1 caso algum parâmetro seja inválido ou o pagamento
 * não seja suficiente para reduzir a dívida.
 */
int meses_para_quitar(double divida, double pagamento, double taxa);

/**
 * @brief Calcula o valor presente de uma parcela antecipada.
 *
 * O valor presente é calculado utilizando juros compostos.
 * Os dias de antecipação são convertidos para meses considerando
 * um mês comercial de 30 dias.
 *
 * @param parcela O valor original da parcela.
 * Deve ser maior que 0.
 * @param taxa A taxa de juros mensal em decimal.
 * Deve ser maior ou igual a 0.
 * @param dias A quantidade de dias de antecipação.
 * Deve ser maior ou igual a 0.
 *
 * @return O valor presente da parcela após o desconto pela antecipação.
 * Retorna -1 caso algum parâmetro seja inválido.
 */
double valor_antecipado(double parcela, double taxa, int dias);

/**
 * @brief Calcula o valor atualizado de uma parcela atrasada.
 *
 * O valor da parcela é atualizado utilizando juros compostos.
 * Os dias de atraso são convertidos para meses considerando
 * um mês comercial de 30 dias.
 *
 * @param parcela O valor original da parcela.
 * Deve ser maior que 0.
 * @param taxa A taxa de juros mensal em decimal.
 * Deve ser maior ou igual a 0.
 * @param dias A quantidade de dias de atraso.
 * Deve ser maior ou igual a 0.
 *
 * @return O valor atualizado da parcela após o atraso.
 * Retorna -1 caso algum parâmetro seja inválido.
 */
double valor_atrasado(double parcela, double taxa, int dias);

#endif
