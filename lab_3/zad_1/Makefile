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
pthreads_detach_kill: pthreads_detach_kill.o
	$(LOADER) $(OPT) -pthread pthreads_detach_kill.o -o pthreads_detach_kill $(LIB)

pthreads_detach_kill.o: pthreads_detach_kill.c
	$(CCOMP) -c $(OPT) pthreads_detach_kill.c $(INC)

clean:
	rm -f *.o
