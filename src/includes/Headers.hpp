#ifndef HEADERS_H
#define HEADERS_H
#include <webserv.hpp>

class Headers {
public:
  Headers(void);
  Headers(const Headers &value);
  Headers &operator=(const Headers &value);
  ~Headers(void);

  void append(std::string key, std::string value);
  std::string &operator[](std::string key);
  std::string toString() const;

private:
  std::map<std::string, std::string> _raw;
};

#endif
