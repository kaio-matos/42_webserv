#include <webserv.hpp>

// Socket tcp_socket("127.0.0.1", 8080);
// Socket peer_socket;
Socket<struct sockaddr_in> tcp_socket(AF_INET, SOCK_STREAM, 0);
Socket<struct sockaddr_in> peer_socket(AF_INET, SOCK_STREAM, 0);

void ctrl_c_handler(int s) {
  std::cout << "Closing sockets" << std::endl;
  exit(1);
}

int main() {
  signal(SIGINT, ctrl_c_handler);
  // signal(SIGABRT, ctrl_c_handler);

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

  std::string response = "Hello\n";
  peer_socket.write(response);

  return 0;
}
