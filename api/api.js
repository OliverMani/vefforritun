const http = require('http');
const fs = require('fs');

const port = 8080;
const hostname = '0.0.0.0';

const CONTENT_TYPES = {
	'.css':'text/css',
	'.js':'application/javascript',
	'.html':'text/html',
	'.ico':'image/x-icon',
	'.png':'image/png',
	'.jpg':'image/jpg',
	'.jpeg':'image/jpeg',
	'.gif':'image/gif',
	'.svg':'image/svg+xml'
}

// Hér er API'inn
function getAPI(path) {
	let obj = {};
	let list = path.split('/');
	console.log("LIST:",list);

	// ====> HÉR VERÐUR KANNSKI MONGODB TENGING <====
	if(list[0] == "books") {
		obj.books = ["Bók 1"];
	}

	return JSON.stringify(obj);
}

// Basic Web Server
const server = http.createServer((req, res) => {
	if(req.url == '/') {
		
		res.setHeader('Content-Type', 'text/html');
		res.statusCode = 200;
		fs.readFile('www/index.html', function(err, data) {
			if(err == null)
				res.end(data)
			else {
				res.statusCode = 404;
				res.end("404 not found!");
			}
		});
	} else if(req.url.startsWith('/static/')) {
		const path = req.url.substring(8);
		
		if(path.includes('/')) {
			res.statusCode = 400;
			res.setHeader('Content-Type', 'text/plain');
			res.end('400 bad request!');
		} else {
			fs.readFile('www/' + path, function(err, data){
				if(err) {
					res.statusCode = 404;
					res.setHeader('Content-Type', 'text/plain');
					res.end("404 not found!");
				} else {
					res.statusCode = 200;
					if(path.endsWith('.js')) {
						res.setHeader('Content-Type', 'application/javascript');
					} else if(path.endsWith('.css')) {
						res.setHeader('Content-Type', 'text/css');
					} else if(path.endsWith('.html')) {
						res.setHeader('Content-Type', 'text/html');
					} else if(path.endsWith('.ico')) {
						res.setHeader('Content-Type', '')
					}
					res.end(data);
				}
			});
		}
	} else if(req.url.startsWith('/api/')) {
		res.statusCode = 200;
		res.setHeader('Content-Type', 'application/json');
		res.end(getAPI(req.url.substring(5)));
	} else {
		res.statusCode = 404;
		res.setHeader('Content-Type', 'text/plain');
		res.end("404 not found!");
	}
});

server.listen(port, hostname, () => {});