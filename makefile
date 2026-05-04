# =============================================================================
# Makefile — Projeto TED (GIS Bitnópolis)
# Disciplina: Estrutura de Dados II — UEL 2026
# Aluno: Nathan Felipe Caroba dos Santos
#
# Estrutura de diretórios:
#   include/ — arquivos de cabeçalho (.h)
#   src/     — arquivos-fonte (.c) e executável ted
#   tst/     — testes unitários (t_modulo.c para cada módulo)
#   unity/   — framework Unity (unity.c, unity.h, unity_internals.h)
#   makefile — este arquivo (raiz do repositório)
#
# Targets principais:
#   make ted           — compila o executável ted dentro de src/
#   make tst_hashfile  — compila e executa testes do módulo hashfile
#   make tst_quadra    — compila e executa testes do módulo quadra
#   make tst_habitante — compila e executa testes do módulo habitante
#   make tst_txt       — compila e executa testes do módulo txt
#   make tst_svg       — compila e executa testes do módulo svg
#   make tst_geo       — compila e executa testes do módulo geo
#   make tst_pm        — compila e executa testes do módulo pm
#   make tst_qry       — compila e executa testes do módulo qry
#   make tst_arquivos  — compila e executa testes do módulo arquivos
#   make tstall        — compila e executa TODOS os testes unitários
#   make clean         — remove todos os arquivos gerados
# =============================================================================

# -----------------------------------------------------------------------------
# Configuração geral
# -----------------------------------------------------------------------------

PROJ_NAME  = ted
NOME_ALUNO = Nathan Felipe Caroba dos Santos

CC     = gcc
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb \
         -Werror=implicit-function-declaration
LIBS   = -lm

# -----------------------------------------------------------------------------
# Diretórios
# -----------------------------------------------------------------------------

INC_DIR   = include
SRC_DIR   = src
TST_DIR   = tst
UNITY_DIR = unity

# -----------------------------------------------------------------------------
# Módulos da aplicação (um .c por módulo, sem ted.c)
# -----------------------------------------------------------------------------

MODULOS = \
	hashfile  \
	quadra    \
	habitante \
	svg       \
	txt       \
	geo       \
	pm        \
	qry       \
	arquivos

# Objetos dos módulos (gerados em src/)
MOD_OBJS = $(addprefix $(SRC_DIR)/, $(addsuffix .o, $(MODULOS)))

# Objeto principal do programa
TED_OBJ = $(SRC_DIR)/ted.o

# Todos os objetos do executável final
OBJS = $(MOD_OBJS) $(TED_OBJ)

# Executável produzido dentro de src/ conforme exigido pelo enunciado
TED_BIN = $(SRC_DIR)/$(PROJ_NAME)

# -----------------------------------------------------------------------------
# Unity
# -----------------------------------------------------------------------------

UNITY_OBJ = $(UNITY_DIR)/unity.o

# -----------------------------------------------------------------------------
# Regra padrão
# -----------------------------------------------------------------------------

.PHONY: all
all: ted

# -----------------------------------------------------------------------------
# Executável ted
# -----------------------------------------------------------------------------

.PHONY: ted
ted: $(TED_BIN)

