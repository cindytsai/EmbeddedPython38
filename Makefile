BIN  := EmbedString.out
FILE := SimpleString.c

$(BIN): $(FILE)
	gcc $(shell python3.8-config --cflags --embed) -o $(BIN) $(FILE) $(shell python3.8-config --ldflags --embed)

clean:
	rm -f *.out