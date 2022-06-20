#include "http_server.hh"

#include <vector>

#include <sys/stat.h>

#include <fstream>
#include <sstream>
#include <cstring>

int compute_fib(int); //function prototype
vector<string> split(const string &s, char delim) {
  vector<string> elems;

  stringstream ss(s);
  string item;

  while (getline(ss, item, delim)) {
    if (!item.empty())
      elems.push_back(item);
  }

  return elems;
}

HTTP_Request::HTTP_Request(string request) {
  vector<string> lines = split(request, '\n');
  vector<string> first_line = split(lines[0], ' ');

  this->HTTP_version = "HTTP/1.0"; // We'll be using 1.0 irrespective of the request

  /*
   TODO : extract the request method and URL from first_line here
  */
  this->method = first_line[0];
  this->url = first_line[1];

  if (this->method != "GET") {
    cerr << "Method '" << this->method << "' not supported" << endl;
    exit(1);
  }
}

HTTP_Response *handle_request(string req) {
  HTTP_Request *request = new HTTP_Request(req);

  HTTP_Response *response = new HTTP_Response();

  string resource = request->url;
  string url = string("html_files") + request->url;
  
  response->HTTP_version = "HTTP/1.0";

  struct stat sb;
  if (stat(url.c_str(), &sb) == 0) // requested path exists
  {
    response->status_code = "200";
    response->status_text = "OK";
    response->content_type = "text/html";

    string body;

    if (S_ISDIR(sb.st_mode)) {
      /*
      In this case, requested path is a directory.
      TODO : find the index.html file in that directory (modify the url
      accordingly)
      */
	url = url + "index.html";
	if (stat(url.c_str(), &sb) != 0) {
		response->status_code = "404";
		response->status_text = "Not Found";

		delete request;

		return response;
	}
    }

    /*
    TODO : open the file and read its contents
    */
	string filename(url);
	auto ss = ostringstream{};
	ifstream infile(filename);
	if (!infile.is_open()) {
		cerr << "Could not open file'" << filename << "'" << endl;
		exit(EXIT_FAILURE);
	}
	ss << infile.rdbuf();
	response->body = ss.str();

    /*
    TODO : set the remaining fields of response appropriately
    */
	response->content_length = to_string((response->body).size());
	cout << "response->content_length = " << response->body.size() << endl;
  }
  else {
    printf("url string name is %s\n", resource.c_str());
    const char *res_url = resource.substr(0, resource.find('?')+1).c_str();
    printf("resource string name is %s\n", res_url);
    if (!strcmp(res_url, "/hello?")) {
	response->status_code = "200";
	response->status_text = "OK";
	response->content_type = "text/html";
	string name = resource.substr(resource.find('?')+1); 
	response->body = "<html> <h2> Hello " + name + "!</h2> </html>";
  response->content_length = to_string((response->body).size());
  cout << "response->body = " << response->body << endl;
    }
    else if (!strcmp(res_url, "/fib?")) {
	response->status_code = "200";
	response->status_text = "OK";
	response->content_type = "text/html";
	string fib_n = resource.substr(resource.find('?')+1); 
	int n = atoi(fib_n.c_str());
	if (n < 0) {
		response->body = "<html> <h2> The number " + fib_n + " is invalid </h2> </html>";
	}
	else {
		int fibn = compute_fib(n);
		response->body = "<html> <h2> The " + fib_n + "th fibonacci number is " + to_string(fibn) + "</h2> </html>";
	}
  response->content_length = to_string((response->body).size());
  cout << "response->body = " << response->body << endl;
    }
    else {
        response->status_code = "404";
        /*
        TODO : set the remaining fields of response appropriately
        */
        response->status_text = "Not Found";

    }
	    
  }

  delete request;

  return response;
}

string HTTP_Response::get_string() {
  /*
  TODO : implement this function
  */
	char buf[1000];
	time_t now = time(0);
	struct tm tm = *gmtime(&now);
	strftime(buf, sizeof buf, "%a %d %b %Y %H:%M:%S %Z", &tm);
	return this->HTTP_version + " " + this->status_code + " " + this->status_text + "\r\n"
	        + "Date: " + buf + "\r\n"	
		+ "Content-Type: " + this->content_type + "\r\n" 
		+ "Content-Length: " + this->content_length + "\r\n\r\n"
		+ this->body;
 //return "";
}

int compute_fib(int n) {
	int f0 = 0;
	int f1 = 1;
	if (n == 0) return 0;
        if (n == 1) return 1;
	int f_n_2 = f0;
	int f_n_1 = f1;
	int i = 1;
	int f_n;
	while (n > i) {
		f_n = f_n_1 + f_n_2;
		f_n_2 = f_n_1;
		f_n_1 = f_n;
		i++;
	}
	return f_n;
}

