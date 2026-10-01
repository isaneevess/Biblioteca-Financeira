# Trabalho-Pratico-2---Biblioteca-Modular-em-C---Calculadora-Financeira

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
