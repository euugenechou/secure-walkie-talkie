# C++ Implementation

The C++ implementation of the secure walkie-talkie.

### Design

Two binaries: ```talker``` and ```listener```. The talker, through an extensive
protocol, securely sends the listener audio. The protocol is comprised of RSA
signing, Diffie-Hellman key exchange, and SHA-3 hash-based tagging and
verification.

### Program Dependencies

All program dependencies can be installed on macOS with Homebrew.

 - ```ffmpeg```
 - ```nlohmann-json```
 - ```GMP```

If you have some other non-macOS system, these dependencies still work, but need
to be installed through other means. In addition, a line of code will need to
change depending on your system in ```audio.cpp``` regarding the call to
```ffmpeg```.

### Building

To build both the talker and listener:

    $ make

Or:

    $ make all

To build just the listener:

    $ make listener

To build just the talker:

    $ make talker

### Usage

Start up ```listener``` in one terminal instance then ```talker``` in another.
