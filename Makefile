all: wReceiver wSender wc wr socket

wReceiver:
	g++ -g -o wReceiver wReceiver.cpp



wSender:
	g++ -g -o wSender wSender.cpp

socket:
	g++ -g -o socket UDPSocket.cpp

clean:
	rm -f utils wReceiver wSender wc wr