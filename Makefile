all: project2.o
	gcc -o project2 project2.c

clean:
	rm project2
	rm project2.o
