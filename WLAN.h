// File: WLAN.h
// Author: Michel Barbeau
// Version: January 16, 2016

// C Headers
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// C++ Headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Handler.h"

using namespace std;

// WLAN Class Definition
class WLAN {
public:
   // Constructor
   WLAN(string interface);
   // Destructor
   ~WLAN();
   // Initialize 
   bool init();
   // Send a frame
   bool send(char address[], char message[]);
   // Set a handler
   void setHandler(Handler* aHandler);
   // Receive a frame
   void receive();
private:
   // Constants
   static const int WLAN_ADDR_LEN = 6;
   static const int WLAN_HEADER_LEN = 14;
   static const unsigned short IP_TYPE = 0x3901;
   const string WLAN_BROADCAST = "ff:ff:ff:ff:ff:ff";
   // Structure of a network address
   struct WLANAddr{
      // address
      unsigned char data[WLAN_ADDR_LEN];
      // return the address in a human readable form
      char * wlan2asc(char str[]);
      // define the address from a human readable form
      int str2wlan(char s[]);
   };

   // Structure of a frame header
   struct WLANHeader{
       // destination address
       WLANAddr destAddr;
       // source address
       WLANAddr srcAddr;
       // type
       unsigned short type;
   };

   // Structure of a network interface configuration
   struct Ifconfig{
      // socket descriptor
      int sockid;
      // interface index
      int ifindex;
      // mac address
      WLANAddr hwaddr;
      // maximum transmission unit
      int mtu;
   };
   // convert a char to a hex digit
   static int hexdigit(char a);
   // convert an address string to a series of hex digits
   static int sscanf6(char str[], int *a1, int *a2, int *a3, int *a4, int *a5,
        int *a6);
   // Network interface label
   char* device;
   // Frame buffer
   unsigned char * buff;
   // Network interface configuration
   Ifconfig ifconfig;
   // Frame header
   WLANHeader hdr;
   // Handler of frame payload
   Handler* aHandler;
   // Initialization helpers
   bool createSocket();
   bool fetchInterfaceIndex();
   bool fetchHardwareAddress();
   bool fetchMTU(); 
   bool addPromiscuousMode(); 
   bool bindSocketToInterface(); 
   // Send helpers  
   void buildHeader(char address[], WLANAddr *daddr);
   void setToAddress(WLANAddr *daddr, struct sockaddr_ll *to);
   // Receive helper
   void parseReceivedFrame(char buff[]);
};
