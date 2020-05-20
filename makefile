all:main


main:build/src/main.o
	gcc -Wall build/src/main.o -o bin/main -lm
build/src/main.o:src/main.c
	gcc -Wall -c src/main.c -o build/src/main.o

move:
	cp -r ../Lab_3.1/build/*.bin bin/

compres:
	bin/./main decode bin/compressed.bin bin/compressed.txt
	cat bin/compressed.txt

uncompres:
	bin/./main decode bin/uncompressed.bin bin/uncompressed.txt
	cat bin/uncompressed.txt

encode:
	bin/./main encode *.txt bin/encode.bin
	cat bin/encode.bin

decode:
	bin/./main decode *.bin bin/decode.txt
	cat bin/decode.txt

clean:
	rm -rf build/src/*.o bin/main bin/*.bin bin/*.txt *.txt
	touch bin/uncompressed.txt
	touch bin/compressed.txt
	touch bin/decode.txt
	touch bin/encode.txt
	touch decode.bin
	touch encode.txt

fastsetup:
	make clean
	make
	make move
	make compres
	make uncompres
