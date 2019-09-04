INPUT = bin/in.out
OUTPUT = bin/out.out
FLAGS = -O -Wall
LOC = include/
SOURCE = src/*.c
INCL = -I$(LOC)
LIBS = $(LOC)/*.h
DIR = log/ bin/
COMPONENTS = input.o ecu.o attuatori.o sensori.o log.o \
	  SocketConnection.o  azioni.o creazione.o

all:  $(DIR) $(INPUT) $(OUTPUT) clean

$(OUTPUT) : output.o
	cc $(FLAGS) -o $@  output.o

$(INPUT): $(COMPONENTS)
	cc $(FLAGS) -o $@ $(COMPONENTS)

$(COMPONENTS): $(LIBS)
	cc $(FLAGS) $(INCL) -c $(SOURCE) 

$(DIR):
	mkdir bin/ log/

clean:
	rm *.o

