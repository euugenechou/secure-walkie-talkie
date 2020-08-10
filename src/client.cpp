#include "client.h"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>

TCPClient::TCPClient(std::string ip, int port) {
  if (!TCPSocket::create()) {
    throw TCPSocketException(strerror(errno));
  }

  if (!TCPSocket::connect(ip, port)) {
    throw TCPSocketException(strerror(errno));
  }
}

void TCPClient::send_type_length(const char type, const size_t length) {
  std::stringstream typelength;

  typelength << type;
  typelength << std::setw(8) << std::setfill('0') << length;

  std::string tmp = typelength.str();
  TCPSocket::send(sockfd, (u8 *)tmp.c_str(), tmp.length());
  return;
}

void TCPClient::send_request(const JSON& request) {
  std::string tmp = request.dump();

  send_type_length(REQUEST, tmp.length());

  TCPSocket::send(sockfd, (u8 *)tmp.c_str(), tmp.length());
  return;
}

void TCPClient::send_hmac(const JSON& hmac) {
  std::string tmp = hmac.dump();

  send_type_length(HMAC, tmp.length());

  TCPSocket::send(sockfd, (u8 *)tmp.c_str(), tmp.length());
  return;
}

void TCPClient::send_audio(const std::string fname) {
  std::stringstream tl;

  struct stat statbuf;
  stat(fname.c_str(), &statbuf);

  send_type_length(AUDIO, statbuf.st_size);

  u8 *audio = new u8[statbuf.st_size]();
  CHECK(audio, "Failed to allocate memory for encrypted audio.");

  std::ifstream audiofile(fname, std::ios::binary);
  CHECK(audiofile, "Failed to open audio file.");

  audiofile.read((char *)audio, statbuf.st_size);
  TCPSocket::send(sockfd, audio, statbuf.st_size);

  audiofile.close();
  delete[] audio;
  return;
}

void TCPClient::recv_type_length(char& type, size_t& length) {
  char type_[2] = { 0 };
  TCPSocket::recv(sockfd, (u8 *)&type_, 1);

  char length_[9] = { 0 };
  TCPSocket::recv(sockfd, (u8 *)length_, 8);

  type = type_[0];

  try {
    length = std::stoi(std::string(length_));
  } catch (const std::invalid_argument& err) {
    std::cerr << "TCPClient error: " << err.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return;
}

void TCPClient::recv_response(JSON& response) {
  char type;
  size_t length;
  recv_type_length(type, length);

  char *value = new char[length + 1]();
  TCPSocket::recv(sockfd, (u8 *)value, length);

  try {
    response = JSON::parse(std::string(value));
  } catch (JSON::parse_error& err) {
    std::cerr << "JSON error: " << err.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  delete[] value;
  return;
}
