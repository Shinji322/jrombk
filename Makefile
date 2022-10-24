CC=gcc

LIBS=-lncurses -lm

SOURCE_FILES=$(wildcard src/*.c)

OUTPUT_FILE=jrombk

main:
	@mkdir -p build
	$(CC) $(SOURCE_FILES) -g -o build/$(OUTPUT_FILE) $(LIBS)
