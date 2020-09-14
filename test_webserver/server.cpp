#include "server.h"

#include <iostream>

using namespace std;

Server::Server(short port) {
	this->port = port;
	int opt = 1;

	this->addrlen = sizeof(this->address);
	
	

	this->socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if(!this->socketfd) {
		cerr << "Socket failed! (step 1)\n";
		return;
	}
	
	int sockopt = setsockopt(this->socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if(sockopt) {
		cerr << "setsocketopt error: opt = " << opt << " sockopt = " << sockopt << "\n";
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if(bind(this->socketfd, (struct sockaddr*) &address, sizeof(address)) < 0) {
		cerr << "Bind failed\n";
		exit(1);
	}

	if(listen(this->socketfd, 100) < 0) {
		cerr << "Listen failed!\n";
		return;
	}
	
	

	cout << "Server OK\n";




}

int Server::accept(){
	FD_ZERO(&this->readfds);
	FD_SET(&this->socketfd, &this->readfds);
	int sock = ::accept(socketfd, (struct sockaddr*)&this->address, (socklen_t*) &addrlen);
	if(sock < 0) {
		cerr << "Accept error!\n";
	}

	return sock;
}

short Server::get_port(){
	return this->port;
}