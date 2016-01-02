CC=gcc
CFLAGS=-I.

all: evolution

obj/server.o: src/server.c
	gcc -c -o obj/server.o src/server.c

obj/client.o: src/client.c
	gcc -c -o obj/client.o src/client.c

obj/network.a: obj/server.o obj/client.o
	ar cr obj/network.a obj/server.o obj/client.o

obj/management.o: src/management.c
	gcc -c -o obj/management.o `xml2-config --cflags` src/management.c `xml2-config --libs`

obj/list.o: src/list.c
	gcc -c -o obj/list.o src/list.c

obj/hash.o: src/hash.c
	gcc -c -o obj/hash.o src/hash.c

obj/func.o: src/func.c
	gcc -c -o obj/func.o src/func.c
	
obj/util.a: obj/list.o obj/hash.o obj/func.o
	ar cr obj/util.a obj/list.o obj/hash.o obj/func.o

obj/environment.o: src/environment.c
	gcc -c -o obj/environment.o `xml2-config --cflags` src/environment.c `xml2-config --libs`

obj/character.o: src/character.c
	gcc -c -o obj/character.o `xml2-config --cflags` src/character.c `xml2-config --libs`
	
obj/stats.o: src/stats.c
	gcc -c -o obj/stats.o src/stats.c

obj/user.o: src/user.c
	gcc -c -o obj/user.o `xml2-config --cflags` src/user.c `xml2-config --libs`
	
obj/action.o: src/action.c
	gcc -c -o obj/action.o `xml2-config --cflags` src/action.c `xml2-config --cflags` 
	
obj/room.o: src/room.c
	gcc -c -o obj/room.o `xml2-config --cflags` src/room.c `xml2-config --libs`

evolution: evolution.c obj/network.a obj/management.o obj/environment.o obj/util.a obj/character.o obj/user.o obj/action.o obj/room.o obj/stats.o
	gcc -o evolution `xml2-config --cflags` evolution.c obj/network.a obj/management.o obj/environment.o obj/util.a obj/character.o obj/user.o obj/action.o obj/room.o obj/stats.o -lpthread  `xml2-config --libs`

clean:
	rm obj/*.o obj/*.a evolution

