# Get the interface name using "ifconfig", e.g., wlan0, and
# update the script.
# For help, see "man iwconfig".
# put the device down
ifconfig wlan0 down
# none cell, no access point
iwconfig wlan0 mode Ad-hoc
# cell name
iwconfig wlan0 essid MyCell
# channel
iwconfig wlan0 channel 3
# data rate
# iwconfig wlan0 rate 11MB
# identity of ad hoc network cell
iwconfig wlan0 ap 00:00:00:00:00:01
# put the device up
ifconfig wlan0 up
iwconfig wlan0
