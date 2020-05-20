all:main


main:build/src/main.o
	gcc -Wall build/src/main.o -o bin/main -lm
build/src/main.o:src/main.c
	gcc -Wall -c src/main.c -o build/src/main.o

run:
	./main


clean:
	rm -rf build/src/*.o bin/main bin/*.dat bin/*.txt
	touch bin/undec.txt
	touch bin/dec.txt
