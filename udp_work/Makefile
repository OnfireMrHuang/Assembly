src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))

all: server client client_broadcast server_broadcast

server:server.o 
	gcc server.o  -o server -Wall 
client:client.o 
	gcc client.o   -o client -Wall
server_broadcast:server_broadcast.o
	gcc server_broadcast.o -o server_broadcast -Wall
client_broadcast:client_broadcast.o
	gcc client_broadcast.o -o client_broadcast -Wall

%.o:%.c
	gcc -c $< -Wall
.PHONY:clean all
clean:
	-rm -rf server client client_broadcast server_broadcast $(obj)	
