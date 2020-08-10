#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "util.h"
#include <string>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define MAX_CONNECTIONS 1

class TCPSocketException {
  private:
    std::string err;

  public:
    TCPSocketException(std::string err_) : err(err_) {};
    ~TCPSocketException() {};

  std::string what() { return err; }
};

class TCPSocket {
  public:
    int sockfd;
    sockaddr_in addr;

    TCPSocket();
    virtual ~TCPSocket();

    // Server creates sockets, binds, listens, and accepts.
    bool create();
    bool bind(const int port);
    bool listen() const;
    bool accept(TCPSocket& sock) const;

    // Client just needs to connect.
    bool connect(const std::string ip, const int port);

    // Send data.
    void send(int fd, uint8_t *buf, int bytes) const;
    void recv(int fd, uint8_t *buf, int bytes) const;

    // Validate socket.
    virtual bool valid() const { return sockfd != -1; }
};

#endif
