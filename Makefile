CC=gcc
CXXFLAGS += -march=native -mtune=native -Os -frename-registers -funroll-loops -ansi -DNDEBUG -Wall

default:
	make clean
	make mstat
	strip ./mstat

install: mstat
	cp mstat /usr/bin

clean:
	rm -f *.o mstat
