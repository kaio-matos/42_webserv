#ifndef SOCKET_H
#define SOCKET_H
#include <webserv.hpp>

template <typename T> class Socket {
public:
  Socket(int domain, int type, int protocol) {
    _domain = domain;
    _type = type;
    _protocol = protocol;
    _fd = socket(domain, type, protocol);
    _addr = NULL;

    _isOpen = false;
    _isListener = false;
    _isClosed = false;
  }

  Socket(const Socket &value) {}

  Socket &operator=(const Socket &value) { return *this; }

  ~Socket(void) {
    close();
    delete _addr;
    _addr = NULL;
  }

  // void bind(const struct sockaddr addr);
  void bind(const T addr) {
    _allocateAddr();
    // _addr->sin_family = addr.sin_family;
    // _addr->sin_addr.s_addr = addr.sin_addr.s_addr;
    // _addr->sin_port = addr.sin_port;
    memcpy(_addr, &addr, sizeof(addr));
    if (::bind(_fd, reinterpret_cast<struct sockaddr *>(_addr),
               sizeof(*_addr)) == -1) {
      throw std::runtime_error("Error while binding the socket");
    }
  }

  void connect(void) {}

  // TODO: This function is not properly made
  std::string read(void) {
    std::string result;
    char buff[30000];
    int bytes = ::read(_fd, buff, 30000);
    while (bytes) {
      buff[bytes] = '\0';
      if (bytes < 30000) {
        result.append(buff);
        bytes = 0;
      } else {
        bytes = ::read(_fd, buff, 30000);
      }
    }
    return result;
  }

  void write(std::string str) { ::write(_fd, str.c_str(), str.size()); }

  void close(void) {
    if (_fd != -1) {
      if (::close(_fd) == -1) {
        throw std::runtime_error("Error while closing the socket ");
      }
    }
  }

  void listen(int n) {
    if (::listen(_fd, n) == -1) {
      throw std::runtime_error("Error while setting socket as listener");
    }
    _isListener = true;
  }

  void accept(Socket &peer_socket) {
    if (!_isListener) {
      throw std::runtime_error(
          "Socket is trying to accept messages without setting listener flag");
    }
    peer_socket._allocateAddr();
    unsigned int peer_addr_size = sizeof(*peer_socket._addr);

    int cfd = ::accept(_fd, reinterpret_cast<sockaddr *>(peer_socket._addr),
                       &peer_addr_size);
    if (cfd == -1) {
      throw std::runtime_error("Error while trying to accept a request ");
    }

    peer_socket._fd = cfd;
  }

  bool isEmpty() const { return (_fd == -1 && _addr == NULL); }
  bool isOpen() const { return _isOpen; }
  bool isListener() const { return _isListener; }
  bool isClosed() const { return _isClosed; }

  int getFd() const { return _fd; }
  int getDomain() const { return _domain; }
  int getType() const { return _type; }
  int getProtocol() const { return _protocol; }
  T getRawAddr() const { return *_addr; }

private:
  int _fd;
  int _domain;
  int _type;
  int _protocol;
  T *_addr;

  bool _isOpen;
  bool _isListener;
  bool _isClosed;

  void _allocateAddr(void) {
    if (_addr == NULL) {
      _addr = new T;
    }
  }
};

#endif