$(TED_BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

# -----------------------------------------------------------------------------
# Regra genérica para objetos em src/
# Headers estão em include/, fontes em src/
# -----------------------------------------------------------------------------

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $(CFLAGS) -I$(INC_DIR) $< -o $@

# -----------------------------------------------------------------------------
# Objeto do Unity
# -----------------------------------------------------------------------------

$(UNITY_OBJ): $(UNITY_DIR)/unity.c $(UNITY_DIR)/unity.h
	$(CC) -c $(CFLAGS) -I$(UNITY_DIR) $< -o $@

# -----------------------------------------------------------------------------
# Dependências explícitas dos módulos
# .c em src/, .h em include/
# -----------------------------------------------------------------------------

$(SRC_DIR)/hashfile.o: $(SRC_DIR)/hashfile.c $(INC_DIR)/hashfile.h

$(SRC_DIR)/quadra.o: $(SRC_DIR)/quadra.c $(INC_DIR)/quadra.h \
                     $(INC_DIR)/hashfile.h

$(SRC_DIR)/habitante.o: $(SRC_DIR)/habitante.c $(INC_DIR)/habitante.h \
                        $(INC_DIR)/hashfile.h

$(SRC_DIR)/svg.o: $(SRC_DIR)/svg.c $(INC_DIR)/svg.h

$(SRC_DIR)/txt.o: $(SRC_DIR)/txt.c $(INC_DIR)/txt.h

$(SRC_DIR)/geo.o: $(SRC_DIR)/geo.c $(INC_DIR)/geo.h \
                  $(INC_DIR)/hashfile.h \
                  $(INC_DIR)/quadra.h   \
                  $(INC_DIR)/svg.h

$(SRC_DIR)/pm.o: $(SRC_DIR)/pm.c $(INC_DIR)/pm.h \
                 $(INC_DIR)/hashfile.h \
                 $(INC_DIR)/habitante.h

$(SRC_DIR)/qry.o: $(SRC_DIR)/qry.c $(INC_DIR)/qry.h \
                  $(INC_DIR)/hashfile.h  \
                  $(INC_DIR)/quadra.h    \
                  $(INC_DIR)/habitante.h \
                  $(INC_DIR)/svg.h       \
                  $(INC_DIR)/txt.h

$(SRC_DIR)/arquivos.o: $(SRC_DIR)/arquivos.c $(INC_DIR)/arquivos.h

$(SRC_DIR)/ted.o: $(SRC_DIR)/ted.c \
                  $(INC_DIR)/arquivos.h  \
                  $(INC_DIR)/hashfile.h  \
                  $(INC_DIR)/quadra.h    \
                  $(INC_DIR)/habitante.h \
                  $(INC_DIR)/svg.h       \
                  $(INC_DIR)/txt.h       \
                  $(INC_DIR)/geo.h       \
                  $(INC_DIR)/pm.h        \
                  $(INC_DIR)/qry.h

# -----------------------------------------------------------------------------
# Testes unitários individuais
# Cada target: compila o executável de teste e executa imediatamente.
# Os executáveis de teste são gerados em tst/.
# Flags de include: -I$(INC_DIR) para os .h do projeto, -I$(UNITY_DIR) para Unity
# -----------------------------------------------------------------------------

# --- hashfile ---
TST_HF_BIN  = $(TST_DIR)/t_hashfile
TST_HF_SRC  = $(TST_DIR)/t_hashfile.c
TST_HF_DEPS = $(SRC_DIR)/hashfile.o $(UNITY_OBJ)

$(TST_HF_BIN): $(TST_HF_SRC) $(TST_HF_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_hashfile
tst_hashfile: $(TST_HF_BIN)
	@echo ">>> Executando testes: hashfile <<<"
	@$(TST_HF_BIN)

# --- quadra ---
TST_QD_BIN  = $(TST_DIR)/t_quadra
TST_QD_SRC  = $(TST_DIR)/t_quadra.c
TST_QD_DEPS = $(SRC_DIR)/hashfile.o $(SRC_DIR)/quadra.o $(UNITY_OBJ)

$(TST_QD_BIN): $(TST_QD_SRC) $(TST_QD_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_quadra
tst_quadra: $(TST_QD_BIN)
	@echo ">>> Executando testes: quadra <<<"
	@$(TST_QD_BIN)

# --- habitante ---
TST_HB_BIN  = $(TST_DIR)/t_habitante
TST_HB_SRC  = $(TST_DIR)/t_habitante.c
TST_HB_DEPS = $(SRC_DIR)/hashfile.o $(SRC_DIR)/habitante.o $(UNITY_OBJ)

$(TST_HB_BIN): $(TST_HB_SRC) $(TST_HB_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_habitante
tst_habitante: $(TST_HB_BIN)
	@echo ">>> Executando testes: habitante <<<"
	@$(TST_HB_BIN)

# --- txt ---
TST_TXT_BIN  = $(TST_DIR)/t_txt
TST_TXT_SRC  = $(TST_DIR)/t_txt.c
TST_TXT_DEPS = $(SRC_DIR)/txt.o $(UNITY_OBJ)

$(TST_TXT_BIN): $(TST_TXT_SRC) $(TST_TXT_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_txt
tst_txt: $(TST_TXT_BIN)
	@echo ">>> Executando testes: txt <<<"
	@$(TST_TXT_BIN)

# --- svg ---
TST_SVG_BIN  = $(TST_DIR)/t_svg
TST_SVG_SRC  = $(TST_DIR)/t_svg.c
TST_SVG_DEPS = $(SRC_DIR)/svg.o $(UNITY_OBJ)

$(TST_SVG_BIN): $(TST_SVG_SRC) $(TST_SVG_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_svg
tst_svg: $(TST_SVG_BIN)
	@echo ">>> Executando testes: svg <<<"
	@$(TST_SVG_BIN)

# --- geo ---
TST_GEO_BIN  = $(TST_DIR)/t_geo
TST_GEO_SRC  = $(TST_DIR)/t_geo.c
TST_GEO_DEPS = $(SRC_DIR)/hashfile.o $(SRC_DIR)/quadra.o \
               $(SRC_DIR)/svg.o $(SRC_DIR)/geo.o $(UNITY_OBJ)

$(TST_GEO_BIN): $(TST_GEO_SRC) $(TST_GEO_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_geo
tst_geo: $(TST_GEO_BIN)
	@echo ">>> Executando testes: geo <<<"
	@$(TST_GEO_BIN)

# --- pm ---
TST_PM_BIN  = $(TST_DIR)/t_pm
TST_PM_SRC  = $(TST_DIR)/t_pm.c
TST_PM_DEPS = $(SRC_DIR)/hashfile.o $(SRC_DIR)/habitante.o \
              $(SRC_DIR)/pm.o $(UNITY_OBJ)

$(TST_PM_BIN): $(TST_PM_SRC) $(TST_PM_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_pm
tst_pm: $(TST_PM_BIN)
	@echo ">>> Executando testes: pm <<<"
	@$(TST_PM_BIN)

# --- qry ---
TST_QRY_BIN  = $(TST_DIR)/t_qry
TST_QRY_SRC  = $(TST_DIR)/t_qry.c
TST_QRY_DEPS = $(SRC_DIR)/hashfile.o $(SRC_DIR)/quadra.o \
               $(SRC_DIR)/habitante.o $(SRC_DIR)/svg.o   \
               $(SRC_DIR)/txt.o $(SRC_DIR)/qry.o $(UNITY_OBJ)

$(TST_QRY_BIN): $(TST_QRY_SRC) $(TST_QRY_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_qry
tst_qry: $(TST_QRY_BIN)
	@echo ">>> Executando testes: qry <<<"
	@$(TST_QRY_BIN)

# --- arquivos ---
TST_ARQ_BIN  = $(TST_DIR)/t_arquivos
TST_ARQ_SRC  = $(TST_DIR)/t_arquivos.c
TST_ARQ_DEPS = $(SRC_DIR)/arquivos.o $(UNITY_OBJ)

$(TST_ARQ_BIN): $(TST_ARQ_SRC) $(TST_ARQ_DEPS)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(UNITY_DIR) \
	      $^ -o $@ $(LIBS)

.PHONY: tst_arquivos
tst_arquivos: $(TST_ARQ_BIN)
	@echo ">>> Executando testes: arquivos <<<"
	@$(TST_ARQ_BIN)

# -----------------------------------------------------------------------------
# tstall — compila e executa todos os testes unitários
# -----------------------------------------------------------------------------

.PHONY: tstall
tstall: tst_hashfile tst_quadra tst_habitante tst_txt tst_svg \
        tst_geo tst_pm tst_qry tst_arquivos
	@echo ""
	@echo "=== Todos os testes unitários executados ==="

# -----------------------------------------------------------------------------
# clean — remove todos os arquivos gerados
# -----------------------------------------------------------------------------

.PHONY: clean
clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(UNITY_DIR)/*.o
	rm -f $(TED_BIN)
	rm -f $(TST_DIR)/t_hashfile
	rm -f $(TST_DIR)/t_quadra
	rm -f $(TST_DIR)/t_habitante
	rm -f $(TST_DIR)/t_txt
	rm -f $(TST_DIR)/t_svg
	rm -f $(TST_DIR)/t_geo
	rm -f $(TST_DIR)/t_pm
	rm -f $(TST_DIR)/t_qry
	rm -f $(TST_DIR)/t_arquivos