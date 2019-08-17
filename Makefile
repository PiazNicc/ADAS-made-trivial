incl/attuatori.o: src/attuatori.h incl/sensori.o incl/azioni.o
	cc -c src/attuatori.c -o incl/attuatori.o

incl/sensori.o: src/sensori.h incl/SocketConnection.o
	cc -c src/sensori.c -o incl/sensori.o

incl/SocketConnection.o: src/SocketConnection.h
	cc -c src/SocketConnection.c -o incl/SocketConnection.o

incl/azioni.o: src/azioni.h
	cc -c src/azioni.c -o incl/azioni.o
clean:
	rm incl/*.o