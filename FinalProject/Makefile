all: client server

client:
	gcc -o client client.c -lpthread
server:
	gcc -o server server.c -lpthread

clean:
	rm -f client server

.PHONY: clean client server all