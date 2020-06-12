all:
	gcc -c hello.c
	ar rs libbasic.a hello.o
	gcc -o main main.c libbasic.a
clean:
	rm hello.o libbasic.a main
