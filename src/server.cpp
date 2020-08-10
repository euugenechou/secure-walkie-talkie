#include "server.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

TCPServer::TCPServer(int port) {
  if (!TCPSocket::create()) {
    throw TCPSocketException("Could not create server socket.");
  }

  if (!TCPSocket::bind(port)) {
    throw TCPSocketException("Could not bind server socket to port.");
  }

  if (!TCPSocket::listen()) {
    throw TCPSocketException("Could not make server socket listen.");
  }

  if (!TCPSocket::accept(conn)) {
    throw TCPSocketException("Could not accept client socket.");
  }
}

void TCPServer::send_type_length(const char type, const size_t length) {
  std::stringstream typelength;

  typelength << type;
  typelength << std::setw(8) << std::setfill('0') << length;

  std::string tmp = typelength.str();
  TCPSocket::send(conn.sockfd, (u8 *)tmp.c_str(), tmp.length());
  return;
}

void TCPServer::send_response(const JSON& response) {
  std::string tmp = response.dump();

  send_type_length(RESPONSE, tmp.length());

  TCPSocket::send(conn.sockfd, (u8 *)tmp.c_str(), tmp.length());
  return;
}

void TCPServer::recv_type_length(char& type, size_t& length) {
  char type_[2] = { 0 };
  TCPSocket::recv(conn.sockfd, (u8 *)&type_, 1);

  char length_[9] = { 0 };
  TCPSocket::recv(conn.sockfd, (u8 *)length_, 8);

  type = type_[0];

  try {
    length = std::stoi(std::string(length_));
  } catch (const std::invalid_argument& err) {
    std::cerr << "TCPClient error: " << err.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return;
}

void TCPServer::recv_request(JSON& request) {
  char type;
  size_t length;
  recv_type_length(type, length);

  char *value = new char[length + 1]();
  TCPSocket::recv(conn.sockfd, (u8 *)value, length);

  try {
    request = JSON::parse(std::string(value));
  } catch (JSON::parse_error& err) {
    std::cerr << "JSON error: " << err.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  delete[] value;
  return;
}

void TCPServer::recv_hmac(JSON& hmac) {
  char type;
  size_t length;
  recv_type_length(type, length);

  char *value = new char[length + 1]();
  TCPSocket::recv(conn.sockfd, (u8 *)value, length);

  try {
    hmac = JSON::parse(std::string(value));
  } catch (JSON::parse_error& err) {
    std::cerr << "JSON error: " << err.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  delete[] value;
  return;
}

void TCPServer::recv_audio(const std::string fname) {
  char type;
  size_t length;
  recv_type_length(type, length);

  u8 *audio = new u8[length]();
  CHECK(audio, "Failed to allocate memory for encrypted audio.");

  TCPSocket::recv(conn.sockfd, audio, length);

  std::ofstream audiofile(fname, std::ios::binary);
  CHECK(audiofile, "Failed to open audio file.");

  audiofile.write((char *)audio, length);

  audiofile.close();
  delete[] audio;
  return;
}
