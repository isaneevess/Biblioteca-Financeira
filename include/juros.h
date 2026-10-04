#ifndef JUROS_H
#define JUROS_H

/**
 * @brief Calcula o montante de um investimento ou empréstimo usando juros simples.
 * 
 * A fórmula para calcular o montante com juros simples é: montante = principal * (1 + taxa * periodos)
 * 
 * @param principal O valor inicial do investimento ou empréstimo.(Deve ser maior ou igual a 0)
 * @param taxa A taxa de juros (em decimal) por período. Por exemplo,0.05 para 5%.
 * @param periodos O número de períodos de tempo (deve ser maior ou igual a 0).
 * @return O montante total após aplicar os juros simples. Retorna -1.0 se algum dos parâmetros for inválido.
 */
double juros_simples(double principal, double taxa, int periodos);

/**
 * @brief Calcula o montante de um investimento ou empréstimo usando juros compostos.
 * 
 * A fórmula para calcular o montante com juros compostos é: montante = principal * (1 + taxa) ^ periodos
 * 
 * @param principal O valor inicial do investimento ou empréstimo.(Deve ser maior ou igual a 0)
 * @param taxa A taxa de juros (em decimal) por período. Por exemplo,0.05 para 5%.
 * @param periodos O número de períodos de tempo (deve ser maior ou igual a 0).
 * @return O montante total após aplicar os juros compostos. Retorna -1.0 se algum dos parâmetros for inválido.
 */
double juros_compostos(double principal, double taxa, int periodos);

/**
 * @brief Calcula o montante de um investimento ou empréstimo usando juros contínuos.
 * 
 * Diferente dos juros simples e compostos, os juros contínuos são calculados de forma contínua ao longo do tempo.
 * A fórmula para calcular o montante com juros contínuos é: montante = principal * e^(taxa * tempo).
 * 
 * @param principal O valor inicial do investimento ou empréstimo.(Deve ser maior ou igual a 0)
 * @param taxa A taxa de juros contínua (em decimal). Por exemplo,0.05 para 5%.
 * @param tempo Tempo decorrido, em número de períodos (pode ser fracionário, pois a capitalização é contínua, 
 * por exemplo, 1.5 para um período e meio).
 * @return O montante final após aplicar os juros contínuos. Retorna -1.0 se algum dos parâmetros for inválido.
 */
double juros_continuos(double principal, double taxa, double tempo);

#endif 
