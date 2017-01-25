CC=g++
LDIR=./Boost/boost_1_61_0/
CFLAGS=-I$(LDIR) -std=c++11 -stdlib=libc++
#CFLAGS=-I$(LDIR) -stdlib=libc++
#CFLAGS=-I$(LDIR) 
ODIR=./bin/
OBJS=$(ODIR)main.o $(ODIR)Simulation.o $(ODIR)MyGraph.o $(ODIR)TempRamp.o $(ODIR)Design.o $(ODIR)Constants.o $(ODIR)Common.o
EXECUTABLE=$(ODIR)DLM
SDIR=./src/


#DEPS = $(SDIR)Common.h $(SDIR)MyGraph.h $(SDIR)TempRamp.h $(SDIR)Constants.h $(SDIR)Design.h $(SDIR)Simulation.h

#%.o: %.cpp $(DEPS)
#		$(CC) -c -o $@ $< $(CFLAGS)


#all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

$(ODIR)main.o: $(SDIR)main.cpp $(ODIR)Simulation.o $(ODIR)MyGraph.o $(ODIR)TempRamp.o $(ODIR)Design.o $(ODIR)Constants.o $(ODIR)Common.o
	$(CC) $(CFLAGS) -c $(SDIR)main.cpp -o $(ODIR)main.o

$(ODIR)Simulation.o: $(SDIR)Simulation.cpp $(SDIR)Simulation.h $(ODIR)Common.o $(ODIR)Design.o $(ODIR)MyGraph.o $(ODIR)TempRamp.o
	$(CC) $(CFLAGS) -c $(SDIR)Simulation.cpp -o $(ODIR)Simulation.o

$(ODIR)MyGraph.o: $(SDIR)MyGraph.cpp $(SDIR)MyGraph.h $(ODIR)Common.o $(ODIR)Design.o $(ODIR)Constants.o
	$(CC) $(CFLAGS) -c $(SDIR)MyGraph.cpp -o $(ODIR)MyGraph.o

$(ODIR)TempRamp.o: $(SDIR)TempRamp.cpp $(SDIR)TempRamp.h $(ODIR)Common.o
	$(CC) $(CFLAGS) -c $(SDIR)TempRamp.cpp -o $(ODIR)TempRamp.o

$(ODIR)Design.o: $(SDIR)Design.cpp $(SDIR)Design.h $(ODIR)Common.o
	$(CC) $(CFLAGS) -c $(SDIR)Design.cpp -o $(ODIR)Design.o

$(ODIR)Constants.o: $(SDIR)Constants.cpp $(SDIR)Constants.h $(ODIR)Common.o
	$(CC) $(CFLAGS) -c $(SDIR)Constants.cpp -o $(ODIR)Constants.o

$(ODIR)Common.o: $(SDIR)Common.cpp $(SDIR)Common.h $(SDIR)Headers.h
	$(CC) $(CFLAGS) -c $(SDIR)Common.cpp -o $(ODIR)Common.o

clean:
	rm $(ODIR)*o $(ODIR)DLM
