/*
 *
 *	Simulation.h
 * 	Author: Behnam
 *
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "OPManager.h"


class Simulation{
public:
	Simulation(OPManager*);
    ~Simulation(){}

    Inputs* inputs;
    Constants* constants;
    FileIO* ofiles;
    Design* design;
    MyGraph* G;
    TempRamp* ramp;
    TransitionManager* trManager;
    StatManager* statManager;
    OPManager* opManager;
    
    void run();
    void out_Iso();
    void out_AM();
    void write_movie();
    void prepare_config();
    
    string str;
    
    void test();
};

#endif

