#include <webserv.hpp>

Socket<struct sockaddr_in> tcp_socket(AF_INET, SOCK_STREAM, 0);
Socket<struct sockaddr_in> peer_socket(AF_INET, SOCK_STREAM, 0);

void ctrl_c_handler(int s) {
  std::cout << "Closing sockets" << std::endl;
  exit(1);
}

int main() {
  signal(SIGINT, ctrl_c_handler);
  // signal(SIGABRT, ctrl_c_handler);

  std::string index_html = readFile("index.html", '\n');
  index_html = replaceAll(index_html, "\n", "\r\n");

  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = stringAddressToBytes("127.0.0.1");
  addr.sin_port = htons(8080);

  tcp_socket.bind(addr);
  tcp_socket.listen(5);
  tcp_socket.accept(peer_socket);

  std::cout << "Reading peer socket" << std::endl;
  std::string request = peer_socket.read();
  std::cout << request << std::endl;
  std::cout << "--------------------------------------------" << std::endl;

  std::map<std::string, std::string> headers;

  headers["Server"] = "webserv";
  headers["Content-Length"] = SSTR(index_html.size());
  headers["Content-Type"] = "text/html";
  headers["Connection"] = "Keep-Alive";

  std::string headersStr;
  for (std::map<std::string, std::string>::iterator it = headers.begin();
       it != headers.end(); it++) {
    headersStr.append(it->first);
    headersStr.append(": ");
    headersStr.append(it->second);
    headersStr.append("\r\n");
  }
  headersStr.append("\r\n");

  std::string response = "HTTP/1.1 200 OK\r\n";
  response.append(headersStr);
  response.append(index_html);

  std::cout << "Sending Response" << std::endl;
  std::cout << response << std::endl;
  std::cout << "--------------------------------------------" << std::endl;
  peer_socket.write(response);

  return 0;
}
