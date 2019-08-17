objs/attuatori.o: src/attuatori.h objs/sensori.o objs/azioni.o
	cc -c src/attuatori.c -o objs/attuatori.o
	
objs/sensori.o: src/sensori.h objs/SocketConnection.o
	cc -c src/sensori.c -o objs/sensori.o

objs/SocketConnection.o: src/SocketConnection.h
	cc -c src/SocketConnection.c -o objs/SocketConnection.o

objs/azioni.o: src/azioni.h
	cc -c src/azioni.c -o objs/azioni.o
clean:
	rm objs/*.o
