// File: WLAN.cpp
// Author: Michel Barbeau
// Version: January 16, 2016

#include "WLAN.h"

// Return the address in a human readable form
char * WLAN::WLANAddr::wlan2asc(char str[]) {
   sprintf(str, "%x:%x:%x:%x:%x:%x",
      data[0],data[1],data[2],data[3],data[4],data[5]);
   return str;
}

// Convert a char to a hex digit
int WLAN::hexdigit(char a) {
  if (a >= '0' && a <= '9') return(a-'0');
  if (a >= 'a' && a <= 'f') return(a-'a'+10);
  if (a >= 'A' && a <= 'F') return(a-'A'+10);
  return -1;
}

// convert an address string to a series of hex digits
int WLAN::sscanf6(char str[], int *a1, int *a2, int *a3, int *a4, int *a5, int *a6) {
  int n;
  *a1 = *a2 = *a3 = *a4 = *a5 = *a6 = 0;
  while ((n=hexdigit(*str))>=0)
    (*a1 = 16*(*a1) + n, str++);
  if (*str++ != ':') return 1;
  while ((n=hexdigit(*str))>=0)
    (*a2 = 16*(*a2) + n, str++);
  if (*str++ != ':') return 2;
  while ((n=hexdigit(*str))>=0)
    (*a3 = 16*(*a3) + n, str++);
  if (*str++ != ':') return 3;
  while ((n=hexdigit(*str))>=0)
    (*a4 = 16*(*a4) + n, str++);
  if (*str++ != ':') return 4;
  while ((n=hexdigit(*str))>=0)
    (*a5 = 16*(*a5) + n, str++);
  if (*str++ != ':') return 5;
  while ((n=hexdigit(*str))>=0)
    (*a6 = 16*(*a6) + n, str++);
  return 6;
}

// Define the address from a human readable form
int WLAN::WLANAddr::str2wlan(char s[]) {
   int a[6], i;
   // parse the address
   if (sscanf6(s, a, a+1, a+2, a+3, a+4, a+5) < 6) {
      return -1;
   }
   // make sure the value of every component does not exceed on byte
   for (i=0; i < 6; i++) {
      if (a[i] > 0xff) return -1;
   }
   // assign the result to the member "data"
   for (i=0; i < 6; i++) { 
      data[i]=a[i];
   }
   return 0;
}

// Constructor
WLAN::WLAN(string interface) {
  // Set device name
  device = new char[interface.length()+1];
  device[interface.length()] = 0;
  memcpy(device, interface.c_str(), interface.length());
}

// Set message handler
void WLAN::setHandler(Handler* aHandler) {
  this->aHandler=aHandler;
}

bool WLAN::createSocket() {
   // Create device level socket
   // - AF_PACKET : packet interface on device level
   // - SOCK_RAW : raw packets including link level header
   // create the socket
   if ((ifconfig.sockid=socket(AF_PACKET, SOCK_RAW, 0)) == -1) {
      cerr << "Cannot open socket: " << strerror(errno) << "\n"; 
      return false;
   }
   cerr << "socket created!\n";
   return true;
}

bool WLAN::fetchInterfaceIndex() {
   // fetch the interface index
   struct ifreq ifr;
   strcpy(ifr.ifr_name, device);
   if (ioctl(ifconfig.sockid, SIOGIFINDEX, &ifr) < 0) {
      cerr << "Failed to fetch ifindex: " << strerror(errno) << "\n"; 
      return false;
   }
   ifconfig.ifindex=ifr.ifr_ifindex;
   cerr << "ifindex is: " << ifr.ifr_ifindex << "\n";
   return true;
}

bool WLAN::fetchHardwareAddress() {
   // fetch the hardware address
   struct ifreq ifr;
   if (ioctl(ifconfig.sockid, SIOCGIFHWADDR, &ifr) == -1) {
      cerr << "Failed to fetch hardware address: " << strerror(errno) << "\n"; 
      return false;
   }
   memcpy(&ifconfig.hwaddr.data, &ifr.ifr_hwaddr.sa_data, WLAN_ADDR_LEN);
   char * addr=new char[32];
   cerr << "hardware address is: " << ifconfig.hwaddr.wlan2asc(addr) << "\n";
   delete addr;
   return true;
}

bool WLAN::fetchMTU() {
   // fetch the MTU
   struct ifreq ifr;
   if (ioctl(ifconfig.sockid, SIOCGIFMTU, &ifr) == -1) {
      cerr << "Failed to get the MTU: " << strerror(errno) << "\n"; 
      return false;
   }
   ifconfig.mtu=ifr.ifr_mtu;
   cerr << "MTU is: " << ifr.ifr_mtu << "\n";
   return true;
}

