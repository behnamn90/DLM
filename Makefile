CC=g++
LDIR=./Boost/boost_1_61_0/
CFLAGS=-I$(LDIR) -std=c++11
ODIR=./bin/
EXECUTABLE=$(ODIR)DLM
SDIR=./src/

all: $(EXECUTABLE)

DEPS = $(SDIR)Common.h $(SDIR)MyGraph.h $(SDIR)TempRamp.h $(SDIR)Constants.h $(SDIR)Design.h $(SDIR)Simulation.h

#%.o: %.cpp $(DEPS)
#		$(CC) -c -o $@ $< $(CFLAGS)



$(EXECUTABLE): $(ODIR)main.o
	$(CC) $(CFLAGS) $(ODIR)main.o -o $(EXECUTABLE)

$(ODIR)main.o: $(SDIR)main.cpp $(DEPS) 
	$(CC) $(CFLAGS) -c $(SDIR)main.cpp -o $(ODIR)main.o

clean:
	rm $(ODIR)*o $(ODIR)DLM
