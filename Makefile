INPUT = bin/in.out
OUTPUT = bin/out.out
FLAGS = -O -Wall
LOC = include/
SOURCE = src/*.c
INCL = -I$(LOC)
LIBS = $(LOC)/*.h
OBJDIR = objs/
OBJS = input.o ecu.o attuatori.o sensori.o  \
	  SocketConnection.o  azioni.o output.o creazione.o
COMPONENTS = input.o ecu.o attuatori.o sensori.o \
	  SocketConnection.o  azioni.o creazione.o

all: $(INPUT) $(OUTPUT) moveObjects


moveObjects:
	mv *.o objs/

$(OUTPUT) : output.o
	cc $(FLAGS) -o $@  output.o



$(INPUT): $(COMPONENTS)
	cc $(FLAGS) -o $@ $(COMPONENTS)

$(OBJS): $(LIBS)
	cc $(FLAGS) $(INCL) -c $(SOURCE) 





clean:
	rm objs/*.o

