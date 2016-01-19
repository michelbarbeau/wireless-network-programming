# Wireless Network Programming - The Linux Way


## Installing

`git clone https://github.com/michelbarbeau/wireless-network-programming.git`

## Configuration

Usually the default configuration is fine, but if your wireless interface is called something other than `wlan0`, modifying `label.h` is necessary.

`iwconfig` is a standard program in Ubuntu. It shows the status of wireless devices on your system.

Sample output:
```
barbeau@ubuntu:~/wireless-network-programming$ iwconfig
eth0      no wireless extensions.

wlan0     IEEE 802.11bgn  ESSID:"BELL150"  
          Mode:Managed  Frequency:2.412 GHz  Access Point: D8:6C:E9:2A:3C:CD   
          Bit Rate=1 Mb/s   Tx-Power=20 dBm   
          Retry short limit:7   RTS thr:off   Fragment thr:off
          Power Management:off
          Link Quality=65/70  Signal level=-45 dBm  
          Rx invalid nwid:0  Rx invalid crypt:0  Rx invalid frag:0
          Tx excessive retries:0  Invalid misc:54   Missed beacon:0

lo        no wireless extensions.
```

`label.h` contains the device that should be used when running this program. Currently it's a compile-time dependency and the program must be recompiled if the device needs to be changed. (ie. you get errors saying that `wlan0` does not exist)
```
barbeau@ubuntu:~/wireless-network-programming$ more label.h
// File: label.h
// Author: Michel Barbeau
// Version: January 16, 2016

//#define LABEL "eth0"
#define LABEL "wlan0"
```

## Building

Execute `make` in the project root directory.

If your system doesn't have g++ or make installed, on Ubuntu try installing `build-essential` using `apt-get`.

Example build output:
```
barbeau@ubuntu:~/wireless-network-programming$ make
g++ -std=c++0x -c Handler.cpp
g++ -std=c++0x -c WLAN.cpp
g++ -std=c++0x -c Sender.cpp
g++ -std=c++0x -c Receiver.cpp
g++ -std=c++0x WLAN.o Handler.o Sender.o -o sender
g++ -std=c++0x WLAN.o Handler.o Receiver.o -o receiver
```

## Running

Note: This program requires root access.

`receiver` output:
```
barbeau@ubuntu:~/wireless-network-programming$ sudo ./receiver
Initializing network interface  : wlan0
socket created!
ifindex is: 3
hardware address is: 1c:bd:b9:7e:b6:5a
MTU is: 1500
socket bind done
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff: E
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff: E
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
from 1c:bd:b9:7e:b6:5a to ff:ff:ff:ff:ff:ff: This is a test frame
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
from 1c:bd:b9:7e:b6:5a to ff:ff:ff:ff:ff:ff: This is a test frame
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
from d8:6c:e9:2a:3c:cb to ff:ff:ff:ff:ff:ff:
from 20:68:9d:97:c5:f3 to ff:ff:ff:ff:ff:ff:
^C
```
Sender sends out messages to the broadcast address.

`sender` output:
```
barbeau@ubuntu:~/wireless-network-programming$ sudo ./sender
Initializing network interface  : wlan0
socket created!
ifindex is: 3
hardware address is: 1c:bd:b9:7e:b6:5a
MTU is: 1500
socket bind done

```

`config.sh` puts your wlan0 interface into Ad-hoc mode.

`config.sh` output:
```
barbeau@ubuntu:~/wireless-network-programming$ sudo ./config.sh
wlan0     IEEE 802.11bgn  ESSID:"MyCell"  
          Mode:Ad-Hoc  Frequency:2.422 GHz  Cell: 00:00:00:00:00:01   
          Tx-Power=20 dBm   
          Retry short limit:7   RTS thr:off   Fragment thr:off
          Encryption key:off
          Power Management:off

```
