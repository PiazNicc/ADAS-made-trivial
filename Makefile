incl/a.o: src/Attuatori.h incl/SocketConnection.o
	cc -c src/Attuatori.c -o incl/a.o

incl/SocketConnection.o: src/SocketConnection.h
	cc -c src/SocketConnection.c -o incl/SocketConnection.o
clean:
	rm incl/*.o