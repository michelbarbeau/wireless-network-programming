// File: Receiver.cpp
// Author: Michel Barbeau
// Version: January 16, 2016

#include "Receiver.h"
#include "WLAN.h"
#include "label.h"

// Message handler
void Receiver::handle(char src[], char dst[], char msg[]) {
   cout << "from " << src << " to " << dst << ": " << msg << "\n";
}

int main(int argc, char ** argv) {
   // create a receiver
   Receiver* aReceiver=new Receiver();
   // create a wireless network abstraction
   WLAN* aWLAN=new WLAN(LABEL);
   // set the handler
   aWLAN->setHandler(aReceiver);
   // initialize 
   aWLAN->init();
   // start accepting frames
   aWLAN->receive();
   return 0;
}
