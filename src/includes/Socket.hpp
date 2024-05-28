#ifndef SOCKET_H
#define SOCKET_H
#include <webserv.hpp>

template <typename T> class Socket {
public:
  Socket(void) {
    _domain = -1;
    _type = -1;
    _protocol = -1;
    _fd = -1;
    _addr = NULL;

    _isOpen = false;
    _isListener = false;
    _isClosed = false;
    _isClient = true;
    _isServer = false;
  }

  Socket(int domain, int type, int protocol) {
    _domain = domain;
    _type = type;
    _protocol = protocol;
    _fd = socket(domain, type, protocol);
    _addr = NULL;

    _isOpen = false;
    _isListener = false;
    _isClosed = false;
    _isClient = false;
    _isServer = true;

    _setSocketAsReusable(_fd);
    _setSocketAsNonBlocking(_fd);
  }

  Socket(const Socket &value) { *this = value; }

  Socket &operator=(const Socket &value) {
    _domain = value._domain;
    _type = value._type;
    _protocol = value._protocol;
    _fd = value._fd;
    _addr = NULL;
    if (value._addr != NULL) {
      _allocateAddr();
      memcpy(_addr, &value._addr, sizeof(value._addr));
    }

    _isOpen = value._isOpen;
    _isListener = value._isListener;
    _isClosed = value._isClosed;
    _isClient = value._isClient;
    _isServer = value._isServer;
    return *this;
  }

  ~Socket(void) {
    // TODO: This was commented due to issues with std::vector, is there another
    // option?

    // close();
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

  std::string read(std::string eof) {
    std::string result;
    int MAX_READ_BYTES = 100;
    char buff[MAX_READ_BYTES];

    int bytes = ::read(_fd, buff, MAX_READ_BYTES);
    if (bytes == -1) {
      throw std::runtime_error("Error while trying to read socket");
    }
    while (bytes) {
      std::string buffer(buff, bytes);
      result.append(buffer);

      if (buffer.substr(bytes - eof.length(), eof.length()) == eof) {
        break;
      }

      bytes = ::read(_fd, buff, MAX_READ_BYTES);
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
    if (!_isServer) {
      throw std::runtime_error(
          "Error while trying to listen on a non server socket");
    }
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
    if (!_isServer) {
      throw std::runtime_error(
          "Error: trying to accept on a non server socket");
    }

    peer_socket._allocateAddr();
    unsigned int peer_addr_size = sizeof(*peer_socket._addr);

    int cfd = ::accept(_fd, reinterpret_cast<sockaddr *>(peer_socket._addr),
                       &peer_addr_size);
    if (cfd == -1) {
      throw std::runtime_error("Error while trying to accept a request ");
    }

    // TODO: check if is necessary to set the accepted socket as reusable and
    // non-blocking
    //
    // _setSocketAsReusable(cfd);
    // _setSocketAsNonBlocking(cfd);
    peer_socket._fd = cfd;
  }

  bool isEmpty() const { return (_fd == -1 && _addr == NULL); }
  bool isOpen() const { return _isOpen; }
  bool isListener() const { return _isListener; }
  bool isClosed() const { return _isClosed; }
  bool isClient() const { return _isClient; }
  bool isServer() const { return _isServer; }

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
  bool _isClient;
  bool _isServer;

  void _allocateAddr(void) {
    if (_addr == NULL) {
      _addr = new T;
    }
  }

  void _setSocketAsReusable(int fd) {
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
      throw std::runtime_error("Error while setting socket as reusable");
    }
  }

  void _setSocketAsNonBlocking(int fd) {
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
      throw std::runtime_error("Error while setting socket as non-blocking");
    }
  }
};

#endif
