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
pthreads_struct: pthreads_struct.o
	$(LOADER) $(OPT) -pthread pthreads_struct.o -o pthreads_struct $(LIB)

pthreads_struct.o: pthreads_struct.c
	$(CCOMP) -c $(OPT) pthreads_struct.c $(INC)

clean:
	rm -f *.o
