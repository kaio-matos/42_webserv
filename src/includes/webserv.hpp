#ifndef WEBSERV_H
#define WEBSERV_H

#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include <sys/socket.h>
#include <sys/un.h>

#include <cerrno>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

template <typename T> std::string NumberToString(T Number);
std::string readfd(int fd);
unsigned int stringAddressToBytes(std::string str);
std::string getAddressFromSockAddrin(const struct sockaddr_in addr);

template <typename T> class Socket;
std::ostream &operator<<(std::ostream &os, const Socket<sockaddr_in> &value);

#include <Socket.hpp>

class Address {
public:
  Address(void);
  Address(std::string address, int port);
  Address(const Address &value);
  Address &operator=(const Address &value);
  ~Address(void);

  void open(void);
  void close(void);
  int accept(Address &peer_socket);

  struct sockaddr_in getRawBoundAddress(void) const;
  int getSocketfd(void) const;
  std::string getBoundAddress(void) const;
  int getPort(void) const;

private:
  struct sockaddr_in *_address;
  int _socketfd;

  unsigned int _stringAddressToBytes(std::string str) const;
};

#endif
