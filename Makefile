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
APP_SRC   := $(APP_DIR)/main.c
APP_BIN   := $(BIN_DIR)/app
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

.PHONY: all

all: $(STATIC_LIB) $(SHARED_LIB) $(APP_BIN)

$(STATIC_LIB): $(OBJS) | $(LIB_DIR)
	ar rcs $@ $(OBJS)

$(SHARED_LIB): $(OBJS) | $(LIB_DIR)
	$(CC) -shared -o $@ $(OBJS) $(LDLIBS)

$(BUILD_DIR) $(LIB_DIR) $(BIN_DIR):
	mkdir -p $@