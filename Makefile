all: WLAN.o Handler.o Sender.o Receiver.o sender receiver 

sender: WLAN.o Handler.o Sender.o
	g++ -std=c++0x WLAN.o Handler.o Sender.o -o sender

receiver: WLAN.o Handler.o Receiver.o
	g++ -std=c++0x WLAN.o Handler.o Receiver.o -o receiver

WLAN.o: WLAN.h WLAN.cpp Handler.o
	g++ -std=c++0x -c WLAN.cpp

Handler.o: Handler.h Handler.cpp
	g++ -std=c++0x -c Handler.cpp

Sender.o: Sender.h label.h Sender.cpp
	g++ -std=c++0x -c Sender.cpp

Receiver.o: Receiver.h label.h Receiver.cpp
	g++ -std=c++0x -c Receiver.cpp

clean:
	rm -f *.o
	rm -rf sender receiver









