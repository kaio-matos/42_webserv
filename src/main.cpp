#include <webserv.hpp>

void ctrl_c_handler(int s) {
  DebugLog << "Closing sockets";
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

void listenToRequests(Socket<struct sockaddr_in> &tcp_socket) {
  Socket<struct sockaddr_in> peer_socket(AF_INET, SOCK_STREAM, 0);

  std::vector<Socket<struct sockaddr_in> > sockets;

  sockets.push_back(tcp_socket);
  sockets.push_back(peer_socket);

  int num_fds = sockets.size();
  struct pollfd poll_fds[num_fds];
  int timeout = (5 * 60 * 1000); // 5 minute (in milliseconds)

  while (1) {
    DebugLog << "---------------------------------------------";

    for (int i = 0; i < num_fds; i++) {
      poll_fds[i].fd = sockets[i].getFd();
      poll_fds[i].events = POLLIN;
    }

    int ret = poll(poll_fds, num_fds, timeout);
    if (ret < 0) {
      throw new std::runtime_error("poll error");
    } else if (ret == 0) {
      DebugLog << BOLDBLUE << "Poll timed out";
      continue;
    }

    for (int i = 0; i < num_fds; i++) {
      Socket<struct sockaddr_in> peer_socket = sockets[i];

      if (poll_fds[i].revents & POLLIN) {
        tcp_socket.accept(peer_socket);

        std::string request = peer_socket.read("\r\n\r\n");
        DebugLog << "Reading peer socket";
        DebugLog << request;

        std::string content = handleRequest(request);

        Headers headers;

        headers["Server"] = "ebserv";
        headers["Content-Length"] = SSTR(content.size());
        headers["Content-Type"] = "text/html";
        headers["Connection"] = "Keep-Alive";

        std::string response = "HTTP/1.1 200 OK\r\n";
        response.append(headers.toString());
        response.append(content);

        DebugLog << "Sending Response";
        DebugLog << response;

        peer_socket.write(response);
        peer_socket.close();
      }
    }

    DebugLog << "---------------------------------------------";
  }
}

int main() {
  DebugLog << BOLDCYAN
           << "-------------------------- Starting webserv "
              "--------------------------";
  signal(SIGINT, ctrl_c_handler);
  // signal(SIGABRT, ctrl_c_handler);
  Socket<struct sockaddr_in> tcp_socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = stringAddressToBytes("0.0.0.0");
  addr.sin_port = htons(8080);

  tcp_socket.bind(addr);
  tcp_socket.listen(5);

  DebugLog << "Listening on:\n" << tcp_socket;

  listenToRequests(tcp_socket);

  tcp_socket.close();

  return 0;
}
