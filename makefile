C_SOURCES = $(wildcard source/*.c) $(wildcard source/**/*.c) $(wildcard source/**/**/*.c)
HEADERS = $(wildcard source/*.h) $(wildcard source/**/*.h) $(wildcard source/**/**/*.h)
OBJ = ${patsubst source/%.c,obj/%.o,$(C_SOURCES)}
OUTPUT = bin/draughts.exe

all: folders ${OUTPUT}
run: all
	./${OUTPUT}

run-text: all
	./${OUTPUT} text

${OUTPUT}: ${OBJ}
	gcc -o $@ $^ -lallegro -lallegro_font -lallegro_primitives

obj/%.o: source/%.c ${HEADERS}
	gcc -c $< -o $@

clean:
	rmdir /q /s obj

# Ignoruje błędy, kiedy foldery bin i obj istnieją
# 2> NUL znika komunikat błędu
# || (exit 0) zapewnia traktowanie komendy jako udanej
folders:
	mkdir obj 2> NUL || (exit 0)
	mkdir bin 2> NUL || (exit 0)
	xcopy source obj /t /e