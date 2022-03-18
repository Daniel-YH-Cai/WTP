all: utils

utils:
	g++ -o -g utils utils.cpp

clean:
	rm -f utils