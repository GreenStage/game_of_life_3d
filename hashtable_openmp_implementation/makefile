#makefile

all: life3d-omp
life3d-omp: main/main.o utils/error.o world/generations.o hashtable/ht.o
	gcc -g -std=c99 -fopenmp -o life3d-omp main/main.o utils/error.o world/generations.o hashtable/ht.o

main/main.o: main/life3d-omp.c utils/error.h utils/defs.h world/generations.h hashtable/ht.h
	cd main && gcc -g -std=c99 -o main.o -c life3d-omp.c && \
	cd ..

hashtable/ht.o: hashtable/ht.c hashtable/ht.h utils/defs.h
	cd hashtable && gcc -g -std=c99 -fopenmp -o ht.o -c ht.c -W -Wall && \
	cd ..

world/generations.o: world/generations.c world/generations.h utils/error.h utils/defs.h
	cd world && gcc -g -std=c99 -fopenmp -o generations.o -c generations.c -W -Wall && \
	cd ..

utils/error.o: utils/error.c utils/error.h
	cd utils && gcc -std=c99 -o error.o -c error.c -W -Wall && \
	cd ..

clean:
	cd main && rm -rf *.o && cd ..; \
	cd world && rm -rf *.o && cd ..; \
	cd utils && rm -rf *.o && cd ..; \
	cd hashtable && rm -rf *.o && cd ..;\
	rm -rf life3d-omp
