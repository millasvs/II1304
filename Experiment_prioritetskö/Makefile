CC = gcc

TARGETS = splay splay_best splay_worst

all:	$(TARGETS)

splay: splay.o print_splay.o
	$(CC) -o $@ splay.o print_splay.o
splay_best: splay_best.o print_splay.o
	$(CC) -o $@ splay_best.o print_splay.o
splay_worst: splay_worst.o print_splay.o
	$(CC) -o $@ splay_worst.o print_splay.o


splay.o: splayTree.c print_splay.h
	$(CC) -c splayTree.c -o splay.o
splay_best.o: splayTree_best.c print_splay.h
	$(CC) -c splayTree_best.c -o splay_best.o
splay_worst.o: splayTree_worst.c print_splay.h
	$(CC) -c splayTree_worst.c -o splay_worst.o
print_splay.o: print_splay.c print_splay.h
	$(CC) -c print_splay.c	

clean:
	rm -f *.o $(TARGETS)
	
	

