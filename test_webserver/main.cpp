// Web Server fyrir vefsíðuna

#include <iostream>
#include <thread>
#include <string>

#include "server.h"
#include "http.h"

using namespace std;

static struct {
	short port;
	bool ssl;
	bool preload_all;

} CONFIG = {
	8080,
	false,
	false,
};

static Server* SERVER;

void parseargs(int argc, char** argv){
	for(int i = 1; i < argc; i++) {
		string str(argv[i]);

		if(str == "--port") {
			CONFIG.port = stoi(string(argv[++i]));
		} else if(str == "-ssl") {
			CONFIG.ssl = true;
		} else if(str == "--preload-all") {
			CONFIG.preload_all = true;
		}
	}
}

void console(){

}

void listen() {
	while(1) {
		int socket = SERVER->accept();
		cout << "Accepted socket " << socket << "!\n";
		
		HTTP http(socket);
		if(http.get_path() == "/") {
			string hello = "<!DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>StæFæ</title>\n\t\t<meta charset=\"utf-8\">\n\t</head>\n\t<body>\n\t\t<h1>Hello world:)</h1>\n\t</body>\n</html>";

			http.set_content_type("text/html");
			http.set_status(200);
			http.set_content((char*)hello.c_str(), hello.length());
			http.respond();
		} else {
			http.set_content_type("text/plain");
			http.set_status(404);
			http.set_content((char*)"404 not found!", 14);
			http.respond();
		}
		cout << "Closed socket " << socket << "!\n";
		

		

	}
}

int main(int argc, char** argv){
	parseargs(argc, argv);
	cout << "Launching server...\n";
	cout << "PORT: " << CONFIG.port << "\nSSL: " << (CONFIG.ssl ? "true" : "false") << "\nPRELOAD: " << (CONFIG.preload_all ? "true" : "false") << "\n";
	SERVER = new Server(CONFIG.port);
	cout << "OK\n";
	listen();
	delete SERVER;

}