all: part1

part1: part1.c 
	gcc part1.c -o part1 -lpthread
	
JAVAC=javac
sources = $(wildcard *.java)
classes = $(sources:.java=.class)

all: $(classes)

clean :
	rm -f *.class
	rm -f part1
%.class : part2.java
	$(JAVAC) $<