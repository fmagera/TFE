CC = gcc
CFLAGS =   -W -Wall -ansi -pedantic 
LASH = -I/home/floriane/lash/src/lash-core/
ARCH32 = -m32

all:  data


data: data.o pattern.o hashtable_r.o genData.o composition.o test_auto.o  addition.o  end_normalize.o fib_add.o base_automaton.o /home/floriane/lash/lib/lash-core.o
	$(CC) $(ARCH32) -o data data.o pattern.o hashtable_r.o genData.o composition.o test_auto.o addition.o end_normalize.o fib_add.o base_automaton.o /home/floriane/lash/lib/lash-core.o -lm


pattern.o: pattern.c
	$(CC) $(ARCH32) $(CFLAGS) -std=c11 -o pattern.o -c pattern.c

hashtable_r.o: hashtable_r.c
	$(CC) $(ARCH32) $(CFLAGS) -std=c11 -o hashtable_r.o  -c hashtable_r.c

genData.o: genData.c hashtable_r.h pattern.h
	$(CC) $(ARCH32) $(CFLAGS) -std=c11 -o genData.o -c genData.c

composition.o: composition.c
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c composition.c -o composition.o -lm

test_auto.o: test_auto.c
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c test_auto.c -o test_auto.o -lm

addition.o: addition.c
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c addition.c -o addition.o -lm

end_normalize.o: end_normalize.c
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c end_normalize.c -o end_normalize.o -lm	

fib_add.o: fib_add.c
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c fib_add.c -o fib_add.o -lm

base_automaton.o: base_automaton.c
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c base_automaton.c -o base_automaton.o -lm

data.o: main.c pattern.h hashtable_r.h genData.h composition.h test_auto.h addition.h end_normalize.h fib_add.h base_automaton.h
	$(CC) $(ARCH32) $(LASH) $(CFLAGS) -std=c11 -c main.c -o data.o -lm

clean:
	rm -rf *.o

mrproper: clean
	rm -rf auto data

