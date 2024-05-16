#include <webserv.hpp>

Headers::Headers(void) {}
Headers::Headers(const Headers &value) { _raw = value._raw; }
Headers &Headers::operator=(const Headers &value) { return *this; }
Headers::~Headers(void) { _raw.clear(); }

void Headers::append(std::string key, std::string value) { _raw[key] = value; }

std::string &Headers::operator[](std::string key) { return _raw[key]; }

std::string Headers::toString() const {
  std::string headersStr;
  for (std::map<std::string, std::string>::const_iterator it = _raw.begin();
       it != _raw.end(); it++) {
    headersStr.append(it->first);
    headersStr.append(": ");
    headersStr.append(it->second);
    headersStr.append("\r\n");
  }
  headersStr.append("\r\n");
  return headersStr;
}
