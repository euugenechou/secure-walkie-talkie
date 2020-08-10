#include "audio.h"
#include "ctr.h"
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void record_audio(std::string fname) {
  // ":1" refers to the microphone device on macOS (depends on system).
  std::string c = "ffmpeg -y -f avfoundation -i \":1\" " + fname + " &> /dev/null";

  // Simulate a button.
  std::cout << "Press <enter> to start recording. ";
  std::cin.ignore();

  // Execute the command.
  std::cout << "Recording audio (press <q> to stop)." << std::endl;
  system(c.c_str());

  return;
}

void play_audio(std::string fname) {
  // Format command to ffplay, volume can be changed.
  std::string c = "ffplay -nodisp -autoexit " + fname + " &> /dev/null";

  // Execute the command.
  std::cout << "Playing audio (will automatically stop)." << std::endl;
  system(c.c_str());

  return;
}
