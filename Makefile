CFLAGS=-std=c++0x -c

all: parser_test

test: parser_test
	./test

parser_test: parser.o parser_test.o
	$(CXX) parser.o parser_test.o -o test

parser.o: parser.cpp
	$(CXX) $(CFLAGS) parser.cpp

parser_test.o: parser_test.cpp
	$(CXX) $(CFLAGS) parser_test.cpp

clean:
	rm *o test
