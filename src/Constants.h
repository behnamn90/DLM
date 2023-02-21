/*
 *
 *	Constants.h
 * 	Author: Behnam
 *
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "Input.h"

//const double k_plus = 3.5 * pow(10,3); // mol-1  m3  s-1

const double l_ds = 0.34 * pow(10,-9); // m  bp-1
const double l_ss = 0.6 * pow(10,-9); // m  nt-1
const double lambda_ss = 1.8 * pow(10,-9); //  m

const double ss_hack = l_ss * lambda_ss;
const double ds_hack = l_ds * l_ds;
const double cs_hack = lambda_ss * lambda_ss;



const double salt_per_phosphate = -0.944;
const double salt_per_phosphate_hack = (0.368 * salt_per_phosphate) / 1000.;
const double conc_Mg = 12.5; // mol  m-3
const double conc_Tris = 40.; // mol  m-3

const double gas_constant = 0.00198720358509; // kcal / mol
const double default_crossover_size = 1; //should be 0

//SantaLucia
const double dH_AA = -7.6;	// kcal / mol
const double dH_AT = -7.2;
const double dH_TA = -7.2;
const double dH_CA = -8.5;
const double dH_GT = -8.4;
const double dH_CT = -7.8;
const double dH_GA = -8.2;
const double dH_CG = -10.6; 
const double dH_GC = -9.8;
const double dH_GG = -8.0;

const double dS_AA = -21.3 * 0.001; // (cal/K) / mol  ---> (kcal/K) / mol
const double dS_AT = -20.4 * 0.001;
const double dS_TA = -21.3 * 0.001;
const double dS_CA = -22.7 * 0.001;
const double dS_GT = -22.4 * 0.001;
const double dS_CT = -21.0 * 0.001;
const double dS_GA = -22.2 * 0.001;
const double dS_CG = -27.2 * 0.001;
const double dS_GC = -24.2 * 0.001;
const double dS_GG = -19.9 * 0.001;

const double dH_average = ( 2.*(dH_AA + dH_CA + dH_GT + dH_CT + dH_GA + dH_GG) + (dH_AT + dH_TA + dH_CG + dH_GC) ) / 16.;
const double dS_average = ( 2.*(dS_AA + dS_CA + dS_GT + dS_CT + dS_GA + dS_GG) + (dS_AT + dS_TA + dS_CG + dS_GC) ) / 16.;

const double dH_termAT = 2.2;
const double dS_termAT = 6.9 * 0.001;
const double dH_termCG = 0.0;
const double dS_termCG = 0.0 * 0.001;

const double dH_init = 0.2;
const double dS_init = -5.7 * 0.001;


class Constants{
public:
    Constants();
    Constants(Inputs*);
    virtual ~Constants();
    
    Inputs* inputs;

    void write_params(ofstream &);
    void change_gamma(double);

    double gamma_parameter;
    double C_parameter;
    double n_parameter;
};

#endif
