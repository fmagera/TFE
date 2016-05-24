CC = gcc
CFLAGS =   -W -Wall -ansi -pedantic 
LASH = -I/home/floriane/lash/src/lash-core/
ARCH32 = -m32

all: auto data

auto: /home/floriane/lash/lib/lash-core.o auto.o
	$(CC) $(ARCH32) -o auto auto.o /home/floriane/lash/lib/lash-core.o

data: data.o pattern.o hashtable_r.o genData.o /home/floriane/lash/lib/lash-core.o
	$(CC) $(ARCH32) -o data data.o pattern.o hashtable_r.o genData.o /home/floriane/lash/lib/lash-core.o -lm

auto.o: test-lash.c
	$(CC) $(ARCH32) $(CFLAGS) $(LASH) -o auto.o -c test-lash.c

pattern.o: pattern.c
	$(CC) $(ARCH32) $(CFLAGS) -std=c11 -o pattern.o -c pattern.c

hashtable_r.o: hashtable_r.c
	$(CC) $(ARCH32) $(CFLAGS) -std=c11 -o hashtable_r.o  -c hashtable_r.c

genData.o: genData.c hashtable_r.h pattern.h
	$(CC) $(ARCH32) $(CFLAGS) -std=c11 -o genData.o -c genData.c

data.o: post_normalization.c pattern.h hashtable_r.h genData.h
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c post_normalization.c -o data.o -lm

clean:
	rm -rf *.o

mrproper: clean
	rm -rf auto

