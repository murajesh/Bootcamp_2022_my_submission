#include "http_server.hh"

#include <vector>

#include <sys/stat.h>

#include <fstream>
#include <sstream>

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

  this->HTTP_version = "1.0"; // We'll be using 1.0 irrespective of the request

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

  string url = string("html_files") + request->url;

  response->HTTP_version = "1.0";

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
    response->status_code = "404";

    /*
    TODO : set the remaining fields of response appropriately
    */
    response->status_text = "Not Found";
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
