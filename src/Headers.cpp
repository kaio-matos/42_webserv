/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers::Headers.hpp                                         :+:      :+:
 * :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: kmatos-s <kmatos-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 19:57:11 by kmatos-s          #+#    #+#             */
/*   Updated: 2024/05/08 20:37:32 by kmatos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
