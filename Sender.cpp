// File: Sender.cpp
// Author: Michel Barbeau
// Version: January 16, 2016

#include "Sender.h"
#include "WLAN.h"
#include "label.h"

using namespace std;

int main(int argc, char ** argv) {
   // create a sender
   Sender* aSender=new Sender();
   // create a wireless network abstraction
   WLAN* aWLAN=new WLAN(LABEL);
   // initialize 
   aWLAN->init();
   // send a frame
   char a[]="ff:ff:ff:ff:ff:ff"; // broadcast address
   // char a[]="1c:bd:b9:7e:b6:5a"; // unicast address
   char f[]="This is a test frame"; // data
   aWLAN->send(a, f);
   return 0;
}
