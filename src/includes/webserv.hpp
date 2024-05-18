#ifndef WEBSERV_H
# define WEBSERV_H

# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <cerrno>
# include <fcntl.h>
# include <signal.h>
# include <stdlib.h>
# include <unistd.h>
# include <iostream>
# include <poll.h>
# include <sstream>
# include <stdexcept>
# include <stdio.h>
# include <Socket.hpp>


// Converte um número para string.
template <typename T> std::string NumberToString(T Number);

// Lê dados de um descritor de arquivo.
std::string readfd(int fd);

// Converte um endereço IP em string para um valor inteiro.
unsigned int stringAddressToBytes(std::string str);

// Obtém o endereço IP a partir de uma estrutura sockaddr_in.
std::string getAddressFromSockAddrin(const struct sockaddr_in addr);

// Declaração da classe template Socket.
template <typename T> class Socket;

// Sobrecarga do operador de inserção para imprimir informações sobre um socket.
std::ostream &operator<<(std::ostream &os, const Socket<sockaddr_in> &value);


#endif // Fim da guarda de inclusão.