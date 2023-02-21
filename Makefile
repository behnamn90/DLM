# Manual Makefile use if cmake fails


export CC="/usr/local/Cellar/gcc/8.3.0/bin/g++-8"
LDIR=$(HOME)/CODE/libs/Boost/boost_1_61_0/
#CFLAGS=-I$(LDIR) -std=c++11 #-g -pg #-stdlib=libc++
CFLAGS=-I$(LDIR) -std=c++17
#CFLAGS=-I$(LDIR) -stdlib=libc++
#CFLAGS=-I$(LDIR) 
ODIR=./bin/
OBJS=$(ODIR)main.o $(ODIR)Domain.o $(ODIR)Crossover.o $(ODIR)Design.o $(ODIR)Constants.o $(ODIR)Common.o $(ODIR)Strand.o $(ODIR)Input.o $(ODIR)Staple.o $(ODIR)Scaffold.o $(ODIR)MyGraph.o $(ODIR)TempRamp.o $(ODIR)Transition.o $(ODIR)Simulation.o $(ODIR)TransitionManager.o $(ODIR)TransitionProperty.o $(ODIR)OrderParameter.o $(ODIR)OPManager.o $(ODIR)TransitionMap.o $(ODIR)StaplePool.o $(ODIR)StatManager.o $(ODIR)Loop.o $(ODIR)Node.o $(ODIR)Helix.o $(ODIR)FileIO.o
EXECUTABLE=$(ODIR)DLM
MDIR=./exe/
SDIR=./src/
HDIR=./Headers/


#DEPS = $(SDIR)Common.h $(SDIR)MyGraph.h $(SDIR)TempRamp.h $(SDIR)Constants.h $(SDIR)Design.h $(SDIR)Simulation.h
DEPS = $(HDIR)VertexProperty.h $(HDIR)EdgeProperty.h $(SDIR)Graph.h $(SDIR)Headers.h 

#%.o: %.cpp $(DEPS)
#		$(CC) -c -o $@ $< $(CFLAGS)


#all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXECUTABLE)

#$(ODIR)main.o: $(SDIR)main.cpp $(ODIR)Design.o $(ODIR)Constants.o $(ODIR)Common.o $(ODIR)Strand.o $(ODIR)Input.o $(ODIR)Staple.o $(ODIR)Scaffold.o $(ODIR)MyGraph.o $(ODIR)TempRamp.o $(ODIR)Transition.o $(ODIR)TransitionManager.o $(ODIR)Simulation.o $(ODIR)Converter.o $(ODIR)TransitionProperty.o $(ODIR)OrderParameter.o $(ODIR)TransitionMap.o $(ODIR)StaplePool.o $(ODIR)OPManager.o $(ODIR)StatManager.o
$(ODIR)main.o: $(MDIR)main.cpp $(ODIR)Simulation.o
	$(CC) $(CFLAGS) -c $(MDIR)main.cpp -o $(ODIR)main.o

$(ODIR)Simulation.o: $(SDIR)Simulation.cpp $(SDIR)Simulation.h $(ODIR)OPManager.o 
	$(CC) $(CFLAGS) -c $(SDIR)Simulation.cpp -o $(ODIR)Simulation.o

$(ODIR)OPManager.o: $(SDIR)OPManager.cpp $(SDIR)OPManager.h $(ODIR)StatManager.o 
	$(CC) $(CFLAGS) -c $(SDIR)OPManager.cpp -o $(ODIR)OPManager.o

$(ODIR)StatManager.o: $(SDIR)StatManager.cpp $(SDIR)StatManager.h $(ODIR)TransitionManager.o 
	$(CC) $(CFLAGS) -c $(SDIR)StatManager.cpp -o $(ODIR)StatManager.o

$(ODIR)TransitionManager.o: $(SDIR)TransitionManager.cpp $(SDIR)TransitionManager.h $(ODIR)Transition.o $(ODIR)TempRamp.o $(ODIR)TransitionMap.o $(ODIR)FileIO.o
	$(CC) $(CFLAGS) -c $(SDIR)TransitionManager.cpp -o $(ODIR)TransitionManager.o

$(ODIR)FileIO.o: $(SDIR)FileIO.cpp $(SDIR)FileIO.h $(ODIR)Design.o
	$(CC) $(CFLAGS) -c $(SDIR)FileIO.cpp -o $(ODIR)FileIO.o

$(ODIR)TempRamp.o: $(SDIR)TempRamp.cpp $(SDIR)TempRamp.h $(ODIR)Input.o
	$(CC) $(CFLAGS) -c $(SDIR)TempRamp.cpp -o $(ODIR)TempRamp.o