bool WLAN::bindSocketToInterface() {
   // bind the socket to the interface
   // only traffic from that interface will be received
   struct sockaddr_ll sll;
   memset(&sll, 0, sizeof(sll));
   sll.sll_family=AF_PACKET;
   sll.sll_ifindex=ifconfig.ifindex;
   sll.sll_protocol=htons(ETH_P_ALL);
   if (bind(ifconfig.sockid, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
      cerr << "Failed to bind the socket: " << strerror(errno) << "\n"; 
      return false;
   }
   cerr << "socket bind done\n";
   return true;
}

// Init
bool WLAN::init() {
   cout << "Initializing network interface  : " << device << "\n";
   // create the socket
   if (!createSocket()) 
      return false;
   else if (!fetchInterfaceIndex())
      return false;
   else if (!fetchHardwareAddress())
      return false;
   else if (!fetchMTU())
      return false;
   else if (!bindSocketToInterface())
      return false;
   else
      return true;
}

// build a frame header
void WLAN::buildHeader(char address[], WLANAddr *daddr) {
   // conversion of  destination address from ASCII to binary
   daddr->str2wlan(address);
   // store the destination address
   memmove(&hdr.destAddr, daddr->data, WLAN_ADDR_LEN);
   // store the source address
   memmove(&hdr.srcAddr, ifconfig.hwaddr.data, WLAN_ADDR_LEN);
   // set the type field
   hdr.type=htons(IP_TYPE);
}

// set the "to address"
void WLAN::setToAddress(WLANAddr *daddr, struct sockaddr_ll *to) {  
   to->sll_family = AF_PACKET;
   to->sll_ifindex = ifconfig.ifindex;
   memmove(&(to->sll_addr), daddr->data, WLAN_ADDR_LEN);
   to->sll_halen=6;
}

// Send
bool WLAN::send(char address[], char message[]) {
   // send buffer
   char buff[WLAN_HEADER_LEN+strlen(message)];
   // destination address
   WLANAddr daddr;
   // build the header
   buildHeader(address, &daddr);
   // store the header into the frame
   memmove(buff, &hdr, WLAN_HEADER_LEN);
   // encapsulate the message into the frame
   memmove(buff+WLAN_HEADER_LEN, message, strlen(message));
   // set the "to address"
   struct sockaddr_ll to = {0};
   setToAddress(&daddr, &to);
   // send a frame
   int sentlen=sendto(
      ifconfig.sockid, buff, WLAN_HEADER_LEN+strlen(message), 0,
         (sockaddr *) &to, sizeof(to));
   // Check errors
   if (sentlen == -1 ) {
      cerr << "WLAN::sendto() failed: " << strerror(errno) << "\n"; 
      return false;
   }
   return true;
}

// Parse a received frame
void WLAN::parseReceivedFrame(char buff[]) {
   // casting to the WLAN header format
   WLANHeader * wlanHdr = (WLANHeader *) buff;
   // get gestination in ascii
   char *dst=new char[32];
   wlanHdr->destAddr.wlan2asc(dst);
   // get source in ascii
   char *src=new char[32];
   wlanHdr->srcAddr.wlan2asc(src);
   // get my address in ascii
   char * myaddress=new char[32];
   // check destination
   if (strcmp(dst,ifconfig.hwaddr.wlan2asc(myaddress))==0 || 
      dst==WLAN_BROADCAST) {
      // destination address is self or broadcast
      if (aHandler!=0) {
         // call the handler for processing that frame
         aHandler->handle(src, dst, buff + sizeof(WLANHeader));
      }
   }
   delete dst;
   delete src;
   delete myaddress;
}

// Receive
void WLAN::receive() {
   // buffer for received frame
   char * buff=new char[ifconfig.mtu];
   // length of received frame
   unsigned int i; // frame length
   // src address of frame
   struct sockaddr_ll from; 
   socklen_t fromlen=sizeof(struct sockaddr_ll);
   // loop and receive frames
   while(true) {
      // loop until a non-empty frame is received
      while(true) {
         // clear buffer
         memset(buff, 0, ifconfig.mtu);
         // wait and receive a frame
         fromlen = sizeof(from);
         i = recvfrom(ifconfig.sockid, buff, ifconfig.mtu, 0,
                (struct sockaddr *) &from, &fromlen);
         if (i == -1) { // error
            cerr << "Cannot receive data: " << strerror(errno) << "\n"; 
            // sleep for 10 milliseconds and try again
            usleep(10000);
         } else { // nor error
            break; // exit the loop
         }
      }
      // parse a received frame
      parseReceivedFrame(buff);
   }
}
