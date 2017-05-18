#makefile
FLAGS = 

all: life3d

debug: FLAGS +=  -g
debug: life3d

life3d: src/mpiVersion/main.o src/common/error.o src/map/world.o src/map/cell.o src/common/utils.o
	mpicc -o life3d src/mpiVersion/main.o src/common/error.o src/map/world.o src/map/cell.o src/common/utils.o

src/mpiVersion/main.o: src/mpiVersion/life3d.c src/common/error.h src/common/defs.h src/map/cell.h src/map/world.h
	cd src/mpiVersion && mpicc -o main.o -c life3d.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/map/world.o: src/map/world.c src/map/world.h src/map/cell.h src/common/error.h src/common/defs.h
	cd src/map && mpicc -o world.o -c world.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/map/cell.o: src/map/cell.c src/map/cell.h src/common/error.h src/common/defs.h
	cd src/map && mpicc -o cell.o -c cell.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/common/error.o: src/common/error.c src/common/error.h
	cd src/common && mpicc -o error.o -c error.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

src/common/utils.o: src/common/utils.c src/common/defs.h
	cd src/common && mpicc -o utils.o -c utils.c $(FLAGS) -W -Wall -ansi && \
	cd .. && cd ..

clean:
	cd src &&\
	cd mpiVersion && rm -rf *.o && cd ..;\
	cd map && rm -rf *.o && cd ..; \
	cd common && rm -rf *.o&& cd ..; \
	cd .. &&\
	rm -rf life3d
