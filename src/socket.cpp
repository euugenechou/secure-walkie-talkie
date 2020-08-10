#include "socket.h"
#include <cstring>
#include <unistd.h>

TCPSocket::TCPSocket() : sockfd(-1) {
  memset(&addr, 0, sizeof(addr));
}

TCPSocket::~TCPSocket() {
  if (valid()) {
    ::close(sockfd);
  }
}

bool TCPSocket::create() {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  return valid();
}

bool TCPSocket::bind(const int port) {
  if (!valid()) {
    return false;
  }

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);

  return ::bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) != -1;
}


bool TCPSocket::listen() const {
  if (!valid()) {
    return false;
  }

  return ::listen(sockfd, MAX_CONNECTIONS) != -1;
}

bool TCPSocket::accept(TCPSocket& sock) const {
  int addr_len = sizeof(addr);
  sock.sockfd = ::accept(sockfd, (sockaddr *) &addr, (socklen_t *)&addr_len);
  return sock.sockfd > 0;
}

void TCPSocket::send(int fd, uint8_t *buf, int nbytes) const {
  int bytes = 0;
  int total = 0;

  do {
    bytes = ::send(fd, buf + total, nbytes - total, 0);
    total += bytes;
  } while (bytes > 0 && total < nbytes);

  return;
}

void TCPSocket::recv(int fd, uint8_t *buf, int nbytes) const {
  int bytes = 0;
  int total = 0;

  do {
    bytes = ::recv(fd, buf + total, nbytes - total, 0);
    total += bytes;
  } while (bytes > 0 && total < nbytes);

  return;
}


bool TCPSocket::connect(const std::string ip, const int port) {
  if (!valid()) {
    return false;
  }

  addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) == -1) {
    return false;
  }

  if (::connect(sockfd, (sockaddr *)&addr, sizeof(addr)) == -1) {
    return false;
  }

  return true;
}
