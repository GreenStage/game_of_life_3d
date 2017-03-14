#makefile
all: life3d
life3d: main.o error.o world.o
	gcc -o life3d main.o error.o world.o

main.o: serial/life3d.c common/error.h common/defs.h common/world.h
	gcc -o main.o -c serial/life3d.c -W -Wall -ansi -pedantic -g

world.o: common/world.c common/world.h common/error.h common/common.h common/cell.h
	gcc -o world.o -c common/world.c -W -Wall -ansi -pedantic

error.o: common/error.c
	gcc -o error.o -c common/error.c -W -Wall -ansi -pedantic

clean:
	rm -rf *.o
clean_serial: clean
	rm -rf life3d
