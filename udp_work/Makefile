src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))

all: server client client_broadcast server_broadcast server_multicast client_multicast

server:server.o 
	gcc server.o  -o server -Wall 
client:client.o 
	gcc client.o   -o client -Wall
server_broadcast:server_broadcast.o
	gcc server_broadcast.o -o server_broadcast -Wall
client_broadcast:client_broadcast.o
	gcc client_broadcast.o -o client_broadcast -Wall
server_multicast:server_multicast.o
	gcc server_multicast.o -o server_multicast -Wall
client_multicast:client_multicast.o
	gcc client_multicast.o -o client_multicast -Wall

%.o:%.c
	gcc -c $< -Wall
.PHONY:clean all
clean:
	-rm -rf server client client_broadcast server_broadcast server_multicast client_multicast $(obj)	
