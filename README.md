# Biblioteca Modular em C - Calculadora-Financeira

## Integrantes: 
Isabela Neves Gonella

Jorge Carneiro Silva Júnior

Maria Júlia Costa de Lima

Millena Nunes Cupolillo

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

### Tratamento de erros

`juros_simples`, `juros_compostos` e `juros_continuos` retornam um código
de erro no lugar do montante, em caso de parâmetro inválido:

| Código                          | Valor | Situação                                  |
|----------------------------------|-------|--------------------------------------------|
| `JUROS_ERRO_PRINCIPAL_INVALIDO`  | -1.0  | `principal < 0`                             |
| `JUROS_ERRO_TAXA_INVALIDA`       | -2.0  | `taxa < -1.0` (não se aplica a juros_continuos) |
| `JUROS_ERRO_PERIODO_INVALIDO`    | -3.0  | `periodos < 0` (juros_simples/compostos)    |
| `JUROS_ERRO_TEMPO_INVALIDO`      | -3.0  | `tempo < 0` (juros_continuos)               |

O limite mínimo de `taxa` (-1.0) existe porque uma taxa menor que isso
tornaria o fator `(1 + taxa)` negativo, podendo gerar um montante negativo
que se confundiria com um código de erro. `juros_continuos` não restringe o
sinal de `taxa` pelo mesmo motivo contrário: como usa `exp()`, que é sempre
positiva, o resultado nunca fica negativo mesmo com taxa negativa (ex:
desvalorização).

