all: wReceiver wSender

wReceiver:
	g++ -g -o wReceiver wReceiver.cpp

wSender:
	g++ -g -o wSender wSender.cpp


clean:
	rm -f utils wReceiver wSender wc wr