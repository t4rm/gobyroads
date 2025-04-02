all : main.o struct.o
	clang -o prog -Wall main.o struct.o -lncurses
run : main.o struct.o
	clang -o prog -Wall main.o struct.o -lncurses && ./prog
main.o : main.c struct.h
	clang -c -Wall main.c
struct.o : struct.c struct.h
	clang -c -Wall struct.c
clean : 
	rm -f *.o prog
