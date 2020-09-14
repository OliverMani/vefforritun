#include "http.h"

#include <iostream>

static struct {
	int status;
	string name;
	
} STATUSES[] = {
	{100, "Continue"},
	{101, "Switching Protocols"},
	{102, "Processing"},
	{200, "OK"},
	{201, "Created"},
	{202, "Accepted"},
	{203, "Non-authoritative Information"},
	{204, "No Content"},
	{205, "Reset Content"},
	{206, "Partial Content"},
	{207, "Multi-Status"},
	{208, "Already Reported"},
	{226, "IM Used"},
	{300, "Multiple Choices"},
	{301, "Moved Permanently"},
	{302, "Found"},
	{303, "See Other"},
	{304, "Not Modified"},
	{305, "Use Proxy"},
	{307, "Temporary Redirect"},
	{308, "Permanent Redirect"},
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{402, "Payment Required"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{406, "Not Acceptable"},
	{407, "Proxy Authentication Required"},
	{408, "Request Timeout"},
	{409, "Conflict"},
	{410, "Gone"},
	{411, "Length Required"},
	{412, "Precondition Failed"},
	{413, "Payload Too Large"},
	{414, "Request-URI Too Long"},
	{415, "Unsupported Media Type"},
	{416, "Requested Range Not Satisfiable"},
	{417, "Expectation Failed"},
	{418, "I'm a teapot"},
	{421, "Misdirected Request"},
	{422, "Unprocessable Entity"},
	{423, "Locked"},
	{424, "Failed Dependency"},
	{426, "Upgrade Required"},
	{428, "Precondition Required"},
	{429, "Too Many Requests"},
	{431, "Request Header Fields Too Large"},
	{444, "Connection Closed Without Response"},
	{451, "Unavailable For Legal Reasons"},
	{499, "Client Closed Request"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{502, "Bad Gateway"},
	{503, "Service Unavailable"},
	{504, "Gateway Timeout"},
	{505, "HTTP Version Not Supported"},
	{506, "Variant Also Negotiates"},
	{507, "Insufficient Storage"},
	{508, "Loop Detected"},
	{510, "Not Extended"},
	{511, "Network Authentication Required"},
	{599, "Network Connect Timeout Error"}
};

string get_message(int status) {
	for(int i = 0; i < 62; i++) {
		if(STATUSES[i].status == status)
			return STATUSES[i].name;
	}
	return "";
}

HTTP::HTTP(int socket) {
	this->socket = socket;
	this->server = "potato.";
	this->status = 404;

	int end_of_head = 0;
	bool first = true;

	char ch;

	string line;
	string kkey;
	int blank = 0;
	
	for(int iline = 0; !end_of_head; iline++) {
		bool key = iline != 0;
		line = "";
		kkey = "";
		
		while(1) {
			ch = 0;
			int err = read(socket, &ch, 1);
			if(ch == 0 || err < 0) {
				cout << "\n======\nConnection dropped!\n======\n\n";
				this->set_status(400);
				string message = "400 bad request!";
				this->set_content_type("text/html");
				this->set_content((char*)message.c_str(), message.length());
				this->respond();
				if(err >= 0)
					close(this->socket);
				return;
			}
			if(ch == '\n' || ch == '\r')
				break;
			if(key && ch == ' ')
				continue;
			if(key && ch == ':') {
				kkey = line;
				key = false;
			}
			line.push_back(ch);
		}

		string part = line;
		if(!iline) {
			cout << line << endl;
			if(part.substr(0, line.find(' ')) == "GET") {
				part = part.substr(line.find(' ') + 1);

				string path = part.substr(0, part.find(' '));
				this->path = path;
				cout << "Path: " << path << endl;
			}
		}



		if(line == "") {
			if(blank) {
				end_of_head = true;
			} else {
				blank++;
			}
		} else {
			blank = 0;
			//if(kkey != "")
			//	cout << "Key: \"" << kkey << "\"\nValue: \"" << line.substr(line.find(':') + 1) << "\"\n";
			//else continue;
		}

		string value = line.substr(line.find(':') + 1);
		if(value[0] == ' ')
			value = value.substr(1);

		if(kkey == "User-Agent") {

			this->agent = value;
		} else if(kkey == "DNT") {
			this->DNT = value == "1" ? 1 : 0;
		} else if(kkey == "Host") {
			this->host = value;
		}

		
	}

}

void HTTP::respond() {
	string res = "HTTP/1.1 " + to_string(this->status) + " " + get_message(this->status) + "\r\r";
	res += "Content-Type: " + this->content_type + "\r\n";
	res += "Server: " + this->server + "\r\n";
	res += "Content-Length: " + to_string(this->content_length) + "\r\n";
	res += "\r\n";
	//cout << res;
	send(this->socket, res.c_str(), res.length(), 0);
	send(this->socket, this->content, this->content_length, 0);


}


string HTTP::get_path() {
	return this->path;
}

string HTTP::get_host() {
	return this->host;
}

int HTTP::get_status() {
	return this->status;
}

void HTTP::set_status(int status) {
	this->status = status;
}

string HTTP::get_user_agent() {
	return this->agent;
}

bool HTTP::is_dnt() {
	return this->DNT;
}

string HTTP::get_content_type() {
	return this->content_type;
}

void HTTP::set_content_type(string type) {
	this->content_type = type;
}

string HTTP::get_server() {
	return this->server;
}

void HTTP::set_content(char* data, int length){
	this->content = data;
	this->content_length = length;
}

char* HTTP::get_content(){
	return this->content;
}

int HTTP::get_content_length(){
	return this->content_length;
}

