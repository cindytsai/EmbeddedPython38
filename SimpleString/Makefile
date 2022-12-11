BIN  := EmbedString.out
FILE := SimpleString.c
COMPILER := gcc

$(BIN): $(FILE)
	$(COMPILER) $(shell python3.8-config --cflags --embed) -o $(BIN) $(FILE) $(shell python3.8-config --ldflags --embed)

clean:
	rm -rf *.out __pycache__ log
