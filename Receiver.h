// File: Receiver.h
// Author: Michel Barbeau
// Version: January 16, 2016

#include "Handler.h"

class Receiver: public Handler {
public:
   // Message handler
   virtual void handle(char src[], char dst[], char msg[]);
};