A taxa é sempre decimal (1% = `0.01`). Em `juros_simples`/`juros_compostos`,
`periodos` é a quantidade de períodos discretos (inteiro); em
`juros_continuos`, `tempo` pode ser fracionário.

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
- **`Amortizacao`**: agrupa os resultados de `amortizacao` (`parcelas` e
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

- **`int amortizacao(double divida, double valor_parcela, double extra, double taxa, Amortizacao *resultado)`**
  Simula o financiamento mês a mês pagando a parcela mais um valor extra. A
  cada mês os juros incidem sobre o saldo devedor (`juros = saldo * taxa`) e o
  pagamento total é descontado, até a dívida ser quitada. Devolve em quantas
  parcelas isso acontece e o total de juros pago, o que permite comparar com
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
## Módulo Metas

Responsável por cálculos de planejamento de metas financeiras: quanto da meta
já foi atingido, quanto depositar por mês para alcançá-la e em quanto tempo
ela é atingida. Considera um saldo inicial e depósitos mensais feitos no fim de
cada mês, com capitalização composta. Implementado em `src/metas.c`, com
interface pública em `include/metas.h`.

### Funções

- **`double meta_percentual_atingido(double meta, double valor_atual)`**
  Calcula o percentual da meta já atingido: `percentual = (valor_atual / meta) * 100`.
  O resultado pode ultrapassar `100.0` quando o valor atual supera a meta.

- **`double meta_aporte_mensal(double meta, double valor_atual, double taxa, int periodos)`**
  Calcula quanto depositar por mês, considerando um saldo inicial, para atingir
  a meta em `periodos` meses. O saldo inicial também rende juros, então reduz o
  valor que falta juntar:
  `aporte = (meta - valor_atual * (1 + taxa)^periodos) * taxa / ((1 + taxa)^periodos - 1)`,
  ou `(meta - valor_atual) / periodos` se `taxa = 0`. Se o saldo inicial, com seus
  rendimentos, já atingir a meta, o aporte é `0`. Use `valor_atual = 0` para
  começar do zero.

- **`int meta_prazo_necessario(double meta, double valor_atual, double aporte_mensal, double taxa_mensal, double *meses)`**
  Calcula em quantos meses a meta é atingida, dado um saldo inicial e um
  aporte mensal:
  `meses = ln((meta * taxa + aporte) / (valor_atual * taxa + aporte)) / ln(1 + taxa)`,
  ou `(meta - valor_atual) / aporte` se `taxa = 0`. Se o valor atual já for
  maior ou igual à meta, o prazo é `0`. O resultado pode ser fracionário
  (por exemplo, 47.3 meses).

As duas últimas funções são coerentes entre si: o aporte calculado por
`meta_aporte_mensal` para um prazo de N meses, quando passado para
`meta_prazo_necessario`, resulta em N meses.

### Tratamento de erros

`meta_percentual_atingido` e `meta_aporte_mensal` retornam `-1.0` caso os
parâmetros de entrada sejam inválidos (`meta <= 0`, `valor_atual < 0`,
`taxa < 0` ou `periodos <= 0`). Valores negativos de `valor_atual` são
rejeitados porque o módulo trata esse valor como saldo acumulado.

`meta_prazo_necessario` retorna um código de status e grava o prazo em
`*meses` apenas em caso de sucesso:

| Código | Valor | Situação |
|---|---|---|
| `META_OK` | `0` | Cálculo concluído com sucesso |
| `META_ERRO_PARAMETRO` | `-1` | `meta <= 0`, `valor_atual < 0`, `aporte_mensal < 0` ou `taxa_mensal < 0` |
| `META_ERRO_INALCANCAVEL` | `-2` | Sem aporte e sem rendimento sobre o saldo, então a meta nunca é atingida |
| `META_ERRO_PONTEIRO_NULO` | `-3` | `meses` é `NULL` |

Em caso de erro, o conteúdo apontado por `meses` não é modificado.

A `taxa` é sempre mensal, em decimal (1% = `0.01`), e `periodos` é o número de
meses.

### Dependência de outros módulos

O módulo Metas depende do módulo Juros: `meta_aporte_mensal` obtém o fator de
capitalização `(1 + taxa)^periodos` chamando `juros_compostos(1.0, taxa, periodos)`
em vez de repetir o cálculo. A dependência é interna, pois `include/Juros.h`
é incluído apenas em `src/metas.c`, e a interface pública de `metas.h` não
depende dele.

### Dependência externa

O módulo utiliza a função `log()` da biblioteca padrão do C (`math.h`) e,
indiretamente, `pow()` através do módulo Juros. Por isso, **é necessário
linkar com a biblioteca matemática ao compilar**, usando a flag `-lm`, e
incluir `src/Juros.c` na compilação:

```bash
gcc -Wall -Wextra -Iinclude tests/test_metas.c src/metas.c src/Juros.c -lm -o build/test_metas
```

Sem o `-lm`, a compilação falha com erro de referência indefinida a `log`/`pow`.
Sem `src/Juros.c`, falha com referência indefinida a `juros_compostos`.

### Testes

Os testes automatizados estão em `tests/test_metas.c` e cobrem casos válidos
(percentual abaixo, igual e acima da meta; aporte do zero e com saldo inicial,
com e sem juros; prazo com aporte, sem aporte e com meta já atingida), a
consistência entre aporte e prazo, e casos limite/erro (meta zero ou negativa,
valores negativos, prazo zero, taxa zero, meta inalcançável e ponteiro nulo).
Para compilar e rodar:

```bash
gcc -Wall -Wextra -Iinclude tests/test_metas.c src/metas.c src/Juros.c -lm -o build/test_metas
./build/test_metas
```


## Módulo Tempo

O módulo **Tempo** reúne funções voltadas ao planejamento financeiro, permitindo calcular intervalos entre datas, estimar o prazo necessário para quitar uma dívida e determinar valores de parcelas em situações de antecipação ou atraso de pagamentos.

### Funcionalidades

- **`dias_entre_datas(Data inicio, Data fim)`**: calcula a quantidade de dias entre duas datas. A função valida as datas informadas e retorna `-1` caso alguma delas seja inválida.

- **`meses_para_quitar(double divida, double pagamento, double taxa)`**: estima a quantidade de meses necessária para quitar uma dívida a partir do valor inicial, do pagamento mensal e da taxa de juros mensal. O cálculo considera a incidência de juros sobre o saldo devedor e a realização de pagamentos fixos. Retorna `-1` quando os parâmetros são inválidos ou quando o pagamento não é suficiente para cobrir os juros da dívida.

- **`valor_antecipado(double parcela, double taxa, int dias)`**: calcula o valor de uma parcela paga antecipadamente, aplicando o desconto correspondente ao período de antecipação.

- **`valor_atrasado(double parcela, double taxa, int dias)`**: calcula o valor atualizado de uma parcela paga com atraso, considerando os juros incidentes durante o período.

### Convenções de cálculo

As taxas de juros são informadas em formato decimal e representam taxas mensais. Por exemplo, uma taxa de `0.02` corresponde a 2% ao mês. Nas funções de antecipação e atraso, o período é convertido utilizando o mês comercial de 30 dias.

As funções de antecipação e atraso utilizam capitalização composta para atualizar o valor da parcela. Para a antecipação, aplica-se o desconto sobre o valor futuro; para o atraso, calcula-se o acréscimo correspondente ao período transcorrido.

### Exemplos de uso

```c
Data inicio = {1, 10, 2026};
Data fim = {15, 10, 2026};

int dias = dias_entre_datas(inicio, fim);
// Resultado: 14 dias

int meses = meses_para_quitar(1000.0, 300.0, 0.02);
// Resultado: 4 meses

double antecipado = valor_antecipado(1000.0, 0.02, 30);
// Resultado aproximado: R$ 980,39

double atrasado = valor_atrasado(1000.0, 0.02, 30);
// Resultado: R$ 1.020,00
```

### Testes

Os testes automatizados do módulo estão localizados em `tests/test_tempo.c` e verificam o cálculo de intervalos entre datas, a estimativa do prazo de quitação de dívidas e os cálculos de antecipação e atraso, incluindo situações com parâmetros inválidos.

Para executar os testes do módulo junto aos demais testes da biblioteca, utilize:

```bash
make test
```


## Documentação

Os comentários do cabeçalho seguem o formato Doxygen. Para gerar a
documentação em HTML (requer o Doxygen instalado), rode `doxygen Doxyfile` e
abra `html/index.html`.
