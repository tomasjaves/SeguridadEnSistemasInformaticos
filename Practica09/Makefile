CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17
LDFLAGS =

SRC = src/rsa.cc src/client.cc
OBJ = $(SRC:src/%.cc=build/%.o)
EXEC = program

# Colores
COLOUR_GREEN=\033[1;32m
COLOUR_RED=\033[1;31m
COLOUR_BLUE=\033[1;34m
COLOUR_END=\033[1m
COLOUR_YELLOW=\033[1;33m
COLOUR_PURPLE=\033[1;35m
COLOUR_CYAN=\033[1;36m

# Contador para el progreso
TOTAL_FILES := $(words $(SRC))
CURRENT_FILE = 0

define compile
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@echo "${COLOUR_CYAN}COMPILANDO $(1) ($(CURRENT_FILE) DE $(TOTAL_FILES))...${COLOUR_CYAN}"
	@$(CXX) $(CXXFLAGS) -c -o $(2) $(1)
endef

all: $(EXEC)
	@echo "${COLOUR_PURPLE}COMPILACIÓN COMPLETADA.${COLOUR_PURPLE}"

$(EXEC): $(OBJ)
	@echo "${COLOUR_CYAN}ENLAZANDO OBJETOS Y CREANDO EJECUTABLE...${COLOUR_CYAN}"
	@$(CXX) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)
	@echo "${COLOUR_GREEN}EJECUTABLE ${EXEC} CREADO.${COLOUR_GREEN}"

build/%.o: src/%.cc
	$(call compile,$<,$@)

clean:
	@echo "${COLOUR_RED}LIMPIANDO ARCHIVOS...${COLOUR_RED}"
	@rm -rf $(OBJ) $(EXEC)
