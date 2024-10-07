# Definir o compilador e as flags de compilação
CC = gcc
CFLAGS = -Wall -pthread

# Definir os arquivos-fonte e o nome do executável
SOURCES = main.c functions.c
EXEC = simulacao_incendio

# Regra padrão (target default, roda quando você executa `make`)
all: $(EXEC)

# Regra para compilar o programa
$(EXEC): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXEC) $(SOURCES)

# Limpar os arquivos de compilação
clean:
	rm -f $(EXEC)

# Para rodar o executável
run: $(EXEC)
	./$(EXEC)

