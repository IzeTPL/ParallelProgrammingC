# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
INC = -I/usr/include

# biblioteki
LIB = -L/usr/lib64

# zaleznosci i komendy
pthreads_self: pthreads_self.o
	$(LOADER) $(OPT) -pthread pthreads_self.o -o pthreads_self $(LIB)

pthreads_self.o: pthreads_self.c
	$(CCOMP) -c $(OPT) pthreads_self.c $(INC)

clean:
	rm -f *.o
