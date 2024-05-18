/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmatos-s <kmatos-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 19:57:11 by kmatos-s          #+#    #+#             */
/*   Updated: 2024/05/08 20:37:32 by kmatos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_H
#define SOCKET_H

#include <webserv.hpp>

template <typename T>
class Socket {
public:

  // Construtor que inicializa um socket com domínio, tipo e protocolo especificados.
  Socket(int domain, int type, int protocol) {
    _domain = domain;
    _type = type;
    _protocol = protocol;
    _fd = socket(domain, type, protocol); // Cria o socket.
    _addr = NULL;

    _isOpen = false;
    _isListener = false;
    _isClosed = false;
  }

  // Construtor de cópia.
  Socket(const Socket &value) {}

  // Operador de atribuição.
  Socket &operator=(const Socket &value) { return *this; }

  // Destrutor que fecha o socket e libera a memória alocada para o endereço.
  ~Socket(void) {
    close();
    delete _addr;
    _addr = NULL;
  }

  // Método para associar o socket a um endereço.
  void bind(const T addr) {
    _allocateAddr();
    memcpy(_addr, &addr, sizeof(addr)); // Copia o endereço fornecido para o endereço interno.
    if (::bind(_fd, reinterpret_cast<struct sockaddr *>(_addr), sizeof(*_addr)) == -1) {
      throw std::runtime_error("Error while binding the socket"); // Lança exceção se ocorrer um erro.
    }
  }

  void connect(void) {}

  // Método para ler dados do socket. (Ainda precisa de melhorias como indicado pelo TODO)
  std::string read(void) {
    std::string result;
    char buff[100];
    int bytes = ::read(_fd, buff, 100);
    while (bytes) {
      buff[bytes] = '\0';
      if (bytes < 100) {
        result.append(buff);
        bytes = 0;
      } else {
        bytes = ::read(_fd, buff, 100);
      }
    }
    return result;
  }

  // Método para escrever dados no socket.
  void write(std::string str) { ::write(_fd, str.c_str(), str.size()); }

  // Método para fechar o socket.
  void close(void) {
    if (_fd != -1) {
      if (::close(_fd) == -1) {
        throw std::runtime_error("Error while closing the socket ");
      }
    }
  }

  // Método para colocar o socket em modo de escuta.
  void listen(int n) {
    if (::listen(_fd, n) == -1) {
      throw std::runtime_error("Error while setting socket as listener");
    }
    _isListener = true;
  }

  // Método para aceitar uma conexão de entrada.
  int accept(Socket &peer_socket) {
    if (!_isListener) {
      throw std::runtime_error(
          "Socket is trying to accept messages without setting listener flag");
    }
    peer_socket._allocateAddr();
    unsigned int peer_addr_size = sizeof(*peer_socket._addr);

    int cfd = ::accept(_fd, reinterpret_cast<sockaddr *>(peer_socket._addr), &peer_addr_size);
    if (cfd == -1) {
      throw std::runtime_error("Error while trying to accept a request ");
    }

    peer_socket._fd = cfd;
  }

  // Métodos para verificar o estado do socket.
  bool isEmpty() const { return (_fd == -1 && _addr == NULL); }
  bool isOpen() const { return _isOpen; }
  bool isListener() const { return _isListener; }
  bool isClosed() const { return _isClosed; }

  // Métodos para obter informações sobre o socket.
  int getFd() const { return _fd; }
  int getDomain() const { return _domain; }
  int getType() const { return _type; }
  int getProtocol() const { return _protocol; }
  T getRawAddr() const { return *_addr; }

private:
  int _fd;         // File descriptor do socket.
  int _domain;     // Domínio do socket (ex: AF_INET).
  int _type;       // Tipo do socket (ex: SOCK_STREAM).
  int _protocol;   // Protocolo do socket (ex: 0 para padrão).
  T *_addr;        // Ponteiro para a estrutura de endereço.

  bool _isOpen;    // Indica se o socket está aberto.
  bool _isListener; // Indica se o socket está em modo de escuta.
  bool _isClosed;   // Indica se o socket está fechado.

  // Método privado para alocar memória para o endereço se ainda não estiver alocada.
  void _allocateAddr(void) {
    if (_addr == NULL) {
      _addr = new T;
    }
  }
};

#endif // Fim da guarda de inclusão.