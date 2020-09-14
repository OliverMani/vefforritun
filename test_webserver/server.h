#pragma once
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdlib.h>

class Server {
private:
	short port;
	int socketfd, client, valread, addrlen, sd, max_sd;
	struct sockaddr_in address;
	unsigned long nbio;
	fd_set readfds;
public:
	Server(short port);
	short get_port();
	int accept();
};