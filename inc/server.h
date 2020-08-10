#include "socket.h"
#include "util.h"

class TCPServer : public TCPSocket {
  public:
    // Handles connection with client.
    TCPSocket conn;

    TCPServer() {};
    TCPServer(int port);
    virtual ~TCPServer() {};

    void send_type_length(const char type, const size_t length);
    void send_response(const JSON& response);

    void recv_type_length(char& type, size_t& length);
    void recv_request(JSON& request);
    void recv_hmac(JSON& hmac);
    void recv_audio(const std::string fname);
};
