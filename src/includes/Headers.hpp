/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Headers.hpp                                         :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: kmatos-s <kmatos-s@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 19:57:11 by kmatos-s          #+#    #+#             */
/*   Updated: 2024/05/08 20:37:32 by kmatos-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
