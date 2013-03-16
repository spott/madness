CC=g++-4.7

CPPFLAGS=-std=c++11 -I./ -Wall -O3 -pedantic
LDFLAGS=
EXECUTABLE=main


SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) -c $(CPPFLAGS) $< -o $@

clean: 
	    rm -rf *.o main
