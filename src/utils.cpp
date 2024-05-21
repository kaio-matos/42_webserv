#include <webserv.hpp>

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
    return "Not known";
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

std::string readFile(std::string filename, char separator) {
  std::ifstream file(filename.c_str());

  if (file.fail()) {
    std::string err = "Error opening the file: ";
    err.append(filename);
    file.close();
    throw std::runtime_error(err);
  }

  DIR *dir = opendir(filename.c_str());

  if (dir != NULL) {

    std::string err = "Error opening the file: ";
    err.append(filename);
    closedir(dir);
    throw std::runtime_error(err);
  }

  std::string line, result;
  unsigned int i = 0;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;

    if (i != 0) {
      result.append("\n");
    }
    result.append(line);
    i++;
  }

  file.close();
  return result;
}

std::string replaceAll(std::string str, const std::string &from,
                       const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length(); // Handles case where 'to' is a substring of 'from'
  }
  return str;
}
