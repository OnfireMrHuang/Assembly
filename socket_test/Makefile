src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))

all: server client multi_process_serv multi_thread_serv io_multiplex_serv

server:server.o wrap.o 
	gcc server.o wrap.o -o server -Wall 
client:client.o wrap.o
	gcc client.o wrap.o -o client -Wall
multi_process_serv:multi_process_serv.o wrap.o
	gcc multi_process_serv.o wrap.o -o multi_process_serv -Wall
multi_thread_serv:multi_thread_serv.o wrap.o
	gcc multi_thread_serv.o wrap.o -o multi_thread_serv -Wall -lpthread
io_multiplex_serv:io_multiplex_serv.o wrap.o
	gcc io_multiplex_serv.o wrap.o -o io_multiplex_serv -Wall

%.o:%.c
	gcc -c $< -Wall
.PHONY:clean all
clean:
	-rm -rf server client multi_process_serv multi_thread_serv io_multiplex_serv $(obj)	
