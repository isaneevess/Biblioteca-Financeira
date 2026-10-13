
# --- Compilador e flags ---
CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -fPIC -Iinclude
LDLIBS   := -lm
# Gera arquivos .d com as dependencias de headers (compilacao incremental)
DEPFLAGS := -MMD -MP

# --- Instalacao (sobrescrevivel: make install PREFIX=/outro/caminho) ---
PREFIX ?= $(HOME)/.local

# --- Diretorios ---
SRC_DIR   := src
INC_DIR   := include
TEST_DIR  := tests
APP_DIR   := app
BUILD_DIR := build
LIB_DIR   := lib
BIN_DIR   := bin

# --- Bibliotecas ---
LIB_NAME   := financeira
STATIC_LIB := $(LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB := $(LIB_DIR)/lib$(LIB_NAME).so

# --- Fontes e objetos (descobertos automaticamente com wildcard) ---
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# --- App cliente e testes (usados pelos Blocos 2 e 3) ---
# dependencias, e o make expande essa lista assim que le a regra: a variavel
APP_SRC     := $(APP_DIR)/main.c
APP_DYNAMIC := $(BIN_DIR)/app_dinamico
APP_STATIC  := $(BIN_DIR)/app_estatico
TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.c)
TEST_BINS := $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SRCS))

# Alvo padrao: o que roda com um simples 'make'
.DEFAULT_GOAL := all

# --- Regra generica: src/X.c -> build/X.o ---
# O '|' indica pre-requisito "order-only": a pasta build/ precisa existir,
# mas mudar a data dela nao forca recompilar os objetos.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# --- Dependencias de headers geradas pelo -MMD -MP ---
# O '-' faz o make ignorar os .d que ainda nao existem (primeira compilacao).
-include $(DEPS)

# -------------------------------------------------------------------
# Bloco 2 - Bibliotecas estatica (.a) e dinamica (.so)
# -------------------------------------------------------------------
# Estatica: 'ar rcs' empacota os .o num arquivo .a. O codigo da
# biblioteca fica copiado dentro de qualquer executavel que linkar
# com ela (standalone, nao precisa do .a depois de compilado).
#
# Dinamica: 'gcc -shared' gera um .so a partir dos mesmos .o.
# O codigo NAO e copiado — fica carregado em tempo de
# execucao, e por isso o alvo 'run' precisa do LD_LIBRARY_PATH.

.PHONY: all run run-static test clean install

all: $(STATIC_LIB) $(SHARED_LIB) $(APP_DYNAMIC) $(APP_STATIC)

$(STATIC_LIB): $(OBJS) | $(LIB_DIR)
	ar rcs $@ $(OBJS)

$(SHARED_LIB): $(OBJS) | $(LIB_DIR)
	$(CC) -shared -o $@ $(OBJS) $(LDLIBS)

$(BUILD_DIR) $(LIB_DIR) $(BIN_DIR):
	mkdir -p $@

# ===================================================================
# Bloco 3 — App cliente, execucao e testes
# ===================================================================



# -------------------------------------------------------------------
# comando install
# -------------------------------------------------------------------
install: all
	mkdir -p $(PREFIX)/include
	mkdir -p $(PREFIX)/lib
	cp $(INC_DIR)/*.h $(PREFIX)/include/
	cp $(STATIC_LIB) $(PREFIX)/lib/
	cp $(SHARED_LIB) $(PREFIX)/lib/


# -------------------------------------------------------------------
# comando clean
# -------------------------------------------------------------------
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(BIN_DIR)
	
# -------------------------------------------------------------------
# App cliente
# -------------------------------------------------------------------
# Linka o app com a biblioteca DINAMICA (.so).
# Depende de $(SHARED_LIB): so compila depois que o Bloco 2 ja gerou a
# biblioteca dinamica.
$(APP_DYNAMIC): $(APP_SRC) $(SHARED_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(APP_SRC) -L$(LIB_DIR) -l$(LIB_NAME) $(LDLIBS) -o $@

# Linka o app com a biblioteca ESTATICA (.a).
# O codigo da biblioteca e incorporado ao executavel durante a linkagem.
$(APP_STATIC): $(APP_SRC) $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(APP_SRC) $(STATIC_LIB) $(LDLIBS) -o $@

# Executa o app cliente com a biblioteca dinamica.
run: $(APP_DYNAMIC)
	LD_LIBRARY_PATH=$(LIB_DIR):$$LD_LIBRARY_PATH ./$(APP_DYNAMIC)

# Executa o app cliente com a biblioteca estatica.
run-static: $(APP_STATIC)
	./$(APP_STATIC)

# -------------------------------------------------------------------
# Testes automatizados — um binario por módulo, nomeado explicitamente
# -------------------------------------------------------------------
# Cada teste é compilado como um executavel separado, linkado com TODOS
# os objetos da biblioteca (não só com o módulo que ele testa) porque
# os arquivos de teste usam apenas as funções da API pública (headers),
# então basta ter os simbolos disponiveis no link.

# Modulo: juros
$(BUILD_DIR)/test_juros: $(TEST_DIR)/test_juros.c $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDLIBS) -o $@

# Modulo: financiamento
$(BUILD_DIR)/test_financiamento: $(TEST_DIR)/test_financiamento.c $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDLIBS) -o $@

# Modulo: metas financeiras
$(BUILD_DIR)/test_metas: $(TEST_DIR)/test_metas.c $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDLIBS) -o $@

# Modulo: planejamento financeiro / tempo
$(BUILD_DIR)/test_tempo: $(TEST_DIR)/test_tempo.c $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(OBJS) $(LDLIBS) -o $@

# Roda os quatro modulos, um de cada vez. O '|| status=1' registra que
# aquele modulo falhou SEM interromper os demais — assim a suite
# inteira roda até o fim mesmo que um teste quebre no meio, e o 'exit'
# no final garante codigo de saida != 0 se qualquer um tiver falhado.
test: $(BUILD_DIR)/test_juros $(BUILD_DIR)/test_financiamento $(BUILD_DIR)/test_metas $(BUILD_DIR)/test_tempo
	@status=0; \
	echo "== Modulo: Juros =="; \
	./$(BUILD_DIR)/test_juros || status=1; \
	echo ""; \
	echo "== Modulo: Financiamento =="; \
	./$(BUILD_DIR)/test_financiamento || status=1; \
	echo ""; \
	echo "== Modulo: Metas financeiras =="; \
	./$(BUILD_DIR)/test_metas || status=1; \
	echo ""; \
	echo "== Modulo: Planejamento financeiro (tempo) =="; \
	./$(BUILD_DIR)/test_tempo || status=1; \
	echo ""; \
	exit $$status

doc:
	doxygen Doxyfile

clean-doc:
	rm -rf docs/html 