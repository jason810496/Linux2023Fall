all: client server

client:
	gcc -o client client.c -lpthread
server:
	gcc -o server server.c -lpthread
clean:
	rm -f client server
test:
	./server &
	./client < in > out
	diff -s out ans

.PHONY: clean client server all