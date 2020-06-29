all:bin/coder

bin/coder:src/main.c
	gcc -Wall -I src src/main.c src/comand.c src/coder.c -o bin/coder -lm
	
clean:
	rm -rf build/*.dat
	rm -rf build/*.bin 
	rm -rf bin/coder
