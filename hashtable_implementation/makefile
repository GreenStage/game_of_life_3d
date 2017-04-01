#makefile

all: life3d
life3d: main/main.o utils/error.o world/generations.o hashtable/ht.o
	gcc -std=c99 -o life3d main/main.o utils/error.o world/generations.o hashtable/ht.o

main/main.o: main/life3d.c utils/error.h utils/defs.h world/generations.h hashtable/ht.h
	cd main && gcc -std=c99 -o main.o -c life3d.c && /
	cd ..

hashtable/ht.o: hashtable/ht.c hashtable/ht.h utils/defs.h
	cd hashtable && gcc -std=c99 -o ht.o -c ht.c -W -Wall && \
	cd ..

world/generations.o: world/generations.c world/generations.h utils/error.h utils/defs.h
	cd world && gcc -std=c99 -o generations.o -c generations.c -W -Wall && \
	cd ..

utils/error.o: utils/error.c utils/error.h
	cd utils && gcc -std=c99 -o error.o -c error.c -W -Wall && \
	cd ..

clean:
	rm -rf *.o && cd ..;\
	cd world && rm -rf *.o && cd ..; \
	cd utils && rm -rf *.o&& cd ..; \
	rm -rf life3d
