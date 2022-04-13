all: wReceiver wSender TimeoutDemo

wReceiver:
	g++ -g -o wReceiver wReceiver.cpp

wSender:
	g++ -g -o wSender wSender.cpp

demo:
	g++ -g -o TimeoutDemo TimeoutDemo.cpp

clean:
	rm -f  wReceiver wSender TimeoutDemo