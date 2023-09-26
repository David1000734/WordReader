# Make file for wordReader project

CCFLAGS = -Wall -Wextra -pedantic -g

all: wordReader

wordReader: wordReader.o
	g++ $(CCFLAGS) -o wordReader wordReader.o

wordReader.o: wordReader.cpp
	g++ $(CCFLAGS) -c wordReader.cpp

# -----
# clean by removing object files
clean:
	rm wordReader.o

# another version which will also remove text files created by program
clear:
	rm wordReader.o
	rm 1gms.txt
	rm 2gms.txt
	rm ast1.txt
	rm ast1log.txt

