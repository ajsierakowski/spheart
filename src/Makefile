# Written by Adam Sierakowski 2015
# for JHU HEART: The Computer Laboratory
#
# Compile with: 'make'

CC = gcc

CFLAGS = -std=c99 -pedantic -Wall -Wextra -g

LDLIBS = -lm

all: sph

sph: sph.o io.o vtk.o rng.o phys.o mergesort.o

sph.o: sph.c

io.o: io.c

vtk.o: vtk.c

rng.o: rng.c

phys.o: phys.c

mergesort.o: mergesort.c

clean:
	rm *.o sph
