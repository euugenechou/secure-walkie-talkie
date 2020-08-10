#include "socket.h"
#include "util.h"

class TCPClient : public TCPSocket {
  public:
    TCPClient(std::string ip, int port);
    virtual ~TCPClient() {};

    void send_type_length(const char type, const size_t length);
    void send_request(const JSON& request);
    void send_hmac(const JSON& hmac);
    void send_audio(const std::string fname);

    void recv_type_length(char& type, size_t& length);
    void recv_response(JSON& response);
};
