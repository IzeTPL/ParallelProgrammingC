# kompilator c
CCOMP = /usr/bin/mpicc

# konsolidator
LINK = /usr/bin/mpicc

MPIRUN = /usr/bin/mpirun

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG -p
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3 -fopenmp -p

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lm
LIB = -lm

# zaleznosci i komendy

pi: pi.o
	$(LINK) $(OPT) pi.o -o pi $(LIB)

pi.o: pi.c
	$(CCOMP) -c $(OPT) pi.c $(INC)

run:
	$(MPIRUN) -np 4 ./pi

clean:
	rm -f *.o
