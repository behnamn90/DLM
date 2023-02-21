cmake_minimum_required(VERSION 3.24)
project(DLM)

set(CMAKE_OSX_ARCHITECTURES "arm64")

set(CMAKE_CXX_STANDARD 17)

set(boost_components graph random)
find_package(Boost MODULE REQUIRED COMPONENTS ${boost_components})
message(STATUS "Found Boost ${Boost_VERSION}")

add_executable(DLM exe/main.cpp Headers/EdgeProperty.h Headers/Graph.h Headers/Headers.h Headers/VertexProperty.h src/Common.cpp src/Common.h src/Constants.cpp src/Constants.h src/Crossover.cpp src/Crossover.h src/Design.cpp src/Design.h src/Domain.cpp src/Domain.h src/FileIO.cpp src/FileIO.h src/Helix.cpp src/Helix.h src/Input.cpp src/Input.h src/Loop.cpp src/Loop.h src/MyGraph.cpp src/MyGraph.h src/Node.cpp src/Node.h src/OPManager.cpp src/OPManager.h src/OrderParameter.cpp src/OrderParameter.h src/Scaffold.cpp src/Scaffold.h src/Simulation.cpp src/Simulation.h src/Staple.cpp src/Staple.h src/StaplePool.cpp src/StaplePool.h src/StatManager.cpp src/StatManager.h src/Strand.cpp src/Strand.h src/TempRamp.cpp src/TempRamp.h src/Transition.cpp src/Transition.h src/TransitionManager.cpp src/TransitionManager.h src/TransitionMap.cpp src/TransitionMap.h src/TransitionProperty.cpp src/TransitionProperty.h)

target_link_libraries(DLM PRIVATE Boost::graph Boost::random)