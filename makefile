C_SOURCES = $(wildcard source/*.c) $(wildcard source/**/*.c)
HEADERS = $(wildcard source/*.h) $(wildcard source/**/*.h)
OBJ = ${patsubst source/%.c,obj/%.o,$(C_SOURCES)}
OUTPUT = bin/draughts.exe

all: ${OUTPUT}
run: all
	./${OUTPUT}

${OUTPUT}: ${OBJ}
	gcc -o $@ $^

obj/%.o: source/%.c ${HEADERS}
	gcc -c $< -o $@

clean:
	find obj/ -name "*.o" -type f -delete