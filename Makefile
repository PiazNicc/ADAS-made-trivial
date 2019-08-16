incl/a.o: src/Attuatori.h incl/sensori.o
	cc -c src/Attuatori.c -o incl/a.o

incl/sensori.o: src/sensori.h incl/SocketConnection.o
	cc -c src/sensori.c -o incl/sensori.o

incl/SocketConnection.o: src/SocketConnection.h
	cc -c src/SocketConnection.c -o incl/SocketConnection.o
clean:
	rm incl/*.o