#include "audio.h"
#include "bigint.h"
#include "client.h"
#include "protocol.h"
#include "util.h"
#include <fstream>
#include <unistd.h>

#define OPTIONS   "hvl:t:i:p:"

void print_usage(char *program) {
  std::cerr <<
    "SYNOPSIS\n"
    "   Record and send encrypted audio.\n"
    "   Audio is received and decrypted by the listener program.\n"
    "\n"
    "USAGE\n";
  std::cerr <<  "   " << program << " [-hv] [-t team] [-i ip] [-p port]\n"
    "\n"
    "OPTIONS\n"
    "   -h        Display program help and usage\n"
    "   -v        Display verbose program output\n"
    "   -i ip     Specify IP to send audio to (default: localhost)\n"
    "   -p port   Specify port to send audio to (default: 8123)";
  std::cerr << std::endl;
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  int port = 8123;
  std::string ip = "localhost";
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
    case 't':
      talker_file = optarg;
      break;
    case 'i':
      ip = optarg;
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
  std::ifstream talker(talker_file);
  std::ifstream listener(listener_file);

  // Give Alice the talker credentials.
  JSON alice;
  talker >> alice;

  // Give Bob the listener credentials.
  JSON bob;
  listener >> bob;

  // The following is required to create the request:
  //  - Alice's private RSA key (RSA d, RSA n)
  //  - Bob's public RSA key (RSA e, RSA n)
  //  - Diffie-Hellman prime and generator (DH p, DH, g)
  BigInt alice_rsa_d = BigInt(alice["rsa_d"].get<std::string>());
  BigInt alice_rsa_n = BigInt(alice["rsa_n"].get<std::string>());
  BigInt bob_rsa_e = BigInt(bob["rsa_e"].get<std::string>());
  BigInt bob_rsa_n = BigInt(bob["rsa_n"].get<std::string>());
  BigInt dh_p = BigInt(alice["dh_p"].get<std::string>());
  BigInt dh_g = BigInt(alice["dh_g"].get<std::string>());

  // The following are retained when creating the request:
  //  - ToD
  //  - Alice's private Diffie-Hellman key
  BigInt tod;
  BigInt alice_dh_pri;

  // Create the request; ToD and Alice's Diffie-Hellman private key are retained.
  JSON request = protocol::create_request(alice_rsa_d, alice_rsa_n, bob_rsa_e,
      bob_rsa_n, dh_g, dh_p, tod, alice_dh_pri);

  try {
    // Attempt communication with server.
    TCPClient client(ip, port);
    LOG("Connected to: " << ip << " on port: " << port);

    // Send the request.
    client.send_request(request);
    LOG("Sent request:\n" << std::setw(2) << request);

    // Receive the response.
    JSON response;
    client.recv_response(response);
    LOG("Received response:\n" << std::setw(2) << response);

    // Two hash keys are retained from creating the response.
    BigInt k1;
    BigInt k2;

    // Verify the response.
    if (!protocol::verify_response(response, alice_rsa_d, alice_rsa_n,
          bob_rsa_e, bob_rsa_n, dh_p, alice_dh_pri, tod, k1, k2)) {
      std::cerr << "Error: invalid response" << std::endl;
      return 1;
    }

    // Record audio.
    record_audio("alice.wav");

    // Encrypt the audio.
    protocol::encrypt_audio("alice.wav", "alice.encrypted", tod, k1);

    // Create tag.
    JSON tag = protocol::create_tag("alice.encrypted", k2);

    // Send the tag and audio.
    client.send_hmac(tag);
    client.send_audio("alice.encrypted");
    LOG("Sent tag:\n" << std::setw(2) << tag);

    // Remove artifacts.
    remove("alice.encrypted");
    remove("alice.wav");
  } catch (TCPSocketException& err) {
    std::cerr << "Client error: " << err.what() << std::endl;
  }

  return 0;
}
