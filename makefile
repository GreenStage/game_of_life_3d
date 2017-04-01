#makefile
FLAGS = 

all: life3d

debug: FLAGS +=  -g
debug: life3d

life3d: src/serial/main.o src/common/error.o src/map/world.o src/map/cell.o
	gcc -o life3d src/serial/main.o src/common/error.o src/map/world.o src/map/cell.o

src/serial/main.o: src/serial/life3d.c src/common/error.h src/common/defs.h src/map/cell.h src/map/world.h
	cd src/serial && gcc -o main.o -c life3d.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/map/world.o: src/map/world.c src/map/world.h src/map/cell.h src/common/error.h src/common/defs.h
	cd src/map && gcc -o world.o -c world.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/map/cell.o: src/map/cell.c src/map/cell.h src/common/error.h src/common/defs.h
	cd src/map && gcc -o cell.o -c cell.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/common/error.o: src/common/error.c src/common/error.h
	cd src/common && gcc -o error.o -c error.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

clean:
	cd src &&\
	cd serial && rm -rf *.o && cd ..;\
	cd map && rm -rf *.o && cd ..; \
	cd common && rm -rf *.o&& cd ..; \
	cd .. &&\
	rm -rf life3d
