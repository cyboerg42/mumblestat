CC=gcc
CXXFLAGS += -march=native -O3 -frename-registers -funroll-loops -Ofast -ansi -DNDEBUG -Wall

default:
	rm -f *.o mstat
	make mstat
	strip ./mstat

install: mstat
	cp mstat /usr/bin

clean:
	rm mstat
