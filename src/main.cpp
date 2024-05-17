#include <webserv.hpp>

void ctrl_c_handler(int s) {
  std::cout << "Closing sockets" << std::endl;
  exit(1);
}

std::string handleRequest(std::string request) {
  int found = request.find("SLOW");
  if (found != std::string::npos) {
    std::string index_html = readFile("assets/index_slow.html", '\n');
    index_html = replaceAll(index_html, "\n", "\r\n");
    return index_html;
  }
  std::string index_html = readFile("assets/index.html", '\n');
  index_html = replaceAll(index_html, "\n", "\r\n");
  return index_html;
}

int main() {
  signal(SIGINT, ctrl_c_handler);
  // signal(SIGABRT, ctrl_c_handler);
  Socket<struct sockaddr_in> tcp_socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = stringAddressToBytes("127.0.0.1");
  addr.sin_port = htons(8080);

  tcp_socket.bind(addr);
  tcp_socket.listen(5);

  while (1) {
    if (VERBOSE)
      std::cout << "---------------------------------------------" << std::endl;
    Socket<struct sockaddr_in> peer_socket(AF_INET, SOCK_STREAM, 0);
    tcp_socket.accept(peer_socket);

    std::string request = peer_socket.read();
    if (VERBOSE) {
      std::cout << "Reading peer socket" << std::endl;
      std::cout << request << std::endl;
    }

    std::string content = handleRequest(request);

    Headers headers;

    headers["Server"] = "webserv";
    headers["Content-Length"] = SSTR(content.size());
    headers["Content-Type"] = "text/html";
    headers["Connection"] = "Keep-Alive";

    std::string response = "HTTP/1.1 200 OK\r\n";
    response.append(headers.toString());
    response.append(content);

    if (VERBOSE) {
      std::cout << "Sending Response" << std::endl;
      std::cout << response << std::endl;
    }

    peer_socket.write(response);
    if (VERBOSE)
      std::cout << "---------------------------------------------" << std::endl;
  }
  return 0;
}
