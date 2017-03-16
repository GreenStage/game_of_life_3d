#makefile

all: life3d
life3d: serial/main.o common/error.o map/world.o map/cell.o
	gcc -o life3d serial/main.o common/error.o map/world.o map/cell.o

serial/main.o: serial/life3d.c common/error.h common/defs.h map/cell.h map/world.h
	cd serial && gcc -o main.o -c life3d.c && \
	cd ..

map/world.o: map/world.c map/world.h map/cell.h common/error.h common/defs.h
	cd map && gcc -o world.o -c world.c -W -Wall -ansi && \
	cd ..

map/cell.o: map/cell.c map/cell.h common/error.h common/defs.h
	cd map && gcc -o cell.o -c cell.c -W -Wall -ansi && \
	cd ..

common/error.o: common/error.c common/error.h
	cd common && gcc -o error.o -c error.c -W -Wall -ansi && \
	cd ..

clean:
	cd serial && rm -rf *.o && cd ..;\
	cd map && rm -rf *.o && cd ..; \
	cd common && rm -rf *.o&& cd ..; \
	rm -rf life3d
