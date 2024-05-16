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

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>

#include <sstream>

#define SSTR(x)                                                                \
  static_cast<std::ostringstream &>((std::ostringstream() << std::dec << x))   \
      .str()

std::string readfd(int fd);
unsigned int stringAddressToBytes(std::string str);
std::string getAddressFromSockAddrin(const struct sockaddr_in addr);
std::string readFile(std::string filename, char separator);
std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to);

template <typename T> class Socket;
std::ostream &operator<<(std::ostream &os, const Socket<sockaddr_in> &value);

#include <Headers.hpp>
#include <Socket.hpp>

#endif
