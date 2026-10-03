# Biblioteca Modular em C - Calculadora-Financeira

## Módulo Juros

Responsável por cálculos de juros sobre um valor aplicado uma única vez (sem
aportes recorrentes). Implementado em `src/Juros.c`, com interface pública
em `include/Juros.h`.

### Funções

- **`double juros_simples(double principal, double taxa, int periodos)`**
  Calcula o montante final usando juros simples: `montante = principal * (1 + taxa * periodos)`.

- **`double juros_compostos(double principal, double taxa, int periodos)`**
  Calcula o montante final usando juros compostos: `montante = principal * (1 + taxa)^periodos`.

- **`double juros_continuos(double principal, double taxa, double tempo)`**
  Calcula o montante final assumindo capitalização contínua (instantânea):
  `montante = principal * e^(taxa * tempo)`. Diferente das outras duas
  funções, aceita `tempo` fracionário, já que a capitalização não ocorre em
  períodos discretos.

Todas as funções retornam `-1.0` caso os parâmetros de entrada sejam
inválidos (ex.: `principal` ou `periodos`/`tempo` negativos).

### Dependência externa

O módulo utiliza as funções `pow()` e `exp()` da biblioteca padrão do C
(`math.h`). Por isso, **é necessário linkar com a biblioteca matemática ao
compilar**, usando a flag `-lm`:

```bash
gcc -Wall -Wextra -Iinclude tests/test_juros.c src/Juros.c -lm -o build/test_juros
```

Sem o `-lm`, a compilação falha com erro de referência indefinida a `pow`/`exp`.

### Testes

Os testes automatizados estão em `tests/test_juros.c` e cobrem casos válidos
e casos limite/erro (valores negativos, período/tempo igual a zero). Para
compilar e rodar:

```bash
gcc -Wall -Wextra -Iinclude tests/test_juros.c src/Juros.c -lm -o build/test_juros
./build/test_juros
```

Durante o desenvolvimento, a função `juros_compostos` foi implementada com um
erro de off-by-one no expoente (`periodos - 1` em vez de `periodos`), detectado
pelo teste automatizado e corrigido posteriormente (ver histórico do Git,
commits `49d418e` e `8871de2`).

## Módulo Financiamento

Responsável por cálculos de financiamento com parcelas mensais fixas e juros
compostos (sistema Price), com ou sem entrada e com ou sem juros. Implementado
em `src/financiamento.c`, com interface pública em `include/financiamento.h`.

### Tipos

- **`Simulacao`**: agrupa os resultados de `simular_fin` (`parcela`,
  `total_pago`, `total_juros` e `percentual`).
- **`Amortizacao`**: agrupa os resultados de `amortizacao` (`meses` e
  `total_juros`).

### Funções

- **`int simular_fin(double valor, double entrada, double taxa, int n, Simulacao *resultado)`**
  Simula um financiamento e calcula a parcela, o total pago, o total de juros e
  o percentual de juros sobre o valor financiado. Cobre os três casos: sem
  juros (`taxa = 0`), com juros e com entrada + juros.
  - `financiado = valor - entrada`
  - `parcela = financiado * taxa / (1 - (1 + taxa)^(-n))`, ou `financiado / n` se `taxa = 0`
  - `total_pago = entrada + parcela * n`
  - `total_juros = total_pago - valor`
  - `percentual = (total_juros / financiado) * 100`

- **`int valor_financiavel(double parcela, double taxa, int n, double *resultado)`**
  Faz o caminho inverso do PMT: dada a maior parcela que cabe no orçamento,
  calcula o maior valor que pode ser financiado:
  `financiado = parcela * (1 - (1 + taxa)^(-n)) / taxa`, ou `parcela * n` se `taxa = 0`.

- **`int amortizacao(double divida, double parcela, double extra, double taxa, Amortizacao *resultado)`**
  Simula o financiamento mês a mês pagando a parcela mais um valor extra. A
  cada mês os juros incidem sobre o saldo devedor (`juros = saldo * taxa`) e o
  pagamento total é descontado, até a dívida ser quitada. Devolve em quantos
  meses isso acontece e o total de juros pago, o que permite comparar com
  `extra = 0` e ver a economia.

Todas as funções retornam `0` em caso de sucesso e `-1` caso os parâmetros de
entrada sejam inválidos. Em `amortizacao`, `-1` também é devolvido quando 
`parcela + extra` não cobre os juros do primeiro mês (`divida * taxa`), 
pois a dívida nunca seria quitada. Em caso de erro, a struct (ou variável) 
de resultado não é modificada.

A `taxa` é sempre mensal, em decimal (2% = `0.02`), e `n` é o número de
parcelas mensais.

### Dependência externa

O módulo utiliza a função `pow()` da biblioteca padrão do C (`math.h`). Por
isso, **é necessário linkar com a biblioteca matemática ao compilar**, usando
a flag `-lm`:

```bash
gcc -Wall -Wextra -Iinclude tests/test_financiamento.c src/financiamento.c -lm -o build/test_financiamento
```

Sem o `-lm`, a compilação falha com erro de referência indefinida a `pow`.

### Testes

Os testes automatizados estão em `tests/test_financiamento.c` e cobrem os três
casos do enunciado (sem juros, com juros e entrada + juros), o inverso do PMT,
a amortização com e sem extra e casos de erro (prazo zero, entrada igual ao
valor, parcela zero e dívida que nunca quita). Para compilar e rodar:

```bash
gcc -Wall -Wextra -Iinclude tests/test_financiamento.c src/financiamento.c -lm -o build/test_financiamento
./build/test_financiamento
```

### Documentação

Os comentários do cabeçalho seguem o formato Doxygen. Para gerar a
documentação em HTML (requer o Doxygen instalado), rode `doxygen Doxyfile` e
abra `html/index.html`.