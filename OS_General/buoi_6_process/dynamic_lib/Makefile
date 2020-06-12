CC = gcc

all: 
	mkdir build
	$(CC) -c -Wall -Werror -fpic ./src/foo.c
	$(CC) -shared -o libfoo.so foo.o
	$(CC) -L./ -Wl,-rpath=/usr/lib -Wall -o ./build/test main.c -lfoo
clean:
	rm -r foo.o libfoo.so 
	rm -r build

