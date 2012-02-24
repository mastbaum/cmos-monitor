#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <avalanche.hpp>
#include <iostream>
#include "PackedEvent.hh"

#define CRATES 19
#define SLOTS 8
#define CHANNELS 32

int main(int argc, char* argv[]) {
  avalanche::server serv("tcp://*:5024");
  srand(time(NULL));

  if (argc == 1)
    return 1;

  if (strcmp(argv[1], "noise") == 0) {
      std::cout << "make some noise..." << std::endl;
    while(1) {
      for (int icrate=0; icrate<CRATES; icrate++) {
        SNOT::CMOSRate r;
        r.errorflags = 0;
        r.crate = icrate;
        for (int islot=0; islot<SLOTS; islot++) {
         for (int ichannel=0; ichannel<CHANNELS; ichannel++) {
            r.rates[32*islot + ichannel] = rand() % 600;
          }
        }
        serv.sendObject(&r);
        usleep(10000000);
      }
    }
  }
  else if (strcmp(argv[1], "song") == 0) {
    std::cout << "bwahahaha!" << std::endl;
    const int n = 10;
    float notes[n] = {0.0, 880.0, 783.99, 880.0, 783.99, 698.46, 659.26, 587.33, 554.37, 587.33};
    float times[n] = {1, 0.5, 0.75, 2, 0.5, 0.5, 0.5, 0.5, 2, 3};

    for (int i=0; i<n; i++) {
      SNOT::CMOSRate r;
      r.errorflags = 0;
      r.crate = 0;
      memset(&(r.rates), 0, 32*16*sizeof(float));
      r.rates[0] = notes[i];
      std::cout << "sending" << std::endl;
      serv.sendObject(&r);
      usleep(600000 * times[i]);
    }

    SNOT::CMOSRate r;
    r.errorflags = 0;
    r.crate = 0;
    memset(&(r.rates), 0, 32*16*sizeof(float));
    std::cout << "sending" << std::endl;
    serv.sendObject(&r);

  }
  else if (strcmp(argv[1], "tone") == 0) {
    std::cout << "tone" << std::endl;
    SNOT::CMOSRate r;
    r.errorflags = 0;
    r.crate = 0;
    memset(&(r.rates), 0, 32*16*sizeof(r.rates[0]));
    r.rates[0] = 440;
    std::cout << "sending" << std::endl;
    serv.sendObject(&r);
  }
  else if (strcmp(argv[1], "crates") == 0) {
    std::cout << "crates" << std::endl;
    for (unsigned short i=0; i<CRATES; i++) {
      SNOT::CMOSRate r;
      r.errorflags = 0;
      r.crate = i;
      memset(&(r.rates), 0, 32*16*sizeof(r.rates[0]));
      r.rates[5] = 110 * i;
      std::cout << "sending" << std::endl;
      serv.sendObject(&r);
      usleep(600000);
    }
  }

  return 0;
}

