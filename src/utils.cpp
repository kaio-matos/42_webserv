#include <webserv.hpp>

template <typename T> std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

// TODO: fix this
std::string readfd(int fd) {
  std::string result;
  char buff[100];
  int bytes = read(fd, buff, 100);
  while (bytes) {
    buff[bytes] = '\0';
    std::cout << "bytes " << bytes << " buff " << buff << std::endl;
    if (bytes < 100) {
      result.append(buff);
      bytes = 0;
    } else {
      bytes = read(fd, buff, 100);
    }
  }
  return result;
}

unsigned int stringAddressToBytes(std::string str) {
  int a, b, c, d;
  char arr[4];
  sscanf(str.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d);
  arr[0] = a;
  arr[1] = b;
  arr[2] = c;
  arr[3] = d;
  return *(unsigned int *)arr;
}

std::string getAddressFromSockAddrin(const struct sockaddr_in addr) {
  char host_buffer[NI_MAXHOST];

  if (int err = getnameinfo(reinterpret_cast<const struct sockaddr *>(&addr),
                            sizeof(addr), host_buffer, sizeof(host_buffer),
                            NULL, 0, NI_NAMEREQD)) {
    throw std::runtime_error(
        std::string("Could not resolve name: ").append(gai_strerror(err)));
  }
  return std::string(host_buffer);
}

std::ostream &operator<<(std::ostream &os, const Socket<sockaddr_in> &value) {
  os << "Socket In {\n"
     << "\tAddress fd: " << value.getFd() << "\n"
     << "\tPort:       " << ntohs(value.getRawAddr().sin_port) << "\n"
     << "\tAddress:   " << getAddressFromSockAddrin(value.getRawAddr())
     << "\n}";
  return os;
}
