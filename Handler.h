// File: Handler.h
// Author: Michel Barbeau
// Version: January 16, 2016

#ifndef HANDLER_H
#define HANDLER_H

#include <string>

using namespace std;

// Handler class declaration
class Handler {
public:
   // Handle message
   virtual void handle(char src[], char dst[], char msg[]);
};

#endif

