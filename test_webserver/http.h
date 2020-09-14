#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

class HTTP {
private:
	int socket;
	string path;
	string host;
	int status;
	string agent;
	bool DNT;
	string content_type;
	string server;

	char* content;
	int content_length;
public:
	HTTP(int socket);
	void respond();

	string get_path();
	string get_host();
	int get_status();
	void set_status(int status);
	string get_user_agent();
	bool is_dnt();
	string get_content_type();
	void set_content_type(string);
	string get_server();
	void set_content(char* data, int length);
	char* get_content();
	int get_content_length();

};