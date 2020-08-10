#include "audio.h"
#include "bigint.h"
#include "protocol.h"
#include "server.h"
#include "util.h"
#include <fstream>
#include <unistd.h>

#define OPTIONS   "hvl:p:"

void print_usage(char *program) {
  std::cerr <<
    "SYNOPSIS\n"
    "   Receive and play decrypted audio.\n"
    "   Audio is encrypted and sent by the talker program.\n"
    "\n"
    "USAGE\n";
  std::cerr <<  "   " << program << " [-hv] [-p port]\n"
    "\n"
    "OPTIONS\n"
    "   -h        Display program help and usage\n"
    "   -v        Display verbose program output\n"
    "   -p port   Specify port to send audio to (default: 8123)";
  std::cerr << std::endl;
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  int port = 8123;
  std::string talker_file = "config/Eugene.json";
  std::string listener_file = "config/Eunice.json";

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      print_usage(argv[0]);
      return 0;
    case 'v':
      verbose = true;
      break;
    case 'l':
      listener_file = optarg;
      break;
    case 'p':
      port = std::stoi(std::string(optarg));
      break;
    default:
      print_usage(argv[0]);
      return 1;
    }
  }

  // Open up filestreams for talker and listener credentials.
  std::ifstream listener(listener_file);

  // Give Bob the listener credentials.
  JSON bob;
  listener >> bob;

  // The following is required to verify the request:
  //  - Bob's private RSA key (RSA d, RSA n)
  //  - Diffie-Hellman prime and generator (DH p, DH, g)
  //  - Bob's contacts
  BigInt bob_rsa_d = BigInt(bob["rsa_d"].get<std::string>());
  BigInt bob_rsa_n = BigInt(bob["rsa_n"].get<std::string>());
  BigInt dh_p = BigInt(bob["dh_p"].get<std::string>());
  BigInt dh_g = BigInt(bob["dh_g"].get<std::string>());
  JSON bob_contacts = bob["contacts"].get<JSON>();

  // Attempt communication with client.
  try {
    TCPServer server(port);
    LOG("Server started on port: " << port);

    // Receive request.
    JSON request;
    server.recv_request(request);
    LOG("Received request:\n" << std::setw(2) << request);

    // The following are retained from verifying the request:
    //  - ToD
    //  - Alice's public Diffie-Hellman key
    //  - Alice's public RSA key (RSA e, RSA n)
    BigInt tod;
    BigInt alice_dh_pub;
    BigInt alice_rsa_e;
    BigInt alice_rsa_n;

    // Verify the request.
    if (!protocol::verify_request(request, bob_rsa_d, bob_rsa_n, bob_contacts,
          tod, alice_dh_pub, alice_rsa_e, alice_rsa_n)) {
      std::cerr << "Error: invalid request" << std::endl;
      return 1;
    }

    LOG("Request verified.");

    // Two hash keys are retained from creating the response.
    BigInt k1;
    BigInt k2;

    // Create the response.
    JSON response = protocol::create_response(bob_rsa_d, bob_rsa_n, alice_rsa_e,
        alice_rsa_n, alice_dh_pub, dh_g, dh_p, tod, k1, k2);

    // Send the response.
    server.send_response(response);
    LOG("Sent response:\n" << std::setw(2) << response);

    // Receive the tag and audio.
    JSON tag;
    server.recv_hmac(tag);
    server.recv_audio("bob.encrypted");
    LOG("Received tag:\n" << std::setw(2) << tag);

    // Verify the tag.
    if (!protocol::verify_tag(tag, "bob.encrypted", k2)) {
      std::cerr << "Error: invalid tag" << std::endl;
      return 1;
    }

    // Decrypt the audio.
    protocol::decrypt_audio("bob.encrypted", "bob.wav", tod, k1);

    // Play the audio.
    play_audio("bob.wav");

    // Remove artifacts.
    remove("bob.encrypted");
    remove("bob.wav");
  } catch (TCPSocketException& err) {
    std::cerr << "Server error: " << err.what() << std::endl;
  }

  return 0;
}
