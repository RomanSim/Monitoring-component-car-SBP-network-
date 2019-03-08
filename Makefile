all: a.out

a.out: exercise.o
	gcc -o a.out

exercise.o: exercise.c
		gcc -c exercise.c
	
