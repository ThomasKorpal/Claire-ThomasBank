CC = gcc
CCFLAGS = -Wall

server_TCP : bank.c server/server.c
	$(CC) $(CCFLAGS) -o server_TCP bank.c server/server.c

client_TCP : client/client.c
	$(CC) $(CCFLAGS) -o client_TCP client/client.c

server_UDP: bank.c server/UDPserver.c
	$(CC) $(CCFLAGS) -o server_UDP bank.c server/UDPserver.c

client_UDP: client/UDPclient.c
	$(CC) $(CCFLAGS) -o client_UDP client/UDPclient.c

clean:
	rm -f *.o
mproper:
	rm -f server_TCP server_UDP client_TCP client_UDP *.o