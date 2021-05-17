all: main.o tree.o
	g++ main.o tree.o -o Diff.exe

tree.o:
	gcc -c tree.cpp -o tree.o

main.o:
	gcc -c main.cpp -o main.o

clean:
	del *.o