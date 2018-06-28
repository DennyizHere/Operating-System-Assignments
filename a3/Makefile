HEADERS = tevent.h

default: tevent

tevent.o: tevent.c $(HEADERS)
	gcc -c -pthread tevent.c -o tevent.o

tevent: tevent.o
	gcc -pthread tevent.o -o tevent

clean:
	-rm -f tevent.o
	-rm -f tevent
