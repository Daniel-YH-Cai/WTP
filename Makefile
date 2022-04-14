all: wReceiver wSender TimeoutDemo

wReceiver:
	g++ -g -o wReceiver wReceiver.cpp

wSender:
	g++ -g -o wSender wSender.cpp

demo:
	g++ -g -o TimeoutDemo TimeoutDemo.cpp

give_permission:
	chmod u+x ./grader/tests

sanity:
	./grader/tests sanity

clean:
	rm -f  wReceiver wSender TimeoutDemo