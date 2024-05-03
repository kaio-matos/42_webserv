#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <webserv.hpp>

#include <cerrno>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

template <typename T> std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

std::string inet_addr_str(const struct sockaddr_in *addr, socklen_t addrlen) {
  char host_buffer[NI_MAXHOST];

  if (getnameinfo(reinterpret_cast<const struct sockaddr *>(addr), addrlen,
                  host_buffer, sizeof(host_buffer), NULL, 0, NI_NAMEREQD)) {
    std::cerr << "Could not resolve name" << std::endl;
  }
  return std::string(host_buffer);
}

unsigned int inet_addr(char *str) {
  int a, b, c, d;
  char arr[4];
  sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d);
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
  return *(unsigned int *)arr;
}

std::ostream &operator<<(std::ostream &os, const struct sockaddr_in &value) {
  os << "sockaddr_in {\n"
     << "\tPort:    " << value.sin_port << "\n"
     << "\tAddress: " << inet_addr_str(&value, sizeof(value)) << "\n"
     << "\tZero:    " << value.sin_zero << "\n"
     << "\tFamily:  " << value.sin_family << "\n}";
  return os;
}

int main() {
  struct sockaddr_in *my_addr;
  struct in_addr *address;
  int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);

  my_addr = new sockaddr_in;
  address = new in_addr;
  address->s_addr = inet_addr("127.0.0.1");
  my_addr->sin_family = AF_INET;
  my_addr->sin_addr = *address;
  my_addr->sin_port = htons(8080);
  if (bind(tcp_socket, reinterpret_cast<sockaddr *>(my_addr),
           sizeof(*my_addr)) == -1) {
    std::cerr << "Error while binding socket " << errno << std::endl;
  }
  std::cout << *my_addr << std::endl;
  // listen(tcp_socket);
  // std::cout << tcp_socket << std::endl;
  return 0;
}
