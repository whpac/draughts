C_SOURCES = $(wildcard source/*.c) $(wildcard source/**/*.c) $(wildcard source/**/**/*.c)
HEADERS = $(wildcard source/*.h) $(wildcard source/**/*.h) $(wildcard source/**/**/*.h)
OBJ = ${patsubst source/%.c,obj/%.o,$(C_SOURCES)}
OUTPUT = bin/draughts.exe

all: folders ${OUTPUT}
run: all
	./${OUTPUT}

${OUTPUT}: ${OBJ}
	gcc -o $@ $^ -lallegro -lallegro_font -lallegro_primitives

obj/%.o: source/%.c ${HEADERS}
	gcc -c $< -o $@

clean:
	rmdir /q /s obj
	mkdir obj

folders:
	xcopy source obj /t /e