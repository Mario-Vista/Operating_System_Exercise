#makefile palindroma

#versione1

all: usaPal usaRev
	./usaPal anna

install:
	cp usaPal usaRev ${bindir}

clean:
	rm *.o usaPal reverse palindroma

usaPal: usaPal.o palindroma.o reverse.o
	gcc reverse.o palindroma.o usaPal.o -o usaPal


usaPal.o: usaPal.c palindroma.h
	gcc -c usaPal.c

palindroma.o: palindroma.c palindroma.h reverse.h
	gcc -c palindroma.c

reverse.o: reverse.c reverse.h
	gcc -c reverse.c