$(ODIR)Transition.o: $(SDIR)Transition.cpp $(SDIR)Transition.h $(ODIR)MyGraph.o $(ODIR)TransitionProperty.o
	$(CC) $(CFLAGS) -c $(SDIR)Transition.cpp -o $(ODIR)Transition.o

$(ODIR)TransitionMap.o: $(SDIR)TransitionMap.cpp $(SDIR)TransitionMap.h $(ODIR)TransitionProperty.o $(ODIR)Staple.o
	$(CC) $(CFLAGS) -c $(SDIR)TransitionMap.cpp -o $(ODIR)TransitionMap.o

$(ODIR)TransitionProperty.o: $(SDIR)TransitionProperty.cpp $(SDIR)TransitionProperty.h $(ODIR)Common.o
	$(CC) $(CFLAGS) -c $(SDIR)TransitionProperty.cpp -o $(ODIR)TransitionProperty.o

$(ODIR)MyGraph.o: $(SDIR)MyGraph.cpp $(SDIR)MyGraph.h $(ODIR)Loop.o
	$(CC) $(CFLAGS) -c $(SDIR)MyGraph.cpp -o $(ODIR)MyGraph.o

$(ODIR)Loop.o: $(SDIR)Loop.cpp $(SDIR)Loop.h $(ODIR)Design.o
	$(CC) $(CFLAGS) -c $(SDIR)Loop.cpp -o $(ODIR)Loop.o

$(ODIR)Design.o: $(SDIR)Design.cpp $(SDIR)Design.h $(ODIR)StaplePool.o
	$(CC) $(CFLAGS) -c $(SDIR)Design.cpp -o $(ODIR)Design.o

$(ODIR)StaplePool.o: $(SDIR)StaplePool.cpp $(SDIR)StaplePool.h $(ODIR)Helix.o $(ODIR)Constants.o $(ODIR)Input.o $(ODIR)Scaffold.o
	$(CC) $(CFLAGS) -c $(SDIR)StaplePool.cpp -o $(ODIR)StaplePool.o

$(ODIR)Helix.o: $(SDIR)Helix.cpp $(SDIR)Helix.h $(ODIR)Staple.o
	$(CC) $(CFLAGS) -c $(SDIR)Helix.cpp -o $(ODIR)Helix.o

$(ODIR)Staple.o: $(SDIR)Staple.cpp $(SDIR)Staple.h $(ODIR)Strand.o
	$(CC) $(CFLAGS) -c $(SDIR)Staple.cpp -o $(ODIR)Staple.o

$(ODIR)Scaffold.o: $(SDIR)Scaffold.cpp $(SDIR)Scaffold.h $(ODIR)Strand.o
	$(CC) $(CFLAGS) -c $(SDIR)Scaffold.cpp -o $(ODIR)Scaffold.o

$(ODIR)Strand.o: $(SDIR)Strand.cpp $(SDIR)Strand.h $(ODIR)Crossover.o
	$(CC) $(CFLAGS) -c $(SDIR)Strand.cpp -o $(ODIR)Strand.o

$(ODIR)Crossover.o: $(SDIR)Crossover.cpp $(SDIR)Crossover.h $(ODIR)Domain.o
	$(CC) $(CFLAGS) -c $(SDIR)Crossover.cpp -o $(ODIR)Crossover.o

$(ODIR)Domain.o: $(SDIR)Domain.cpp $(SDIR)Domain.h $(ODIR)Node.o
	$(CC) $(CFLAGS) -c $(SDIR)Domain.cpp -o $(ODIR)Domain.o

$(ODIR)Node.o: $(SDIR)Node.cpp $(SDIR)Node.h $(ODIR)OrderParameter.o $(HDIR)Graph.h
	$(CC) $(CFLAGS) -c $(SDIR)Node.cpp -o $(ODIR)Node.o

$(ODIR)OrderParameter.o: $(SDIR)OrderParameter.cpp $(SDIR)OrderParameter.h $(ODIR)Common.o
	$(CC) $(CFLAGS) -c $(SDIR)OrderParameter.cpp -o $(ODIR)OrderParameter.o

$(ODIR)Constants.o: $(SDIR)Constants.cpp $(SDIR)Constants.h $(ODIR)Input.o
	$(CC) $(CFLAGS) -c $(SDIR)Constants.cpp -o $(ODIR)Constants.o

$(ODIR)Input.o: $(SDIR)Input.cpp $(SDIR)Input.h $(ODIR)Common.o 
	$(CC) $(CFLAGS) -c $(SDIR)Input.cpp -o $(ODIR)Input.o

$(ODIR)Common.o: $(SDIR)Common.cpp $(SDIR)Common.h $(HDIR)Headers.h
	$(CC) $(CFLAGS) -c $(SDIR)Common.cpp -o $(ODIR)Common.o

clean:
	rm $(ODIR)*o $(ODIR)DLM
