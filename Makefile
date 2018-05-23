CC=gcc
CXXFLAGS += -march=native -frename-registers -funroll-loops -Ofast -ansi -DNDEBUG -Wall

default:
	rm -f *.o mstat
	make mstat

install: mstat
	cp mstat /usr/bin

clean:
	rm mstat
